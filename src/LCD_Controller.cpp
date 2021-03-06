//
// Created by orzgg on 1/9/19.
//

#include "LCD_Controller.h"
#include <EmuFramework.h>
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <bitset>

LCD_Controller::LCD_Controller(uint8_t* frameBuffer, uint8_t* memptr) :
    LCDC( memptr[ RegAddr::LCDC ] ), STAT( memptr[ RegAddr::STAT ] ),
    SCY( memptr[ RegAddr::SCY ] ), SCX( memptr[ RegAddr::SCX ] ),
    LY( memptr[ RegAddr::LY ] ), LYC( memptr[ RegAddr::LYC ] ),
    BGP( memptr[ RegAddr::BGP ] ), OBP0( memptr[ RegAddr::OBP0 ] ),
    OBP1( memptr[ RegAddr::OBP1 ] ), WY( memptr[ RegAddr::WY ] ),
    WX( memptr[ RegAddr::WX ] ), _frameBuffer(frameBuffer), mainMemoryPtr(memptr)
{
}

void LCD_Controller::ChangeLCD_Mode(uint8_t mode) {
    STAT = STAT & 0b11111100 ;
    STAT = STAT | mode ;
    if ( mode < 3 && LR35902::TEST_BIT( STAT, mode + 3 ) )
        EmuFramework::RequestInterrupt( 1 ) ; // Request LCD interrupt
} // ChangeLCD_Mode

void LCD_Controller::CheckStatus() {
    if ( !LCD_IsOn() ) {
        // V-blanking
        cycle_scanCounter = cycle_perscanline ;
        LY  = 0 ;
        if ( GetLCD_Mode( STAT ) != 1 )
            ChangeLCD_Mode( 1 ) ;
    } // if
    else {
        if ( LY  >= 144 ) {
            // V-blank mode
            if ( GetLCD_Mode( STAT ) != 1 )
                ChangeLCD_Mode( 1 ) ;
        } // if
        else {
            if ( cycle_scanCounter >= MODE2_END ) {
                if ( GetLCD_Mode( STAT ) != 2 )
                    ChangeLCD_Mode( 2 ) ;
            } // if
            else if ( cycle_scanCounter >= MODE3_END ) {
                if ( GetLCD_Mode( STAT ) != 3 ) {
                    if ( LY  == mainMemoryPtr[ RegAddr::LYC ] ) {
                        STAT |= ( 1 << CoFlag ) ;
                        if ( STAT & ( 1 << CoITR ) )
                            EmuFramework::RequestInterrupt( 1 ) ;
                    } // if
                    else
                        STAT &= 0b11111011 ; // clear CoFlag

                    ChangeLCD_Mode( 3 ) ;

                } // if

            } // else if
            else if ( GetLCD_Mode( STAT ) != 0 ) {
                ChangeLCD_Mode( 0 ) ;
            } // else
        } // else
    } // else
}

uint8_t LCD_Controller::GetCurrentPixelX(bool inWindowArea, uint8_t x) {
    uint8_t windowX = static_cast<uint8_t>( (WX - 7) < 0 ? 0 : WX - 7 ) ;
    if (inWindowArea && x >= windowX)
        return x - windowX ;
    else
        return (x + SCX) % 256 ;
}

void LCD_Controller::RenderToBuffer() {

    if ( LR35902::TEST_BIT( LCDC , 0 ) ) {

        // Rendering Tiles
        // Determine to draw BG or WINDOW
        bool inWindowArea = LR35902::TEST_BIT( LCDC , 5 ) && LY >= WY   ;
        uint16_t tileMapBaseAddr = 0, pixelY = 0 ;

        if ( inWindowArea ) {
            // Determine the window pattern base address
            tileMapBaseAddr = LR35902::TEST_BIT( LCDC , 6 ) ? 0x9C00 : 0x9800 ;
            pixelY = LY  - WY  ;
        } // if
        else {
            // Determine the background pattern base address
            tileMapBaseAddr = LR35902::TEST_BIT( LCDC , 3 ) ? 0x9C00 : 0x9800 ;
            pixelY = (SCY + LY) % 256 ;
        } // else

        uint8_t currentTileY = ( pixelY / 8 ) ;   // rowNum / 8 ( 8 bit height per tile )

        for ( int x = 0 ; x < 160 ; ++x ) {

            uint8_t currentMapBlockX = GetCurrentPixelX(inWindowArea, x) / 8 ; // 8 bit width per tile
            uint8_t tileNum = mainMemoryPtr[ tileMapBaseAddr + currentTileY * 32 + currentMapBlockX ] ;
            uint16_t currentTileData_base =
                    LR35902::TEST_BIT( LCDC , 4 ) ? 0x8000 + tileNum * 16
                                                                      : 0x9000 + static_cast<int8_t>( tileNum ) * 16 ;
            uint8_t color= GetPixelColor( inWindowArea ? x : (x + SCX), pixelY % 8, currentTileData_base, BGP, false) ;
            uint32_t pixelAddress = 640 * LY + x * 4 ;
            memcpy( _frameBuffer + pixelAddress, DMG_Palette + color, 4 ) ;
        } // for
    } // if


    if ( LR35902::TEST_BIT( LCDC , 1 ) ) {
        // Sprites
        enum { yPos, xPos, patternNum, attr } ;
        enum { paletteNum = 4, xFlip, yFlip, priority } ;

        bool mode8x16 = LR35902::TEST_BIT( LCDC , 2 );

        for ( int spriteIndex = 39 ; spriteIndex >= 0 ; --spriteIndex ) {
            uint8_t* currentOAM = mainMemoryPtr + RegAddr::OAM + ( spriteIndex *4 ) ;

            uint8_t spriteHeight = mode8x16 ? 16 : 8 ;
            int16_t realY = currentOAM[ yPos ] - 16 ;
            int16_t realX = currentOAM[ xPos ] - 8 ;

            if ( LY >= realY && LY < realY + spriteHeight ) {
                uint8_t cRenderingY = LR35902::TEST_BIT( currentOAM[ attr ], yFlip ) ?
                                      spriteHeight - 1 - ( LY  - realY ) : LY  - realY ;

                for ( int x = 0 ; x < 8 ; ++x ) {
                    if ( realX + x < 0 || realX + x > 160 )
                        continue ;
                    uint32_t pixelAddress = 640 * LY  + ( realX + x ) * 4 ;

                    uint8_t cRenderingX = LR35902::TEST_BIT( currentOAM[ attr ], xFlip ) ?
                                          7 - x : x ;
                    uint8_t paletteOnUse = LR35902::TEST_BIT( currentOAM[attr], paletteNum ) ?
                                           OBP1 : OBP0 ;

                    uint8_t color = 0 ;
                    if ( mode8x16 && currentOAM[ patternNum ] % 2 != 0 )
                         color = GetPixelColor( cRenderingX, cRenderingY, 0x8000 + ( ( currentOAM[ patternNum ] - 1 ) << 4 ),
                                 paletteOnUse, true) ;
                    else
                        color = GetPixelColor( cRenderingX, cRenderingY, 0x8000 + ( currentOAM[ patternNum ] << 4 ),
                                               paletteOnUse, true) ;

                    if ( color != 5 ) {
                        // memcpy( _frameBuffer + pixelAddress, DMG_Palette + color, 4 ) ;
                        uint8_t* addr = _frameBuffer + pixelAddress ;
                        *reinterpret_cast<uint32_t*> ( addr ) = DMG_Palette[ color ] ;
                    }

                } // for
            } // if
        } // for
    } // if
}

uint8_t LCD_Controller::GetPixelColor(uint8_t px, uint8_t py, uint16_t baseAddr, uint8_t palette, bool spriteMode) {
    /**
     * One tile line is construct by two byte. ex:
     *      #px:          01234567
     *      22...22. ---> 00000000 ( data1 )
     *                    11000110 ( data2 )
     *
     * Combine data2 and data1s' #px bit, then we get pixel (px,py)'s color code.
     * NOTE: sequential is important (d2 combine to d1)
     * */

    //if ( py > 7 )
    //    printf( "what?\n" ) ;
/*
    uint8_t currentTileData1 = mainMemoryPtr[ baseAddr + ( py << 1 ) ] ;
    uint8_t currentTileData2 = mainMemoryPtr[ baseAddr + ( py << 1 ) + 1 ] ;

    uint8_t index = 0 ;
    uint8_t pixelMask = ( 1 << (7 - px) ) ;

    currentTileData1 = ( currentTileData1 & pixelMask ) >> (7 - px) ;
    currentTileData2 = ( currentTileData2 & pixelMask ) >> (7 - px) ;
    index = (currentTileData2 << 1) | currentTileData1 ;
*/

    uint8_t currentTileData1 = mainMemoryPtr[ baseAddr + ( py << 1 ) ] ;
    uint8_t currentTileData2 = mainMemoryPtr[ baseAddr + ( py << 1 ) + 1 ] ;

    uint8_t pixelX = px % 8 ;

    pixelX = 7 - pixelX ;
    uint8_t mask = 1 << pixelX ;

    currentTileData1 &= mask ;
    currentTileData2 &= mask ;

    currentTileData2 = ( pixelX == 0 ? currentTileData2 << 1 : currentTileData2 >> ( pixelX - 1 ) ) ;
    currentTileData1 = currentTileData1 >> pixelX ;

    uint8_t index = currentTileData2 | currentTileData1 ;
    if ( spriteMode && index == 0 )
        return 5 ;
    // printf( "b1:%x b2:%x x:%d y:%d --> %x\n", currentTileData1, currentTileData2, px, py, index ) ;
    switch ( index ) {
        case 0 :
            return palette & 0b00000011 ;
        case 1 :
            return (palette & 0b00001100) >> 2 ;
        case 2 :
            return (palette & 0b00110000) >> 4 ;
        case 3 :
            return (palette & 0b11000000) >> 6 ;
    } // switch
}

void LCD_Controller::Update( uint8_t instructionCycle ) {
    CheckStatus() ;

    if ( LCD_IsOn() ) {
        cycle_scanCounter -= instructionCycle ;
    }
    else {
        // printf( " %d(lcd off)\n", cycle_scanCounter ) ;
        return ;
    }

    if ( cycle_scanCounter <= 0 ) {
        // printf( "Counter zero!\n" ) ;
        cycle_scanCounter = cycle_perscanline ;

        if ( LY  < 144 ) {
            RenderToBuffer() ;
            if ( LY  == 143 ) {
                EmuFramework::RequestInterrupt( 0 ) ;
                EmuFramework::Render() ;
            } // else if
        } // if

        LY  = (LY  == 153) ? 0 : LY  + 1 ;
    } // if
}