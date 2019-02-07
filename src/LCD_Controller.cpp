//
// Created by orzgg on 1/9/19.
//

#include "LCD_Controller.h"
#include <EmuFramework.h>
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <bitset>

LCD_Controller::LCD_Controller(uint8_t* frameBuffer) :
    LCDC( MAINMEM[ RegAddr::LCDC ] ), STAT( MAINMEM[ RegAddr::STAT ] ),
    SCY( MAINMEM[ RegAddr::SCY ] ), SCX( MAINMEM[ RegAddr::SCX ] ),
    LY( MAINMEM[ RegAddr::LY ] ), LYC( MAINMEM[ RegAddr::LYC ] ),
    BGP( MAINMEM[ RegAddr::BGP ] ), OBP0( MAINMEM[ RegAddr::OBP0 ] ),
    OBP1( MAINMEM[ RegAddr::OBP1 ] ), WY( MAINMEM[ RegAddr::WY ] ),
    WX( MAINMEM[ RegAddr::WX ] ), _frameBuffer(frameBuffer)
{

}

void LCD_Controller::CheckStatus() {
    if ( !LCD_IsOn() ) {
        // V-blanking
        cycle_scanCounter = cycle_perscanline ;
        LY = 0 ;
        ChangeLCD_Mode( 1 ) ;
    } // if
    else {
        uint8_t lastStatus = STAT  ;
        if ( LY >= 144 ) {
            // V-blank mode
            ChangeLCD_Mode( 1 ) ;
        } // if
        else {
            if ( cycle_scanCounter >= MODE2_END ) {
                ChangeLCD_Mode( 2 ) ;
            } // if
            else if ( cycle_scanCounter >= MODE3_END ) {
                ChangeLCD_Mode( 3 ) ;
            } // else if
            else {
                ChangeLCD_Mode( 0 ) ;
            } // else
        } // else

        if ( GetLCD_Mode( lastStatus ) != GetLCD_Mode( STAT ) )
            EmuFramework::RequestInterrupt( 1 ) ; // Request LCD interrupt
        if ( LY == LYC ) {
            STAT |= ( 1 << CoFlag ) ;
            if ( STAT & ( 1 << CoITR ) )
                EmuFramework::RequestInterrupt( 1 ) ;
        } // if
        else
            STAT &= 0b11111011 ; // clear CoFlag
    } // else
}

uint8_t LCD_Controller::GetCurrentPixelX(bool inWindowArea, uint8_t x) {
    uint8_t windowX = static_cast<uint8_t>( WX - 7 ) ;
    if (inWindowArea && x >= windowX)
        return x - windowX ;
    else
        return SCX + x ;
}

void LCD_Controller::RenderToBuffer() {

    if ( LR35902::TEST_BIT( LCDC, 0 ) ) {
        // Rendering Tiles

        // Determine to draw BG or WINDOW
        bool inWindowArea = LR35902::TEST_BIT( LCDC, 5 ) && LY >= WY  ;

        uint16_t tileMapBaseAddr = 0, pixelY = 0 ;
        if ( inWindowArea ) {
            // Determine the window pattern base address
            tileMapBaseAddr = LR35902::TEST_BIT( LCDC, 6 ) ? 0x9C00 : 0x9800 ;
            pixelY = LY - WY ;
        } // if
        else {
            // Determine the background pattern base address
            tileMapBaseAddr = LR35902::TEST_BIT( LCDC, 3 ) ? 0x9C00 : 0x9800 ;
            pixelY = SCY + LY ;
        } // else

        uint8_t currentTileY = ( pixelY / 8 ) ;   // rowNum / 8 ( 8 bit height per tile )
        for ( int x = 0 ; x < 160 ; ++x ) {
            uint8_t currentTileX = GetCurrentPixelX(inWindowArea, x) / 8 ; // 8 bit width per tile
            uint8_t tileNum = MAINMEM[ tileMapBaseAddr + currentTileY * 32 + currentTileX ] ;
            uint16_t currentTileData_base =
                    LR35902::TEST_BIT( LCDC, 4 ) ? 0x8000 + tileNum * 16
                    : 0x9000 + static_cast<int8_t>( tileNum ) * 16 ;
            uint8_t color= GetPixelColor(x, pixelY % 8, currentTileData_base, BGP) ;

            uint32_t pixelAddress = 640 * LY + x * 4 ;
            memcpy( _frameBuffer + pixelAddress, DMG_Palette + color, 4 ) ;
        } // for
    } // if

    if ( LR35902::TEST_BIT( LCDC, 1 ) ) {
        // Sprites
        enum { yPos, xPos, patternNum, attr } ;
        enum { paletteNum = 4, xFlip, yFlip, priority } ;

        bool mode8x16 = LR35902::TEST_BIT( LCDC, 2 ) ;

        for ( int spriteIndex = 0 ; spriteIndex < 40 ; ++spriteIndex ) {
            uint8_t* currentOAM = MAINMEM + RegAddr::OAM + ( spriteIndex *4 ) ;

            uint8_t spriteHeight = mode8x16 ? 16 : 8 ;
            uint8_t realY = currentOAM[ yPos ] - 16 ;
            uint8_t realX = currentOAM[ xPos ] - 8 ;

            if ( LY >= realY && LY < realY + spriteHeight ) {

                /* attr bit7 still not implement!!*/
                // printf( "spriteIndex = %x(tile# %x), x = %x y = %x\n", spriteIndex, currentOAM[patternNum], realX, realY ) ;
                uint8_t cRenderingY = LR35902::TEST_BIT( currentOAM[ attr ], yFlip ) ?
                        7 - (LY - realY) : LY - realY ;
                for ( int x = 0 ; x < 8 ; ++x ) {
                    uint8_t cRenderingX = LR35902::TEST_BIT( currentOAM[ attr ], xFlip ) ?
                                          7 - x : x ;
                    uint8_t& paletteOnUse = LR35902::TEST_BIT( currentOAM[attr], paletteNum ) ?
                            OBP1 : OBP0 ;
                    uint8_t color = GetPixelColor( cRenderingX, cRenderingY,
                            0x8000 + ( currentOAM[ patternNum ] << 4 ), paletteOnUse
                            ) ;

                    uint32_t pixelAddress = 640 * LY + ( realX + x ) * 4 ;
                    memcpy( _frameBuffer + pixelAddress, DMG_Palette + color, 4 ) ;
                } // for
            } // if
        } // for
    } // if

}

uint8_t LCD_Controller::GetPixelColor(uint8_t px, uint8_t py, uint16_t baseAddr, uint8_t& palette) {
    /**
     * One tile line is construct by two byte. ex:
     *
     *      22...22. ---> 00000000 ( data1 )
     *                    11000110 ( data2 )
     *
     * Combine data2 and data1s' #py bit, then we get pixel (0,py)'s color code.
     * NOTE: sequential is important (d2 combine to d1)
     * */

    uint8_t currentTileData1 = MAINMEM[ baseAddr + ( py << 1 ) ] ;
    uint8_t currentTileData2 = MAINMEM[ baseAddr + ( py << 1 ) + 1 ] ;

    uint8_t pixelX = (SCX + px) % 8 ;

    pixelX = 7 - pixelX ;
    uint8_t mask = 1 << pixelX ;

    currentTileData1 &= mask ;
    currentTileData2 &= mask ;

    currentTileData2 = ( pixelX == 0 ? currentTileData2 << 1 : currentTileData2 >> ( pixelX - 1 ) ) ;
    currentTileData1 = currentTileData1 >> pixelX ;

    uint8_t index = currentTileData2 | currentTileData1 ;

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

    if ( LCD_IsOn() )
        cycle_scanCounter -= instructionCycle ;
    else
        return ;

    if ( cycle_scanCounter <= 0 ) {
        cycle_scanCounter = cycle_perscanline ;
        ++ LY ;

        if ( LY == 144 ) {
            EmuFramework::RequestInterrupt( 0 ) ;
            EmuFramework::Render() ;
        } // if
        else if ( LY < 144 ) {
            RenderToBuffer() ;
        } // else if
        else if ( LY > 153 ) {
            LY = -1 ;
        } // else if
    } // if
}