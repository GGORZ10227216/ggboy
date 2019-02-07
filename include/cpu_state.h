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
    uint8_t regs[ 8 ] ;
    uint16_t SP, PC ;
    uint32_t deltaCycle = 0 ;

    bool IME, pause = false, pc_jumping = false ;

    CPU_status() ;
    CPU_status( const LR35902* srcCpu ) ;

    inline uint16_t Get_AF() { return *( (uint16_t*)( regs + REGNAME::F ) ) ; }
    inline uint16_t Get_BC() { return *( (uint16_t*)( regs + REGNAME::C ) ) ; }
    inline uint16_t Get_DE() { return *( (uint16_t*)( regs + REGNAME::E ) ) ; }
    inline uint16_t Get_HL() { return *( (uint16_t*)( regs + REGNAME::L ) ) ; }
    inline void Set_BC( uint16_t val ) { *( (uint16_t*)( regs + REGNAME::C ) ) = val ; }
    inline void Set_DE( uint16_t val ) { *( (uint16_t*)( regs + REGNAME::E ) ) = val ; }
    inline void Set_HL( uint16_t val ) { *( (uint16_t*)( regs + REGNAME::L ) ) = val ; }
    inline void Set_SP( uint16_t val ) { SP = val ; }
    inline bool Get_ZeroFlag() { return 0b10000000 & regs[ F ] ; }
    inline bool Get_SubFlag()  { return 0b01000000 & regs[ F ] ; }
    inline bool Get_HCFlag()   { return 0b00100000 & regs[ F ] ; }
    inline bool Get_CFlag()    { return 0b00010000 & regs[ F ] ; }
    inline void Set_ZeroFlag() { regs[ F ] |= 0b10000000 ; }
    inline void Set_SubFlag()  { regs[ F ] |= 0b01000000 ; }
    inline void Set_HCFlag()   { regs[ F ] |= 0b00100000 ; }
    inline void Set_CFlag()    { regs[ F ] |= 0b00010000 ; }
    inline void Clear_ZeroFlag() { regs[ F ] &= 0b01111111 ; }
    inline void Clear_SubFlag()  { regs[ F ] &= 0b10111111 ; }
    inline void Clear_HCFlag()   { regs[ F ] &= 0b11011111 ; }
    inline void Clear_CFlag()    { regs[ F ] &= 0b11101111 ; }
    
};

#endif //CPU_CPU_STATE_H
