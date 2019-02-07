//
// Created by ORZGG on 2018/12/31.
//

#include <cpu.h>
#include <MBC.h>
#include <string>
#include <Cartridge.h>
#include <LCD_Controller.h>
#include <SDL2/SDL.h>

#ifndef CPU_EMUFRAMEWORK_H
#define CPU_EMUFRAMEWORK_H

using namespace lr35902 ;

class LCD_Controller ;

class EmuFramework {
public :
    typedef void (*RenderMethod)();
    EmuFramework( int argc, char* argv[] ) ;
    void StartUI() ;
    bool LoadRom( const char* romPath ) ;
    void StartEmu() ;

    void PauseEmu() ;
    void Reset() ;

    static MBC* GetMMU() { return mmu ; }
    static RenderMethod Render ;

    void UI_EventPolling( bool& running ) ;
    static void StopEmu() ;

    static inline void RequestInterrupt( uint8_t reqNum ) {
        mmu->getMainMemory()[ IF ] |= 1 << reqNum ;
    } // RequestInterrupt()

    virtual ~EmuFramework();

private :
    LR35902* cpu = nullptr ;
    static MBC* mmu ;

    LCD_Controller* lcd ;
    Cartridge* cartridge = nullptr ;

    static SDL_Window* emuWin ;
    static SDL_Renderer* renderer ;
    static SDL_Texture* frame  ;

    SDL_Event e ;

    int _argc ;
    char** _argv ;

    uint16_t divCounter = 0 ;
    static uint8_t* frameBuffer ;
    uint32_t thisRoundCycle = 4096 ;

    void UpdateTimer() ;
    void InitMBC() ;

    static void InitSDL_TextureWindow(const char *winName) ;
    static void RenderSDL_Texture() ;

    const static uint32_t cpuSpeed = 4194304 ;
    const static uint8_t gbLCD_X = 160 ;
    const static uint8_t gbLCD_Y = 144 ;
};

#define MAINMEM EmuFramework::GetMMU()->getMainMemory()
#define nSecPerCycle 238
#endif //CPU_EMUFRAMEWORK_H
