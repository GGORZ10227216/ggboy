//
// Created by ORZGG on 2019/1/1.
//

#include "MBC2.h"
#include <string.h>

MBC2::MBC2(Cartridge* rom) : MBC (rom) {
    if ( ramBanks == nullptr )
        ramBanks = new uint8_t[ 0x8000 ]() ;
    else
        memset( ramBanks, 0, 0x8000 ) ;
    if ( ramBanks == nullptr )
        romBanks = new uint8_t[ 0x200000 ]() ;
    else
        memset( romBanks, 0, 0x200000 ) ;

    memcpy( romBanks, rom->getRawRomData() + 0x4000, rom->getRawRomSize() - 0x4000 ) ;
}

uint8_t MBC2::ReadMemory(const uint16_t addr) {
    if ( ( addr >= ROM_SWAP_BEG ) && ( addr < ROM_SWAP_END ) ) {
        uint16_t fixedAddr = addr - ROM_SWAP_BEG + romBankSize * currentRomBank ;
        return romBanks[ fixedAddr ] ;
    } // if
    else if ( ( addr >= 0xA000 ) && ( addr <= 0xA1FF ) ) {
        return ramBanks[ addr - 0xA000 ] ;
    } // else if
    else
        return mainMemory[ addr ] ;
}

void MBC2::WriteMemory(const uint16_t addr, const uint8_t value) {
    if ( addr < 0x2000 && ( ( addr & 0b0000000100000000 ) == 0 ) ) {
        if ( ( value & 0x0F ) == 0x0A )
            exterRamEnabled = true ;
        else
            exterRamEnabled = false ;
    } // if
    else if ( addr >= 0x2000 && addr < 0x4000 ) {
        // Rom Bank number bit 0-3 (LoRomBank) change
        if ( ( addr & 0b0000000100000000 ) != 0 ) {
            currentRomBank &= 0b11110000 ;
            currentRomBank |= value & 0b00001111 ;
            if ( currentRomBank == 0 )
                ++ currentRomBank ;
        } // if
    } // else if
    else if ( addr >= 0xA000 && addr < 0xA200 ) {
        // Writing to MBC2's internal RAM ( but it is external from gameboy :-P )
        if ( exterRamEnabled ) {
            ramBanks[ addr - 0xA000 ] = value ;
        } // if
    } // else if
    else if ( ( addr >= 0xE000 ) && (addr < 0xFE00) ) {
        // writing to ECHO ram also writes in Working RAM(c000~d000)
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