//
// Created by ORZGG on 2019/1/1.
//

#include "MBC1.h"
#include <string.h>

MBC1::MBC1(Cartridge* rom) : MBC (rom) {
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

uint8_t MBC1::ReadMemory(const uint16_t addr) {
    if ( ( addr >= ROM_SWAP_BEG ) && ( addr < ROM_SWAP_END ) ) {
        uint16_t fixedAddr = addr - ROM_SWAP_BEG + romBankSize * currentRomBank ;
        return romBanks[ fixedAddr ] ;
    } // if
    else if ( ( addr >= EXT_RAM_BEG ) && ( addr <= EXT_RAM_END ) ) {
        uint16_t fixedAddr  = addr - EXT_RAM_BEG + currentRamBank * ramBankSize ;
        return ramBanks[ fixedAddr ] ;
    } // else if
    else
        return mainMemory[ addr ] ;
}

void MBC1::WriteMemory(const uint16_t addr, const uint8_t value) {
    if ( addr < 0x2000 ) {
        if ( ( value & 0x0F ) == 0x0A )
            exterRamEnabled = true ;
        else
            exterRamEnabled = false ;
    } // if
    else if ( addr >= 0x2000 && addr < 0x4000 ) {
        // Rom Bank number bit 0-4 (LoRomBank) change
        currentRomBank &= 0b11100000 ;
        currentRomBank |= value & 0b00011111 ;
        if ( ( value & 0b00011111 ) == 0 )
            ++ currentRomBank ;
    } // else if
    else if ( addr >= 0x4000 && addr < 0x6000 ) {
        // HiBankNum change or RamBank change
        if ( romBankingMode ) {
            // In ROM banking mode
            currentRomBank &= 0b10011111 ;
            currentRomBank |= ( value & 0b00000011 ) << 5 ;
        } // if
        else {
            // In RAM banking mode
            currentRamBank = static_cast<uint8_t>( value & 0b00000011 ) ;
        } // else
    } // else if
    else if ( addr >= 0x6000 && addr < 0x8000 ) {
        // Banking mode change (ROM or RAM banking)
        romBankingMode = ( value & 0b00000001 ) == 0 ;
    } // else if
    else if ( addr >= 0xA000 && addr < 0xC000 ) {
        // Writing to external RAM
        if ( exterRamEnabled ) {
            ramBanks[ currentRamBank * 0x2000 + ( addr - 0xA000 ) ] = value ;
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