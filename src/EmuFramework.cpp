//
// Created by ORZGG on 2018/12/31.
//

#include "EmuFramework.h"
#include <iostream>
#include <iomanip>
#include <MBC_header.h>
#include <Cartridge.h>

EmuFramework::EmuFramework(  int argc, char* argv[]  )  :
    _argc( argc ), _argv( argv ) {

}

void EmuFramework::StartUI() {
    std::cout << "Welcome use GGBoy!!" << std::endl ;
    if ( _argc < 2 ) {
        std::cout << "Usage: GGBoy [rompath] [-flags]" << std::endl ;
    } // if
    else {
        if ( LoadRom( _argv[1] ) ) {
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

        } // if
    } // else
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

EmuFramework::~EmuFramework() {

}
