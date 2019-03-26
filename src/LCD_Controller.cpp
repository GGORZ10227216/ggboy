//
// Created by orzgg on 1/9/19.
//

#include "LCD_Controller.h"
#include <EmuFramework.h>
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <bitset>

uint8_t* LCD_Controller::_frameBuffer = NULL ;
uint32_t LCD_Controller::DMG_Palette[] = {
        0xffe0f8d0,
        0xff88c070,
        0xff346856,
        0xff081820
} ;

LCD_Controller::LCD_Controller(uint8_t* frameBuffer) :
    LCDC( MAINMEM[ RegAddr::LCDC ] ), STAT( MAINMEM[ RegAddr::STAT ] ),
    SCY( MAINMEM[ RegAddr::SCY ] ), SCX( MAINMEM[ RegAddr::SCX ] ),
    LY( MAINMEM[ RegAddr::LY ] ), LYC( MAINMEM[ RegAddr::LYC ] ),
    BGP( MAINMEM[ RegAddr::BGP ] ), OBP0( MAINMEM[ RegAddr::OBP0 ] ),
    OBP1( MAINMEM[ RegAddr::OBP1 ] ), WY( MAINMEM[ RegAddr::WY ] ),
    WX( MAINMEM[ RegAddr::WX ] )
{
    _frameBuffer = frameBuffer ;
}

void LCD_Controller::ChangeLCD_Mode(uint8_t mode) {
    STAT = STAT & 0b11111100 ;
    STAT = STAT | mode ;
    if ( mode < 3 && LR35902::TEST_BIT( STAT, mode + 3 ) )
        EmuFramework::RequestInterrupt( 1 ) ; // Request LCD interrupt
} // ChangeLCD_Mode

void LCD_Controller::CheckStatus() {
    switch (current_mode) {
        case VideoMode::ACCESS_OAM:
            if (cycle_counter >= CLOCKS_PER_SCANLINE_OAM) {
                cycle_counter = cycle_counter % CLOCKS_PER_SCANLINE_OAM;
                ChangeLCD_Mode( 3 ) ;
                current_mode = VideoMode::ACCESS_VRAM;
            }
            break;
        case VideoMode::ACCESS_VRAM:
            if (cycle_counter >= CLOCKS_PER_SCANLINE_VRAM) {
                cycle_counter = cycle_counter % CLOCKS_PER_SCANLINE_VRAM;
                current_mode = VideoMode::HBLANK;

                bool hblank_interrupt = LR35902::TEST_BIT( STAT, 3 ) ;

                if (hblank_interrupt) {
                    EmuFramework::RequestInterrupt( 1 ) ;
                }

                bool ly_coincidence_interrupt = LR35902::TEST_BIT( STAT, 6 );
                bool ly_coincidence = LYC == LY ;

                if (ly_coincidence_interrupt && ly_coincidence) {
                    EmuFramework::RequestInterrupt( 1 ) ;
                }
                STAT |= 0b00000100 ;

                ChangeLCD_Mode( 0 ) ;
            }
            break;
        case VideoMode::HBLANK:
            if (cycle_counter >= CLOCKS_PER_HBLANK) {

                RenderToBuffer() ;
                ++LY ;

                cycle_counter = cycle_counter % CLOCKS_PER_HBLANK;

                /* Line 145 (index 144) is the first line of VBLANK */
                if (LY == 144) {
                    current_mode = VideoMode::VBLANK;
                    ChangeLCD_Mode( 1 ) ;
                    EmuFramework::RequestInterrupt( 0 ) ;
                } else {
                    ChangeLCD_Mode( 2 ) ;
                    current_mode = VideoMode::ACCESS_OAM;
                }
            }
            break;
        case VideoMode::VBLANK:
            if (cycle_counter >= CLOCKS_PER_SCANLINE) {
                ++LY ;

                cycle_counter = cycle_counter % CLOCKS_PER_SCANLINE;

                /* Line 155 (index 154) is the last line */
                if (LY == 154) {
                    // RenderToBuffer() ;
                    EmuFramework::Render() ;
                    // buffer.reset();
                    LY = 0 ;
                    current_mode = VideoMode::ACCESS_OAM;
                    ChangeLCD_Mode( 2 ) ;
                };
            }
            break;
    }
}

uint8_t LCD_Controller::GetCurrentPixelX(bool inWindowArea, uint8_t x) {
    uint8_t windowX = static_cast<uint8_t>( MAINMEM[ 0xFF4B ] - 7 ) ;
    if (inWindowArea && x >= windowX)
        return x - windowX ;
    else
        return ( SCX + x ) % 256 ;
}

void LCD_Controller::RenderToBuffer() {

    if ( LR35902::TEST_BIT( MAINMEM[ RegAddr::LCDC ] , 0 ) ) {

        // Rendering Tiles
        // Determine to draw BG or WINDOW
        bool inWindowArea = LR35902::TEST_BIT( MAINMEM[ RegAddr::LCDC ] , 5 ) && MAINMEM[ RegAddr::LY ]  >= MAINMEM[ RegAddr::WY ]   ;
        uint16_t tileMapBaseAddr = 0, pixelY = 0 ;

        if ( inWindowArea ) {
            // Determine the window pattern base address
            tileMapBaseAddr = LR35902::TEST_BIT( MAINMEM[ RegAddr::LCDC ] , 6 ) ? 0x9C00 : 0x9800 ;
            pixelY = MAINMEM[ RegAddr::LY ]  - MAINMEM[ RegAddr::WY ]  ;
        } // if
        else {
            // Determine the background pattern base address
            tileMapBaseAddr = LR35902::TEST_BIT( MAINMEM[ RegAddr::LCDC ] , 3 ) ? 0x9C00 : 0x9800 ;
            pixelY = (SCY + LY) % 256 ;
        } // else

        uint8_t currentTileY = ( pixelY / 8 ) ;   // rowNum / 8 ( 8 bit height per tile )

        for ( int x = 0 ; x < 160 ; ++x ) {

            uint8_t currentMapBlockX = GetCurrentPixelX(inWindowArea, x) / 8 ; // 8 bit width per tile
            uint8_t tileNum = MAINMEM[ tileMapBaseAddr + currentTileY * 32 + currentMapBlockX ] ;
            uint16_t currentTileData_base =
                    LR35902::TEST_BIT( MAINMEM[ RegAddr::LCDC ] , 4 ) ? 0x8000 + tileNum * 16
                    : 0x9000 + static_cast<int8_t>( tileNum ) * 16 ;
            uint8_t color= GetPixelColor( (x + SCX), pixelY % 8, currentTileData_base, BGP, false) ;
            uint32_t pixelAddress = 640 * MAINMEM[ RegAddr::LY ] + x * 4 ;
            memcpy( _frameBuffer + pixelAddress, DMG_Palette + color, 4 ) ;
        } // for
    } // if


    if ( LR35902::TEST_BIT( MAINMEM[ RegAddr::LCDC ] , 1 ) ) {
        // Sprites
        enum { yPos, xPos, patternNum, attr } ;
        enum { paletteNum = 4, xFlip, yFlip, priority } ;

        bool mode8x16 = LR35902::TEST_BIT( MAINMEM[ RegAddr::LCDC ] , 2 );

        for ( int spriteIndex = 0 ; spriteIndex < 40 ; ++spriteIndex ) {
            uint8_t* currentOAM = MAINMEM + RegAddr::OAM + ( spriteIndex *4 ) ;
            uint8_t spriteHeight = mode8x16 ? 16 : 8 ;
            int16_t realY = currentOAM[ yPos ] - 16 ;
            int16_t realX = currentOAM[ xPos ] - 8 ;

            if ( MAINMEM[ RegAddr::LY ] >= realY && MAINMEM[ RegAddr::LY ] < realY + spriteHeight ) {
                uint8_t cRenderingY = LR35902::TEST_BIT( currentOAM[ attr ], yFlip ) ?
                                      spriteHeight - 1 - ( MAINMEM[ RegAddr::LY ]  - realY ) : MAINMEM[ RegAddr::LY ]  - realY ;

                for ( int x = 0 ; x < 8 ; ++x ) {
                    if ( realX + x < 0 || realX + x > 160 )
                        continue ;
                    uint32_t pixelAddress = 640 * MAINMEM[ RegAddr::LY ]  + ( realX + x ) * 4 ;
                    uint32_t bgColor = *reinterpret_cast<uint32_t*>( _frameBuffer + pixelAddress ) ;

                    // memcpy( &bgColor, _frameBuffer + pixelAddress, 4 ) ;
/*
                    bool ignore = ( LR35902::TEST_BIT( currentOAM[ attr ], priority ) && bgColor != DMG_Palette[0] ) ;
                    if ( ignore )
                        continue ;

*/
                    uint8_t cRenderingX = LR35902::TEST_BIT( currentOAM[ attr ], xFlip ) ?
                                          7 - x : x ;
                    uint8_t paletteOnUse = LR35902::TEST_BIT( currentOAM[attr], paletteNum ) ?
                                            MAINMEM[ RegAddr::OBP1 ] : MAINMEM[ RegAddr::OBP0 ]  ;
                    // printf( "[%d,#%x]palette= %x\n", LR35902::TEST_BIT( currentOAM[attr], paletteNum ), spriteIndex, paletteOnUse ) ;

                    uint8_t color = GetPixelColor( cRenderingX, cRenderingY,
                            0x8000 + ( currentOAM[ patternNum ] << 4 ), paletteOnUse,
                            true ) ;

                    if ( color != 5 ) {
                        // memcpy( _frameBuffer + pixelAddress, DMG_Palette + color, 4 ) ;
                        uint8_t* addr = _frameBuffer + pixelAddress ;
                        *reinterpret_cast<uint32_t*> ( addr ) = DMG_Palette[ color ] ;
                    }
                } // for

                //printf( "currentOAM= %x\n", spriteIndex ) ;
                //EmuFramework::Render() ;
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
    uint8_t currentTileData1 = MAINMEM[ baseAddr + ( py << 1 ) ] ;
    uint8_t currentTileData2 = MAINMEM[ baseAddr + ( py << 1 ) + 1 ] ;

    uint8_t index = 0 ;
    uint8_t pixelMask = ( 1 << (7 - px) ) ;

    currentTileData1 = ( currentTileData1 & pixelMask ) >> (7 - px) ;
    currentTileData2 = ( currentTileData2 & pixelMask ) >> (7 - px) ;
    index = (currentTileData2 << 1) | currentTileData1 ;
*/

    uint8_t currentTileData1 = MAINMEM[ baseAddr + ( py << 1 ) ] ;
    uint8_t currentTileData2 = MAINMEM[ baseAddr + ( py << 1 ) + 1 ] ;

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
    cycle_counter += instructionCycle;
    CheckStatus() ;
}