//
// Created by ORZGG on 2018/12/30.
//

#include "MBC.h"
#include <string.h>
#include <iostream>
#include <fstream>

using namespace lr35902 ;

MBC::MBC(Cartridge* rom) {
    if ( mainMemory == nullptr )
        mainMemory = new uint8_t[ 0x10000 ] () ;
    else
        memset( mainMemory, 0, 0x10000 ) ;

    memcpy( mainMemory, rom->getRawRomData(), 0x4000 ) ; // copy the bank#00 of rom
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
    else if ( addr == 0xFF46 ) {
        DMA_Write( value ) ;
    } // else if
    else {
        // no control needed over this area so write to memory
        mainMemory[ addr ] = value ;
    } // else
}

uint8_t MBC::ReadMemory(const uint16_t addr) {
    bool accessLimited = ( ( addr >= ROM_SWAP_BEG ) && ( addr < ROM_SWAP_END ) ) ||
            ( ( addr >= EXT_RAM_BEG ) && ( addr <= EXT_RAM_END ) );
    if ( accessLimited ) {
        // Maybe we need to log this invalid memory require
        return 0;
    } // if
    else
        return mainMemory[ addr ] ;
}

void MBC::DMA_Write(uint8_t addr) {
    uint16_t srcAddr = addr << 8 ;

    for ( int i = 0 ; i < 0x8c ; ++i )
        WriteMemory( 0xFE00 + i, ReadMemory( srcAddr ) ) ;
}

void MBC::Debug_ReadMemDump( char* dumpFilePath ) {
    std::fstream dumpFile( dumpFilePath, std::fstream::in | std::fstream::binary ) ;
    if ( dumpFile.is_open() ) {
        char buf = 0 ;
        dumpFile.read(reinterpret_cast<char*>(mainMemory), 0x10000 ) ;
        std::cout << "Read mem dump file:" << dumpFilePath << " done." << std::endl ;
    } // if
    else
        std::cout << "Read dump file failed." << std::endl ;
}

uint8_t *MBC::getMainMemory() const {
    return mainMemory;
}
