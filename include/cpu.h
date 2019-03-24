#ifndef CPU_H
#define CPU_H

#include <functional>

#include <stdint.h>
#include <cstdlib>
#include <cstdio>
#include <RegEnum.h>

#include <cpu_state.h>
#include <memory>
#include <MBC.h>

using namespace lr35902 ;

class LR35902 {
  public:
    LR35902( MBC& mmu ) ;
    virtual ~LR35902();

    FILE* cpu_debug;

    CPU_status currentStatus ;

    inline uint16_t Get_AF() {
        return currentStatus.Get_AF() ;
    }

    inline uint16_t Get_BC() {
        return currentStatus.Get_BC() ;
    }

    inline uint16_t Get_DE() {
        return currentStatus.Get_DE() ;
    }

    inline uint16_t Get_HL() {
        return currentStatus.Get_HL() ;
    }

    inline void Set_BC( uint16_t val ) {
        currentStatus.Set_BC( val ) ;
    }

    inline void Set_DE( uint16_t val ) {
        currentStatus.Set_DE( val ) ;
    }

    inline void Set_HL( uint16_t val ) {
        currentStatus.Set_HL( val ) ;
    }

    static inline bool TEST_BIT( uint8_t byte, uint8_t bitPos ) {
        return byte & ( 1 << bitPos ) ;
    }

    inline bool Get_ZeroFlag() const { return currentStatus.Get_ZeroFlag() ; }
    inline bool Get_SubFlag() const { return currentStatus.Get_SubFlag() ; }
    inline bool Get_HCFlag() const { return currentStatus.Get_HCFlag() ; }
    inline bool Get_CFlag() const { return currentStatus.Get_CFlag() ; }

    inline void Set_ZeroFlag() { currentStatus.Set_ZeroFlag() ; }
    inline void Set_SubFlag()  { currentStatus.Set_SubFlag() ; }
    inline void Set_HCFlag()   { currentStatus.Set_HCFlag() ; }
    inline void Set_CFlag()    { currentStatus.Set_CFlag() ; }
    inline void Clear_ZeroFlag() { currentStatus.Clear_ZeroFlag() ; }
    inline void Clear_SubFlag()  { currentStatus.Clear_SubFlag() ; }
    inline void Clear_HCFlag()   { currentStatus.Clear_HCFlag() ; }
    inline void Clear_CFlag()    { currentStatus.Clear_CFlag() ; }

    inline uint16_t Fetch_16bitByMMU( uint16_t addr ) {
        uint16_t value = 0 ;
        uint8_t* hilo = (uint8_t*) &value ;
        hilo[ LOW ] = _mmu.ReadMemory( addr ) ;
        hilo[ HIGH ] = _mmu.ReadMemory( addr + 1 )  ;
        return value ;
    } // Fetch_16bitByMMU()

    inline bool GetJumpCondition( uint8_t opcode ) {
        switch ( ( opcode & 0b00011000 ) ) {
            case 0x00 :
                return !Get_ZeroFlag() ;
            case 0x08 :
                return  Get_ZeroFlag() ;
            case 0x10 :
                return  !Get_CFlag() ;
            case 0x18 :
                return  Get_CFlag() ;
        } // switch
    } //


    inline uint8_t MEMREAD(uint16_t addr) { return _mmu.ReadMemory( addr ) ; }
    inline void MEMWRITE(uint16_t addr, uint8_t data) { _mmu.WriteMemory( addr, data ) ; }

    void RunExtendInstruction( uint8_t opcode ) ;
    void ResumeFromState( const CPU_status cs ) ;
    void ExecuteCurrentInstruction() ;
    void CheckInterrupts() ;
    void DoInterrupt( uint8_t irtNum ) ;
    void LogCPU( uint8_t opcode ) ;
    bool halting = false ;
    static uint8_t  cop ;
    static uint16_t cpc ;
private:
    MBC& _mmu ;
    bool stackLogging = false ;
    int8_t IME_delay = 0 ;
    uint64_t instC = 0;
    static uint16_t DAATable[] ;

};

enum RegAddr{
    P1 = 0xFF00 , SB = 0xFF01, SC = 0xFF02, DIV = 0xFF04, TIMA = 0xFF05, TMA = 0xFF06, TAC = 0xFF07,
    IF = 0xFF0F, IE = 0xFFFF, LCDC = 0xFF40, STAT = 0xFF41, SCY = 0xFF42, SCX = 0xFF43, LY = 0xFF44,
    LYC = 0xFF45, DMA = 0xFF46, BGP = 0xFF47, OBP0 = 0xFF48, OBP1 = 0xFF49, WY = 0xFF4A, WX = 0xFF4B,
    OAM = 0xFE00
} ;

#endif // CPU_H
