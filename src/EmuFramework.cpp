//
// Created by ORZGG on 2018/12/31.
//

#include "EmuFramework.h"
#include <iostream>
#include <iomanip>
#include <MBC_header.h>
#include <unistd.h>
#include <time.h>
#include <sstream>
#include <Cartridge.h>


std::unique_ptr<MBC> EmuFramework::mmu ;
EmuFramework::RenderMethod EmuFramework::Render = nullptr ;
uint8_t* EmuFramework::frameBuffer = nullptr ;
const uint8_t* EmuFramework::keyboardStatus = nullptr ;

SDL_Window* EmuFramework::emuWin = nullptr ;
SDL_Renderer* EmuFramework::renderer = nullptr ;
SDL_Texture* EmuFramework::frame = nullptr ;

uint64_t EmuFramework::last = 0 ;
uint64_t EmuFramework::now = 0 ;
bool EmuFramework::running = true ;
SDL_Event EmuFramework::e ;
uint8_t EmuFramework::inputStatus = 0 ;
Stereo_Buffer EmuFramework::buf;
blip_sample_t EmuFramework::out_buf [EmuFramework::out_size] ;
Sound_Queue EmuFramework::sound;
int EmuFramework::speed = 0 ;

Uint8* EmuFramework::audio_pos; // global pointer to the audio buffer to be played
Uint32 EmuFramework::audio_len; // remaining length of the sample we have to play

EmuFramework::EmuFramework(  int argc, char* argv[]  )  :
    _argc( argc ), _argv( argv ) {

}

void EmuFramework::InitSDL_TextureWindow(const char *winName) {
    if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) ) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl ;
    } // if
    else {
        emuWin = SDL_CreateWindow( winName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                gbLCD_X*4, gbLCD_Y*4, SDL_WINDOW_SHOWN ) ;
        if( emuWin == NULL ) {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl ;
        } // if
        else {
            renderer = SDL_CreateRenderer( emuWin, -1, SDL_RENDERER_ACCELERATED ) ;
            frame = SDL_CreateTexture (
                            renderer,
                            SDL_PIXELFORMAT_ARGB8888,
                            SDL_TEXTUREACCESS_STREAMING,
                            160, 144
                    );

            SDL_SetRenderDrawColor( renderer, 255, 255, 255, SDL_ALPHA_OPAQUE ) ;
            SDL_RenderClear( renderer ) ;
        } // else
    } // else
}

void EmuFramework::StartUI() {
    // std::cout << "Welcome use GGBoy!!" << std::endl ;
    if ( _argc < 2 ) {
        std::cout << "Usage: GGBoy [rompath] [-flags]" << std::endl ;
    } // if
    else {
        if ( LoadRom( _argv[1] ) ) {

            StartEmu() ;


            // cpu->ExecuteBios() ;
        } // if
    } // else
}

void EmuFramework::UI_EventPolling(bool &running) {
    while( SDL_PollEvent( &e ) != 0 ) {
        keyboardStatus = SDL_GetKeyboardState( NULL ) ;
        switch ( e.type ) {
            case SDL_QUIT :
                running = false ;
                break ;
        }

        keyboardStatus[ SDL_SCANCODE_UP ] ? inputStatus |= UP : inputStatus &= ~UP ;
        keyboardStatus[ SDL_SCANCODE_DOWN ] ? inputStatus |= DOWN : inputStatus &= ~DOWN ;
        keyboardStatus[ SDL_SCANCODE_LEFT ] ? inputStatus |= LEFT : inputStatus &= ~LEFT ;
        keyboardStatus[ SDL_SCANCODE_RIGHT ] ? inputStatus |= RIGHT : inputStatus &= ~RIGHT ;
        keyboardStatus[ SDL_SCANCODE_RETURN ] ? inputStatus |= START : inputStatus &= ~START ;
        keyboardStatus[ SDL_SCANCODE_RSHIFT ] ? inputStatus |= SEL : inputStatus &= ~SEL ;
        keyboardStatus[ SDL_SCANCODE_Z ] ? inputStatus |= A : inputStatus &= ~A ;
        keyboardStatus[ SDL_SCANCODE_X ] ? inputStatus |= B : inputStatus &= ~B ;

        if ( keyboardStatus[ SDL_SCANCODE_A ] ) {
            speed += 500 ;
        } // if
        if ( keyboardStatus[ SDL_SCANCODE_S ] ) {
            speed -= 500 ;
        } // if

    } // while

    // printf( "%x\n", inputStatus ) ;
}
/*
void my_audio_callback(void *userdata, Uint8 *stream, int len) {

    if (audio_len ==0)
        return;

    len = ( len > audio_len ? audio_len : len );
    //SDL_memcpy (stream, audio_pos, len); 					// simply copy from one buffer into the other
    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another

    audio_pos += len;
    audio_len -= len;
}
*/
void EmuFramework::RenderSDL_Texture() {

    SDL_UpdateTexture( frame, NULL, frameBuffer, gbLCD_X * 4 ) ;
    SDL_RenderCopy( renderer, frame, NULL, NULL ) ;
    SDL_RenderPresent( renderer ) ;

    bool stereo = mmu->apu.end_frame( 68000 );
    buf.end_frame( 68000, stereo );

    if ( buf.samples_avail() >= out_size ) {
        size_t count = buf.read_samples(out_buf, out_size);
        sound.write(out_buf, count);
    } // if

    mmu->cpu_time = 0;

    now = SDL_GetPerformanceCounter() ;

    double fix = (double)((now - last)*1000000) / SDL_GetPerformanceFrequency() ;

    int64_t delayT = 16000 - static_cast<int>(fix) + speed ;
    if ( delayT > 0 )
        usleep( delayT ) ;
    else
        printf( "delay: %ld\n", delayT ) ;

    last = SDL_GetPerformanceCounter() ;

    UI_EventPolling( running ) ;
}

void EmuFramework::InitMBC() {
    switch ( cartridge->GetMBC_Code() ) {
        case 0 :
            mmu.reset( new MBC( cartridge ) ) ;
            break ;
        case 1 :
            mmu.reset( new MBC1( cartridge ) ) ;
            break ;
        case 2 :
            mmu.reset( new MBC2( cartridge ) ) ;
            break ;
    } // switch
}

void EmuFramework::StartEmu() {
    const int cyclePerFrame = 69905 ;
    int elapsedCycle = 0 ;
    bool debug = true ;

    InitMBC() ;

    cpu = new LR35902( *mmu ) ;

    frameBuffer = new uint8_t[ gbLCD_X * gbLCD_Y * 4 ] ;
    lcd = new LCD_Controller( frameBuffer, mmu->getMainMemory() ) ;

    InitSDL_TextureWindow( cartridge->getRomName().c_str() ) ;
    Render = &(EmuFramework::RenderSDL_Texture) ;

    blargg_err_t error = buf.set_sample_rate( 48000, 1000 );
    if ( error ) {
        std::cout << "APU error!!" << std::endl ;
        StopEmu() ;
        return ;
    } // if

    sound.start( 48000, 2 ) ;
    buf.clock_rate( 4194304 );
    mmu->apu.output( buf.center(), buf.left(), buf.right() );

    // mmu->Debug_ReadMemDump( "/home/orzgg/CLionProjects/cpu/Dracula.dump" ) ; // For Graphics debug
    //mmu->Debug_ReadBIOS( "/home/orzgg/CLionProjects/cpu/bios.gb" ) ;

    int i = 0 ;
    last = SDL_GetPerformanceCounter() ;
    while (running) {
        // UI_EventPolling( running ) ;
        // EmuFramework::Render() ; // for testing
        // lcd->Update( 4 ) ;


        if ( cpu->halting ) {
#ifdef DEBUG
            // printf( "%d\n", cpu->currentStatus.deltaCycle ) ;
#endif
        }

        else {
            cpu->ExecuteCurrentInstruction() ;
        } // else


        UpdateTimer() ;
        lcd->Update( cpu->currentStatus.deltaCycle ) ;

        cpu->CheckInterrupts() ;

        elapsedCycle += cpu->currentStatus.deltaCycle ;
        if ( elapsedCycle >= cyclePerFrame ) {
            elapsedCycle = 0 ;
        } // if
    } // while

    StopEmu() ;
}

void EmuFramework::StopEmu() {
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( emuWin );
    SDL_Quit();
}

bool EmuFramework::LoadRom(const char *romPath) {
    cartridge = new Cartridge( romPath ) ;
    if ( cartridge->IsValid() ) {
        /*
        std::cout << "Title: " << cartridge->getRomName() << std::endl
                  << "FileSize: " << cartridge->getRawRomSize() << std::endl
                  << "ROM bank number: " << cartridge->getRomBankNum() << std::endl
                  << "External RAM size: " << cartridge->getExtraRamSize() << std::endl
                  << "CGB support: " << std::boolalpha << cartridge->getIsCgbOnly() << std::endl ;

        int mbc = cartridge->GetMBC_Code() ;
        if ( mbc == 0 )
            std::cout << "No MBC in use." << std::endl ;
        else
            std::cout << "MBC in use: " << "MBC" << mbc << std::endl;
*/
        return true ;
    } // if
    else
        return false ;
}

void EmuFramework::UpdateTimer() {
    /* DIV reg change here */
    divCounter += cpu->currentStatus.deltaCycle ;
    if ( divCounter >= 256 ) {
        divCounter = 0 ;
        ++ mmu->getMainMemory()[ DIV ] ;
    } //if

    if ( mmu->ReadMemory( TAC ) & 0b00000100 != 0 ) {
        // Timer is on

        thisRoundCycle -= cpu->currentStatus.deltaCycle ;
        if ( thisRoundCycle <= 0 ) {
            /*Check current timer speed*/
            switch ( mmu->ReadMemory( TAC ) & 0b00000011 ) {
                case 0 :
                    thisRoundCycle = 4096 ;
                    break;
                case 1 :
                    thisRoundCycle = 262144 ;
                    break ;
                case 2 :
                    thisRoundCycle = 65536 ;
                    break ;
                case 3 :
                    thisRoundCycle = 16384 ;
                    break ;
            } // switch

            if ( mmu->ReadMemory( TIMA ) == 255 ) {
                mmu->WriteMemory( TIMA, mmu->ReadMemory( TMA ) ) ;
                RequestInterrupt( 2 ) ;
            } // if
            else {
                mmu->WriteMemory( TIMA, mmu->ReadMemory( TIMA ) + 1 ) ;
            } // else
        } // if
    } // if
} // UpdateTimer()

EmuFramework::~EmuFramework() {

}
