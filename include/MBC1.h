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
    uint8_t autoKey[ 7 ] = { 0x1f, 0x1f, 0x17, 0x17, 0x17, 0x17, 0x1f } ;
    uint64_t inputIndex = 0 ;
    uint8_t ReadMemory( const uint16_t addr ) ;
    void WriteMemory( const uint16_t addr, const uint8_t value ) ;
};


#endif //CPU_MBC1_H
