//
// Created by ORZGG on 2018/12/31.
//

#include <cpu.h>
#include <MBC.h>
#include <string>
#include <Cartridge.h>
#include <LCD_Controller.h>
#include <Gb_Apu.h>
#include <Multi_Buffer.h>
#include <Sound_Queue.h>
#include <SDL2/SDL.h>

#ifndef CPU_EMUFRAMEWORK_H
#define CPU_EMUFRAMEWORK_H

using namespace lr35902 ;

class LCD_Controller ;

class EmuFramework {
public :
    typedef void (*RenderMethod)();

    enum {
        UP       = 0b00000100,
        DOWN     = 0b00001000,
        LEFT     = 0b00000010,
        RIGHT    = 0b00000001,
        START    = 0b10000000,
        SEL      = 0b01000000,
        A        = 0b00010000,
        B        = 0b00100000
    } ;

    EmuFramework( int argc, char* argv[] ) ;

    static uint8_t inputStatus ;
    void StartUI() ;
    bool LoadRom( const char* romPath ) ;
    void StartEmu() ;

    void PauseEmu() ;
    void Reset() ;

    static MBC& GetMMU() { return *(mmu.get()) ; }
    static RenderMethod Render ;

    static void UI_EventPolling( bool& running ) ;
    static void StopEmu() ;

    static inline void RequestInterrupt( uint8_t reqNum ) {
        mmu->getMainMemory()[ IF ] |= 1 << reqNum ;
    } // RequestInterrupt()

    // static std::fstream stausLog ;

    virtual ~EmuFramework();

private :
    LR35902* cpu = nullptr ;
    static std::unique_ptr<MBC> mmu ;

    LCD_Controller* lcd ;
    Cartridge* cartridge = nullptr ;

    static SDL_Window* emuWin ;
    static SDL_Renderer* renderer ;
    static SDL_Texture* frame  ;

    static SDL_Event e ;

    int _argc ;
    char** _argv ;

    uint16_t divCounter = 0 ;
    static uint8_t* frameBuffer ;
    static const uint8_t* keyboardStatus ;
    uint32_t thisRoundCycle = 4096 ;

    static uint64_t last ;
    static uint64_t now ;

    static bool running ;

    void UpdateTimer() ;
    void InitMBC() ;

    static void InitSDL_TextureWindow(const char *winName) ;
    static void RenderSDL_Texture() ;

    const static uint32_t cpuSpeed = 4194304 ;
    const static uint8_t gbLCD_X = 160 ;
    const static uint8_t gbLCD_Y = 144 ;

    static Stereo_Buffer buf;
    enum { out_size = 4096 };
    static blip_sample_t out_buf [out_size];
    static void my_audio_callback(void *userdata, Uint8 *stream, int len);
    static int speed ;
    static Sound_Queue sound;
    static Uint8 *audio_pos; // global pointer to the audio buffer to be played
    static Uint32 audio_len; // remaining length of the sample we have to play
};

#define MAINMEM EmuFramework::GetMMU().getMainMemory()
#define nSecPerCycle 238
#endif //CPU_EMUFRAMEWORK_H
