//
// Created by ORZGG on 2018/12/30.
//

#include <stdint.h>
#include <Cartridge.h>

#ifndef CPU_MMU_H
#define CPU_MMU_H

namespace lr35902 {
    enum { LOW, HIGH } ;
    enum memoryAddrLabel {
        ROM_B0_BEG = 0x0,
        ROM_B0_END = 0x3FFF,
        ROM_SWAP_BEG = 0x4000,
        ROM_SWAP_END = 0x7FFF,
        VRAM_BEG = 0x8000,
        VRAM_END = 0x9FFF,
        EXT_RAM_BEG = 0xA000,
        EXT_RAM_END = 0xBFFF,
        WRAM_B0_BEG = 0xC000,
        WRAM_B0_END = 0xCFFF,
        WRAM_SWAP_BEG = 0xD000,
        WRAM_SWAP_END = 0xDFFF,
        WRAM_ECHO_BEG = 0xE000,
        WRAM_ECHO_END = 0xFDFF,
        OAM_BEG = 0xFE00,
        OAM_END = 0xFE9F,
        IO_BEG = 0xFF00,
        IO_END = 0xFF7F,
        HRAM_BEG = 0xFF80,
        HRAM_END = 0xFFFE,
    } ;

    class MBC {
    public :
        MBC( Cartridge* rom );
        virtual uint8_t ReadMemory( const uint16_t addr ) ;
        virtual void WriteMemory( const uint16_t addr, const uint8_t value ) ;

    protected:
        uint8_t* ramBanks ;
        uint8_t* romBanks ;
        uint8_t* mainMemory ;

        uint8_t currentRomBank = 1 ;
        uint8_t currentRamBank = 0 ;

        bool exterRamEnabled = false, romBankingMode = true ;

        static const uint16_t romBankSize = ROM_SWAP_END - ROM_SWAP_BEG + 1 ;
        static const uint16_t ramBankSize = EXT_RAM_END - EXT_RAM_BEG + 1 ;
    };

}

#endif //CPU_MMU_H
