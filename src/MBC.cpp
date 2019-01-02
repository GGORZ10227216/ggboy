//
// Created by ORZGG on 2018/12/30.
//

#include "MBC.h"
#include <string.h>

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