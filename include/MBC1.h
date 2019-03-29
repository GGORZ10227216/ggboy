//
// Created by ORZGG on 2019/1/1.
//

#include <MBC.h>

#ifndef CPU_MBC1_H
#define CPU_MBC1_H

using namespace lr35902 ;

class MBC1 : public MBC {
public:
    MBC1(Cartridge* rom) ;
    uint8_t autoKey[ 9 ] = { 0x1f, 0x1f, 0x17, 0x1f, 0x1f, 0x2f, 0x2f, 0x1e, 0x1e } ;
    uint64_t inputIndex = 0 ;
    uint8_t ReadMemory( const uint16_t addr ) ;
    void WriteMemory( const uint16_t addr, const uint8_t value ) ;
};


#endif //CPU_MBC1_H
