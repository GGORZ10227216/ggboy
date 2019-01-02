#ifndef CPU_H
#define CPU_H

#include <functional>

#include <stdint.h>
#include <cstdlib>
#include <cstdio>
#include <RegEnum.h>

#include <cpu_state.h>

class LR35902 {
  public:
    LR35902();
    virtual ~LR35902();

    uint8_t* memory = NULL ;
    CPU_status currentStatus ;

    static void (*unprefixed_opcode[ 256 ])(void)  ;

    inline uint16_t Get_AF() {
        return currentStatus.regs[ A ] << 8 | currentStatus.regs[ F ] ;
    }

    inline uint16_t Get_BC() {
        return currentStatus.regs[ B ] << 8 | currentStatus.regs[ C ] ;
    }

    inline uint16_t Get_DE() {
        return currentStatus.regs[ D ] << 8 | currentStatus.regs[ E ] ;
    }

    inline uint16_t Get_HL() {
        return currentStatus.regs[ H ] << 8 | currentStatus.regs[ L ] ;
    }

    inline void Set_BC( uint16_t val ) {
        currentStatus.regs[ B ] = reinterpret_cast<uint8_t*> ( &val )[ 1 ] ;
        currentStatus.regs[ C ] = reinterpret_cast<uint8_t*> ( &val )[ 0 ] ;
    }

    inline void Set_DE( uint16_t val ) {
        currentStatus.regs[ D ] = reinterpret_cast<uint8_t*> ( &val )[ 1 ] ;
        currentStatus.regs[ E ] = reinterpret_cast<uint8_t*> ( &val )[ 0 ] ;
    }

    inline void Set_HL( uint16_t val ) {
        currentStatus.regs[ H ] = reinterpret_cast<uint8_t*> ( &val )[ 1 ] ;
        currentStatus.regs[ L ] = reinterpret_cast<uint8_t*> ( &val )[ 0 ] ;
    }

    inline void Set_SP( uint16_t val ) { currentStatus.SP = val ; }

    inline bool Get_ZeroFlag() { return 0b10000000 & currentStatus.regs[ F ] ; }
    inline bool Get_SubFlag()  { return 0b01000000 & currentStatus.regs[ F ] ; }
    inline bool Get_HCFlag()   { return 0b00100000 & currentStatus.regs[ F ] ; }
    inline bool Get_CFlag()    { return 0b00010000 & currentStatus.regs[ F ] ; }

    inline void Set_ZeroFlag() { currentStatus.regs[ F ] |= 0b10000000 ; }
    inline void Set_SubFlag()  { currentStatus.regs[ F ] |= 0b01000000 ; }
    inline void Set_HCFlag()   { currentStatus.regs[ F ] |= 0b00100000 ; }
    inline void Set_CFlag()    { currentStatus.regs[ F ] |= 0b00010000 ; }
    inline void Clear_ZeroFlag() { currentStatus.regs[ F ] &= 0b01111111 ; }
    inline void Clear_SubFlag()  { currentStatus.regs[ F ] &= 0b10111111 ; }
    inline void Clear_HCFlag()   { currentStatus.regs[ F ] &= 0b11011111 ; }
    inline void Clear_CFlag()    { currentStatus.regs[ F ] &= 0b11101111 ; }

    bool InitMemory( int romSize, int ramSize ) ;
    void ResumeFromState( const CPU_status cs ) ;
    void ExecuteCurrentInstruction() ;
    const uint16_t IE = 0xFFFF ;
    const uint16_t IF = 0xFF0F ;
};

extern LR35902* CPU_obj ;

#define CPU_regs CPU_obj->currentStatus.regs
#define CPU_SP   CPU_obj->currentStatus.SP
#define CPU_PC   CPU_obj->currentStatus.PC

enum RegAddr{
    P1 = 0xFF00 , SB = 0xFF01, SC = 0xFF02, DIV = 0xFF04, TIMA = 0xFF05, TMA = 0xFF06, TAC = 0xFF07,
    IF = 0xFF0F, IE = 0xFFFF, LCDC = 0xFF40, STAT = 0xFF41, SCY = 0xFF42, SCX = 0xFF43, LY = 0xFF44,
    LYC = 0xFF45, DMA = 0xFF46, BGP = 0xFF47, OBP0 = 0xFF48, OBP1 = 0xFF49, WY = 0xFF4A, WX = 0xFF4B,
    OAM = 0xFE00
} ;

#endif // CPU_H
