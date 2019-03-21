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
    uint64_t test = 0 ;
    uint8_t testA[ 3 ] = { 0xcf, 0xc7, 0xce } ;
    uint8_t ReadMemory( const uint16_t addr ) ;
    void WriteMemory( const uint16_t addr, const uint8_t value ) ;
};


#endif //CPU_MBC1_H
