//
// Created by orzgg on 12/13/18.
//

#ifndef CPU_CPU_STATE_H
#define CPU_CPU_STATE_H

#include <cstdint>
#include <cstring>
#include <RegEnum.h>

class LR35902 ;

struct CPU_status {
public :
    inline uint16_t Get_AF() const {
        return regs[ A ] << 8 | regs[ F ] ;
    }

    inline uint16_t Get_BC() const {
        return regs[ B ] << 8 | regs[ C ] ;
    }

    inline uint16_t Get_DE() const {
        return regs[ D ] << 8 | regs[ E ] ;
    }

    inline uint16_t Get_HL() const {
        return ( regs[ H ] << 8 ) | regs[ L ] ;
    }

    inline void Set_BC( uint16_t val ) {
        regs[ B ] = reinterpret_cast<uint8_t*> ( &val )[ 1 ] ;
        regs[ C ] = reinterpret_cast<uint8_t*> ( &val )[ 0 ] ;
    }

    inline void Set_DE( uint16_t val ) {
        regs[ D ] = reinterpret_cast<uint8_t*> ( &val )[ 1 ] ;
        regs[ E ] = reinterpret_cast<uint8_t*> ( &val )[ 0 ] ;
    }

    inline void Set_HL( uint16_t val ) {
        regs[ H ] = reinterpret_cast<uint8_t*> ( &val )[ 1 ] ;
        regs[ L ] = reinterpret_cast<uint8_t*> ( &val )[ 0 ] ;
    }

    inline bool Get_ZeroFlag() const { return 0b10000000 & regs[ F ] ; }
    inline bool Get_SubFlag() const { return 0b01000000 & regs[ F ] ; }
    inline bool Get_HCFlag() const  { return 0b00100000 & regs[ F ] ; }
    inline bool Get_CFlag() const   { return 0b00010000 & regs[ F ] ; }

    inline void Set_ZeroFlag() { regs[ F ] |= 0b10000000 ; }
    inline void Set_SubFlag()  { regs[ F ] |= 0b01000000 ; }
    inline void Set_HCFlag()   { regs[ F ] |= 0b00100000 ; }
    inline void Set_CFlag()    { regs[ F ] |= 0b00010000 ; }
    inline void Clear_ZeroFlag() { regs[ F ] &= 0b01111111 ; }
    inline void Clear_SubFlag()  { regs[ F ] &= 0b10111111 ; }
    inline void Clear_HCFlag()   { regs[ F ] &= 0b11011111 ; }
    inline void Clear_CFlag()    { regs[ F ] &= 0b11101111 ; }
    
    CPU_status() ;
    CPU_status( const LR35902* srcCpu ) ;

    uint8_t regs[ 8 ] ;
    uint16_t SP, PC ;
    uint32_t deltaCycle = 0 ;

    bool IME, pause = false, pc_jumping = false ;
};

#endif //CPU_CPU_STATE_H
