//
// Created by ORZGG on 2018/12/30.
//

#include "MBC.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include <EmuFramework.h>

using namespace lr35902 ;

MBC::MBC(Cartridge* rom) {
    if ( mainMemory == nullptr )
        mainMemory = new uint8_t[ 0x10000 ] ;
    else
        memset( mainMemory, 0, 0x10000 ) ;

    if ( rom != nullptr )
        memcpy( mainMemory, rom->getRawRomData(), 0x8000 ) ; // copy the bank#00 of rom
}

MBC::~MBC() {
    delete ramBanks ;
    ramBanks = nullptr ;
    delete romBanks ;
    romBanks = nullptr ;
    delete mainMemory ;
    mainMemory = nullptr ;
}

void MBC::WriteMemory(const uint16_t addr, const uint8_t value) {
    if ( addr < 0x8000 ) {
        // dont allow any writing to the read only memory
    } // if
    else if ( ( addr >= 0xE000 ) && (addr < 0xFE00) ) {
        // writing to ECHO ram also writes in RAM
        mainMemory[ addr ] = value ;
        mainMemory[ addr - 0x2000 ] = value ;
    } // else if
    else if ( ( addr >= 0xFEA0 ) && (addr < 0xFEFF) ) {
        // this area is restricted
    } // else if
/*
    else if ( addr == 0xff00 ) {
        mainMemory[ addr ] = fixvalue ;
    } // else if
*/
    else if ( addr == 0xFF46 ) {
        DMA_Write( value ) ;
    } // else if
    else {
        // no control needed over this area so write to memory
        if ( addr >= 0xff10 && addr <= 0xff3f )
            apu.write_register( cpu_time, addr, value );
        /*
        if ( addr == 0xffff ) {
            if ( value & 0b00000001 )
                printf( "Vertical Blanking itrp enable\n" ) ;
            else
                printf( "Vertical Blanking itrp disable\n" ) ;

            if ( value & 0b00000010 )
                printf( "LCDC itrp enable\n" ) ;
            else
                printf( "LCDC itrp disable\n" ) ;

            if ( value & 0b00000100 )
                printf( "Timer overflow itrp enable\n" ) ;
            else
                printf( "Timer overflow itrp disable\n" ) ;

            if ( value & 0b00001000 )
                printf( "Serial itrp enable\n" ) ;
            else
                printf( "Serial itrp disable\n" ) ;

            if ( value & 0b00010000 )
                printf( "terminal negative itrp enable\n" ) ;
            else
                printf( "terminal negative itrp disable\n" ) ;
        } // if
*/
        mainMemory[ addr ] = value ;
    } // else
}

uint8_t MBC::ReadMemory(const uint16_t addr) {
    if ( addr == 0xff00 ) {
        uint8_t realStatus = 0 ;
        switch ( mainMemory[ 0xff00 ] & 0b00110000 ) {
            case 0x20 :
                EmuFramework::inputStatus & EmuFramework::UP ?
                    realStatus &= ~EmuFramework::UP : realStatus |= EmuFramework::UP  ;
                EmuFramework::inputStatus & EmuFramework::DOWN ?
                    realStatus &= ~EmuFramework::DOWN : realStatus |= EmuFramework::DOWN;
                EmuFramework::inputStatus & EmuFramework::LEFT ?
                    realStatus &= ~EmuFramework::LEFT : realStatus |= EmuFramework::LEFT ;
                EmuFramework::inputStatus & EmuFramework::RIGHT ?
                        realStatus &= ~EmuFramework::RIGHT : realStatus |= EmuFramework::RIGHT ;
                break ;
            case 0x10 :
                EmuFramework::inputStatus & EmuFramework::A ?
                    realStatus &= ~( EmuFramework::A >> 4 ) : realStatus |= EmuFramework::A >> 4 ;
                EmuFramework::inputStatus & EmuFramework::B ?
                    realStatus &= ~( EmuFramework::B >> 4 ) : realStatus |= EmuFramework::B >> 4 ;
                EmuFramework::inputStatus & EmuFramework::START ?
                    realStatus &= ~( EmuFramework::START >> 4 ) : realStatus |= EmuFramework::START >> 4 ;
                EmuFramework::inputStatus & EmuFramework::SEL ?
                    realStatus &= ~( EmuFramework::SEL >> 4 ) : realStatus |= EmuFramework::SEL >> 4 ;
                break ;
        }

        return realStatus ;
    } // else if
    else if ( addr >= 0xff10 && addr <= 0xff3f )
        return apu.read_register( cpu_time, addr );
    else
        return mainMemory[ addr ] ;
}

void MBC::DMA_Write(uint8_t addr) {
    uint16_t srcAddr = addr << 8 ;
/*
    for ( int i = 0 ; i < 0x8c ; ++i )
        WriteMemory( 0xFE00 + i, ReadMemory( srcAddr+ i ) ) ;
        */
    memcpy( mainMemory + 0xfe00, mainMemory + srcAddr, 0xa0 ) ;
}

void MBC::Debug_ReadMemDump( char* dumpFilePath ) {
    std::fstream dumpFile( dumpFilePath, std::fstream::in | std::fstream::binary ) ;
    if ( dumpFile.is_open() ) {
        char buf = 0 ;
        for ( int i = 0 ; i < 0x10000 ; ++i ) {
            buf = dumpFile.get() ;
            mainMemory[ i ] = buf ;
        } // for
        // dumpFile.read(reinterpret_cast<char*>(mainMemory), 0x10000 ) ;
        std::cout << "Read mem dump file:" << dumpFilePath << " done." << std::endl ;
    } // if
    else
        std::cout << "Read dump file failed." << std::endl ;
}

void MBC::Debug_ReadBIOS( char* dumpFilePath ) {
    std::fstream BIOSFile( dumpFilePath, std::fstream::in | std::fstream::binary ) ;
    if ( BIOSFile.is_open() ) {
        char buf = 0 ;
        BIOSFile.read(reinterpret_cast<char*>(mainMemory + 0x100), 0x256 ) ;
        std::cout << "Read bios file:" << dumpFilePath << " done." << std::endl ;
    } // if
    else
        std::cout << "Read bios failed." << std::endl ;
}

uint8_t *MBC::getMainMemory() const {
    // printf( "%x\n", mainMemory ) ;
    return mainMemory;
}

uint8_t MBC::getCurrentRomBank() const {
    return currentRomBank;
}
