//
// Created by ORZGG on 2019/1/1.
//

#include "MBC1.h"
#include <string.h>
#include <EmuFramework.h>

MBC1::MBC1(Cartridge* rom) : MBC (rom) {
    if ( ramBanks == nullptr )
        ramBanks = new uint8_t[ 0x8000 ]() ;
    else
        memset( ramBanks, 0, 0x8000 ) ;

    if ( romBanks == nullptr )
        romBanks = new uint8_t[ 0x200000 ]() ;
    else
        memset( romBanks, 0, 0x200000 ) ;

    if ( mainMemory == nullptr )
        mainMemory = new uint8_t[ 0x10000 ]() ;
    else
        memset( mainMemory, 0, 0x10000 ) ;

    memcpy( romBanks, rom->getRawRomData() + 0x4000, rom->getRawRomSize() - 0x4000 ) ;
    memcpy( mainMemory, rom->getRawRomData(), 0x4000 ) ;
}

uint8_t MBC1::ReadMemory(const uint16_t addr) {
    if ( ( addr >= ROM_SWAP_BEG ) && ( addr < ROM_SWAP_END ) ) {
        uint32_t fixedAddr = addr - ROM_SWAP_BEG + romBankSize * ( currentRomBank - 1 );
        return romBanks[ fixedAddr ] ;
    } // if
    else if ( ( addr >= EXT_RAM_BEG ) && ( addr <= EXT_RAM_END ) ) {
        uint32_t fixedAddr  = addr - EXT_RAM_BEG + currentRamBank * ramBankSize ;
        return ramBanks[ fixedAddr ] ;
    } // else if
    if ( addr == 0xff00 ) {
        uint8_t realStatus = 0x0F ;
        mainMemory[ 0xff00 ] &= 0b00110000 ;

        if ( mainMemory[ 0xFF00 ] == 0x20 ) {
            EmuFramework::inputStatus & EmuFramework::UP ?
                    realStatus &= ~EmuFramework::UP : realStatus |= EmuFramework::UP  ;
            EmuFramework::inputStatus & EmuFramework::DOWN ?
                    realStatus &= ~EmuFramework::DOWN : realStatus |= EmuFramework::DOWN;
            EmuFramework::inputStatus & EmuFramework::LEFT ?
                    realStatus &= ~EmuFramework::LEFT : realStatus |= EmuFramework::LEFT ;
            EmuFramework::inputStatus & EmuFramework::RIGHT ?
                    realStatus &= ~EmuFramework::RIGHT : realStatus |= EmuFramework::RIGHT ;
        } // if

        if ( mainMemory[ 0xFF00 ] == 0x10 ) {
            EmuFramework::inputStatus & EmuFramework::A ?
                    realStatus &= ~( EmuFramework::A >> 4 ) : realStatus |= EmuFramework::A >> 4 ;
            EmuFramework::inputStatus & EmuFramework::B ?
                    realStatus &= ~( EmuFramework::B >> 4 ) : realStatus |= EmuFramework::B >> 4 ;
            EmuFramework::inputStatus & EmuFramework::START ?
                    realStatus &= ~( EmuFramework::START >> 4 ) : realStatus |= EmuFramework::START >> 4 ;
            EmuFramework::inputStatus & EmuFramework::SEL ?
                    realStatus &= ~( EmuFramework::SEL >> 4 ) : realStatus |= EmuFramework::SEL >> 4 ;
        } // if

        //
        //printf( "padReg: %x inputState: %x realState: %x\n", mainMemory[ 0xff00 ], EmuFramework::inputStatus, realStatus ) ;
        return realStatus ;
    } // if
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
        currentRomBank = value & 0b00011111 ;
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
    else if ( addr == 0xFF46 ) {
        DMA_Write( value ) ;
    } // else if
    else {
        // no control needed over this area so write to memory
        //if ( addr == RegAddr::SCX )
        //    printf( "%x %x %x LY=%x\n", LR35902::cpc, LR35902::cop, value, mainMemory[ LY ] ) ;
        mainMemory[ addr ] = value ;
    } // else
}