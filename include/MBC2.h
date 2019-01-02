//
// Created by ORZGG on 2019/1/1.
//

#include <MBC.h>

#ifndef CPU_MBC2_H
#define CPU_MBC2_H

using namespace lr35902 ;

class MBC2 : public MBC {
public :
    MBC2(Cartridge* rom) ;
    uint8_t ReadMemory( const uint16_t addr ) ;
    void WriteMemory( const uint16_t addr, const uint8_t value ) ;
};


#endif //CPU_MBC2_H
