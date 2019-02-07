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

MBC* EmuFramework::mmu = nullptr ;
EmuFramework::RenderMethod EmuFramework::Render = nullptr ;
uint8_t* EmuFramework::frameBuffer = nullptr ;

SDL_Window* EmuFramework::emuWin = nullptr ;
SDL_Renderer* EmuFramework::renderer = nullptr ;
SDL_Texture* EmuFramework::frame = nullptr ;

EmuFramework::EmuFramework(  int argc, char* argv[]  )  :
    _argc( argc ), _argv( argv ) {

}

void EmuFramework::InitSDL_TextureWindow(const char *winName) {
    if ( SDL_Init( SDL_INIT_VIDEO ) ) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl ;
    } // if
    else {
        emuWin = SDL_CreateWindow( winName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                gbLCD_X*3, gbLCD_Y*3, SDL_WINDOW_SHOWN ) ;
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
    std::cout << "Welcome use GGBoy!!" << std::endl ;
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
        if( e.type == SDL_QUIT ) {
            running = false ;
        } // if
    } // while
}

void EmuFramework::RenderSDL_Texture() {
    SDL_UpdateTexture( frame, NULL, frameBuffer, gbLCD_X * 4 ) ;
    SDL_RenderCopy( renderer, frame, NULL, NULL ) ;
    SDL_RenderPresent( renderer ) ;
    usleep( 16000 ) ;
}

void EmuFramework::InitMBC() {
    switch ( cartridge->GetMBC_Code() ) {
        case 0 :
            mmu = new MBC( cartridge ) ;
            break ;
        case 1 :
            mmu = new MBC1( cartridge ) ;
            break ;
        case 2 :
            mmu = new MBC2( cartridge ) ;
            break ;
    } // switch
}

void EmuFramework::StartEmu() {
    const int cyclePerFrame = 69905 ;
    int elapsedCycle = 0 ;
    bool running = true ;

    InitMBC() ;
    cpu = new LR35902() ;

    frameBuffer = new uint8_t[ gbLCD_X * gbLCD_Y * 4 ] ;
    lcd = new LCD_Controller( frameBuffer ) ;

    InitSDL_TextureWindow( cartridge->getRomName().c_str() ) ;
    Render = &(EmuFramework::RenderSDL_Texture) ;

    mmu->Debug_ReadMemDump( "/home/orzgg/Documents/cpu/bgbtest2.dump" ) ; // For Graphics debug
    timespec inst ;
    std::stringstream ss ;

    int i = 0 ;
    while (running) {
        UI_EventPolling( running ) ;
        lcd->Update( 4 ) ;
        //std::cout << seconds << std::endl ;
        // EmuFramework::Render() ; // for testing
        /*
        cpu->ExecuteCurrentInstruction() ;
        UpdateTimer() ;
        // UpdateGraphics() ;
        cpu->CheckInterrupts() ;

        elapsedCycle += cpu->currentStatus.deltaCycle ;
        if ( elapsedCycle >= cyclePerFrame ) {
            elapsedCycle = 0 ;
            // Render() ;
        } // if
         */
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
