//
// Created by orzgg on 1/9/19.
//

#include <cstdint>
#include <cpu.h>
#include <SDL2/SDL.h>

#ifndef CPU_LCD_CONTROLLER_H
#define CPU_LCD_CONTROLLER_H


class LCD_Controller {
public :
    LCD_Controller() = delete ;
    LCD_Controller(uint8_t* frameBuffer, uint8_t* memptr) ;

    inline bool LCD_IsOn() { return LCDC & ( 1 << 7 ) ; }
    void Update( uint8_t instructionCycle ) ;
    void RenderToBuffer() ;
    void RenderSprites() ;

    int16_t cycle_scanCounter = 456 ;
private :
    uint8_t* _frameBuffer ;
    uint8_t* mainMemoryPtr = nullptr ;

    uint8_t& LCDC, &STAT, &SCY, &SCX, &LY, &LYC ;
    uint8_t& BGP, &OBP0, &OBP1, &WY, &WX ;

    uint32_t DMG_Palette[ 4 ] = {
            0xffe0f8d0,
            0xff88c070,
            0xff346856,
            0xff081820
    } ;

    enum { cycle_perscanline = 456, LCD_MODE0 = 0,
           LCD_MODE1 = 1, CoFlag = 2, LCD_ITR_MODE0 = 3,
           LCD_ITR_MODE1 = 4, LCD_ITR_MODE2 = 5,
           CoITR = 6, MODE2_END = 376, MODE3_END = 204,
           SCR_W = 160, SCR_H = 144
    } ;

    void CheckStatus() ;
    uint8_t GetPixelColor(uint8_t px, uint8_t py, uint16_t baseAddr, uint8_t palette, bool spriteMode) ;
    uint8_t GetCurrentPixelX( bool inWindowArea, uint8_t x ) ;

    void ChangeLCD_Mode( uint8_t mode ) ;

    inline uint8_t GetLCD_Mode( uint8_t state ) {
        return state & 0b00000011 ;
    }
};


#endif //CPU_LCD_CONTROLLER_H
