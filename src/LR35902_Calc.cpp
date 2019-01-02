#include "LR35902_Calc.h"

#include <cpu.h>
#include <iostream>

void lr35902::DAA() {
    if ( CPU_obj->Get_SubFlag() ) {
        if ( CPU_obj->Get_CFlag() )
            CPU_regs[ A ] += 0x60 ;
        if ( CPU_obj->Get_HCFlag() )
            CPU_regs[ A ] += 0x06 ;
    } // if
    else {
        if ( CPU_obj->Get_CFlag() || CPU_regs[ A ] > 0x99 ) {
            CPU_regs[ A ] += 0x60 ;
            CPU_obj->Set_CFlag() ;
        } // if
        if ( CPU_obj->Get_HCFlag() || ( CPU_regs[ A ] & 0x0f ) > 0x09 )
            CPU_regs[ A ] += 0x06 ;
    }

    if ( CPU_regs[ A ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    CPU_obj->Clear_HCFlag() ;
}

void lr35902::SCF() {
    CPU_obj->Set_CFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
}

void lr35902::CPL() {
    CPU_regs[ A ] = ~CPU_regs[ A ] ;
    CPU_obj->Set_HCFlag() ;
    CPU_obj->Set_SubFlag() ;
}

void lr35902::CCF() {
    CPU_obj->Get_CFlag() ? CPU_obj->Clear_CFlag() : CPU_obj->Set_CFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
}

/* 8bit INC reg */

void lr35902::INC_B() {
    if ( CPU_regs[ B ] >= 0b00001111 )
        CPU_obj->Set_HCFlag(); 
    else
        CPU_obj->Clear_HCFlag();

    ++ CPU_regs[ B ] ;

    if ( CPU_regs[ B ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag();

    CPU_obj->Clear_SubFlag();
}

void lr35902::INC_C() {
    if ( CPU_regs[ C ] >= 0b00001111 )
        CPU_obj->Set_HCFlag();
    else
        CPU_obj->Clear_HCFlag();

    ++ CPU_regs[ C ] ;

    if ( CPU_regs[ C ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag();

    CPU_obj->Clear_SubFlag();
}

void lr35902::INC_D() {
    if ( CPU_regs[ D ] >= 0b00001111 )
        CPU_obj->Set_HCFlag();
    else
        CPU_obj->Clear_HCFlag();

    ++ CPU_regs[ D ] ;

    if ( CPU_regs[ D ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag();

    CPU_obj->Clear_SubFlag();
}

void lr35902::INC_E() {
    if ( CPU_regs[ E ] >= 0b00001111 )
        CPU_obj->Set_HCFlag();
    else
        CPU_obj->Clear_HCFlag();

    ++ CPU_regs[ E ] ;

    if ( CPU_regs[ E ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag();

    CPU_obj->Clear_SubFlag();
}

void lr35902::INC_H() {
    if ( CPU_regs[ H ] >= 0b00001111 )
        CPU_obj->Set_HCFlag();
    else
        CPU_obj->Clear_HCFlag();

    ++ CPU_regs[ H ] ;

    if ( CPU_regs[ H ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag();

    CPU_obj->Clear_SubFlag();
}

void lr35902::INC_L() {
    if ( CPU_regs[ L ] >= 0b00001111 )
        CPU_obj->Set_HCFlag();
    else
        CPU_obj->Clear_HCFlag();

    ++ CPU_regs[ L ] ;    
        
    if ( CPU_regs[ L ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag();

    CPU_obj->Clear_SubFlag();
}

void lr35902::INC_refHL() {
    if ( CPU_obj->memory[ CPU_obj->Get_HL() ] >= 0b00001111 )
        CPU_obj->Set_HCFlag();
    else
        CPU_obj->Clear_HCFlag();

    ++ CPU_obj->memory[ CPU_obj->Get_HL() ] ;

    if ( CPU_obj->memory[ CPU_obj->Get_HL() ] == 0 )
        CPU_obj->Set_ZeroFlag();
    else
        CPU_obj->Clear_ZeroFlag();

    CPU_obj->Clear_SubFlag();
}

void lr35902::INC_A() {
    if ( CPU_regs[ A ] >= 0b00001111 )
        CPU_obj->Set_HCFlag();
    else
        CPU_obj->Clear_HCFlag();

    ++ CPU_regs[ A ] ;

    if ( CPU_regs[ A ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag();

    CPU_obj->Clear_SubFlag();
}

/* 16bit INC reg */

void lr35902::INC_BC() {
    CPU_obj->Set_BC(static_cast<uint16_t>(CPU_obj->Get_BC() + 1)) ;
}

void lr35902::INC_DE() {
    CPU_obj->Set_DE(static_cast<uint16_t>(CPU_obj->Get_DE() + 1)) ;
}

void lr35902::INC_HL() {
    CPU_obj->Set_HL(static_cast<uint16_t>(CPU_obj->Get_HL() + 1)) ;
}

void lr35902::INC_SP() {
    CPU_obj->Set_SP(static_cast<uint16_t>(CPU_SP + 1)) ;
}

/* 8bit DEC reg */

void lr35902::DEC_B() {
    if ( CPU_regs[ B ] == 0b00010000 || CPU_regs[ B ] == 0x0 )
        CPU_obj->Set_HCFlag();
    else
        CPU_obj->Clear_HCFlag();

    -- CPU_regs[ B ] ;

    if ( CPU_regs[ B ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag();

    CPU_obj->Set_SubFlag();
}

void lr35902::DEC_C() {
    if ( CPU_regs[ C ] == 0b00010000 || CPU_regs[ C ] == 0x0 )
        CPU_obj->Set_HCFlag();
    else
        CPU_obj->Clear_HCFlag();

    -- CPU_regs[ C ] ;

    if ( CPU_regs[ C ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag();

    CPU_obj->Set_SubFlag();
}

void lr35902::DEC_D() {
    if ( CPU_regs[ D ] == 0b00010000 || CPU_regs[ D ] == 0x0 )
        CPU_obj->Set_HCFlag();
    else
        CPU_obj->Clear_HCFlag();

    -- CPU_regs[ D ] ;

    if ( CPU_regs[ D ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag();

    CPU_obj->Set_SubFlag();
}

void lr35902::DEC_E() {
    if ( CPU_regs[ E ] == 0b00010000 || CPU_regs[ E ] == 0x0 )
        CPU_obj->Set_HCFlag();
    else
        CPU_obj->Clear_HCFlag();

    -- CPU_regs[ E ] ;

    if ( CPU_regs[ E ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag();

    CPU_obj->Set_SubFlag();
}

void lr35902::DEC_H() {
    if ( CPU_regs[ H ] == 0b00010000 || CPU_regs[ H ] == 0x0 )
        CPU_obj->Set_HCFlag();
    else
        CPU_obj->Clear_HCFlag();

    -- CPU_regs[ H ] ;

    if ( CPU_regs[ H ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag();

    CPU_obj->Set_SubFlag();
}

void lr35902::DEC_L() {
    if ( CPU_regs[ L ] == 0b00010000 || CPU_regs[ L ] == 0x0 )
        CPU_obj->Set_HCFlag();
    else
        CPU_obj->Clear_HCFlag();

    -- CPU_regs[ L ] ;

    if ( CPU_regs[ L ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag();

    CPU_obj->Set_SubFlag();
}

void lr35902::DEC_refHL() {
    if ( CPU_obj->memory[ CPU_obj->Get_HL() ] == 0b00010000 || CPU_obj->memory[ CPU_obj->Get_HL() ] == 0x0 )
        CPU_obj->Set_HCFlag();
    else
        CPU_obj->Clear_HCFlag();

    -- CPU_obj->memory[ CPU_obj->Get_HL() ] ;

    if ( CPU_obj->memory[ CPU_obj->Get_HL() ] == 0 )
        CPU_obj->Set_ZeroFlag();
    else
        CPU_obj->Clear_ZeroFlag();

    CPU_obj->Set_SubFlag();
}

void lr35902::DEC_A() {
    if ( CPU_regs[ A ] == 0b00010000 || CPU_regs[ A ] == 0x0 )
        CPU_obj->Set_HCFlag();
    else
        CPU_obj->Clear_HCFlag();

    -- CPU_regs[ A ] ;

    if ( CPU_regs[ A ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag();

    CPU_obj->Set_SubFlag();
}

/* 16bit DEC reg */

void lr35902::DEC_BC() {
    CPU_obj->Set_BC(static_cast<uint16_t>(CPU_obj->Get_BC() - 1)) ;
}

void lr35902::DEC_DE() {
    CPU_obj->Set_DE(static_cast<uint16_t>(CPU_obj->Get_DE() - 1)) ;
}

void lr35902::DEC_HL() {
    CPU_obj->Set_HL(static_cast<uint16_t>(CPU_obj->Get_HL() - 1)) ;
}

void lr35902::DEC_SP() {
    CPU_obj->Set_SP(static_cast<uint16_t>(CPU_SP - 1)) ;
}

/* 8bit ADD reg */

void lr35902::ADD_A_B() {
    uint8_t tempReg = CPU_regs[ A ] ;
    if ( ( (tempReg & 0x0f) + (CPU_regs[ B ] & 0x0f) & 0x10 ) == 0x10 )
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_regs[ A ] += CPU_regs[ B ] ;

    if ( CPU_regs[ A ] < tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;

    if ( CPU_regs[ A ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Clear_SubFlag();
}

void lr35902::ADD_A_C() {
    uint8_t tempReg = CPU_regs[ A ] ;
    if ( ( (tempReg & 0x0f) + (CPU_regs[ C ] & 0x0f) & 0x10 ) == 0x10 )
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_regs[ A ] += CPU_regs[ C ] ;

    if ( CPU_regs[ A ] < tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;

    if ( CPU_regs[ A ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag() ;
    CPU_obj->Clear_SubFlag();
}

void lr35902::ADD_A_D() {
    uint8_t tempReg = CPU_regs[ A ] ;
    if ( ( (tempReg & 0x0f) + (CPU_regs[ D ] & 0x0f) & 0x10 ) == 0x10 )
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_regs[ A ] += CPU_regs[ D ] ;

    if ( CPU_regs[ A ] < tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;

    if ( CPU_regs[ A ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag() ;
    CPU_obj->Clear_SubFlag();
}

void lr35902::ADD_A_E() {
    uint8_t tempReg = CPU_regs[ A ] ;
    if ( ((tempReg & 0x0f) + (CPU_regs[ E ] & 0x0f) & 0x10) == 0x10 )
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_regs[ A ] += CPU_regs[ E ] ;

    if ( CPU_regs[ A ] < tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;

    if ( CPU_regs[ A ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag() ;
    CPU_obj->Clear_SubFlag();
}

void lr35902::ADD_A_H() {
    uint8_t tempReg = CPU_regs[ A ] ;
    if ( ( (tempReg & 0x0f) + (CPU_regs[ H ] & 0x0f) & 0x10 ) == 0x10 )
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_regs[ A ] += CPU_regs[ H ] ;

    if ( CPU_regs[ A ] < tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;

    if ( CPU_regs[ A ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag() ;
    CPU_obj->Clear_SubFlag();
}

void lr35902::ADD_A_L() {
    uint8_t tempReg = CPU_regs[ A ] ;
    if ( ((tempReg & 0x0f) + (CPU_regs[ L ] & 0x0f) & 0x10) == 0x10 )
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_regs[ A ] += CPU_regs[ L ] ;

    if ( CPU_regs[ A ] < tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;

    if ( CPU_regs[ A ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag() ;
    CPU_obj->Clear_SubFlag();
}

void lr35902::ADD_A_refHL() {
    uint8_t tempReg = CPU_regs[ A ] ;
    if ( ( (tempReg & 0x0f) + (CPU_obj->memory[ CPU_obj->Get_HL() ] & 0x0f) & 0x10 ) == 0x10 )
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_regs[ A ] += CPU_obj->memory[ CPU_obj->Get_HL() ] ;

    if ( CPU_regs[ A ] < tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;

    if ( CPU_regs[ A ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Clear_SubFlag();
}

void lr35902::ADD_A_A() {
    uint8_t tempReg = CPU_regs[ A ] ;
    if ( ( (tempReg & 0x0f) + (tempReg & 0x0f) & 0x10 ) == 0x10 )
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_regs[ A ] += CPU_regs[ A ] ;

    if ( CPU_regs[ A ] < tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;

    if ( CPU_regs[ A ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Clear_SubFlag();
}

void lr35902::ADD_A_imm8() {
    uint8_t tempReg = CPU_regs[ A ] ;
    if ( ( (tempReg & 0x0f) + (CPU_obj->memory[ CPU_PC + 1 ] & 0x0f) & 0x10 ) == 0x10 )
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_regs[ A ] += CPU_obj->memory[ ++(CPU_PC) ] ;

    if ( CPU_regs[ A ] < tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;

    if ( CPU_regs[ A ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Clear_SubFlag();
}

/* 16bit ADD regPair */

void lr35902::ADD_HL_BC() {
  uint16_t tempReg = CPU_obj->Get_HL() ;
  if ( ( (tempReg & 0x0fff) + (CPU_obj->Get_BC() & 0x0fff) & 0x1000 ) == 0x1000 )
      CPU_obj->Set_HCFlag() ;
  else
      CPU_obj->Clear_HCFlag() ;

  CPU_obj->Set_HL( CPU_obj->Get_HL() + CPU_obj->Get_BC() ) ;

  if ( CPU_obj->Get_HL() < tempReg )
      CPU_obj->Set_CFlag() ;
  else
      CPU_obj->Clear_CFlag() ;
  /*
  if ( CPU_obj->Get_HL() == 0 )
      CPU_obj->Set_ZeroFlag() ;
  */
  CPU_obj->Clear_SubFlag();
}

void lr35902::ADD_HL_DE() {
    uint16_t tempReg = CPU_obj->Get_HL() ;
    if ( ( (tempReg & 0x0fff) + (CPU_obj->Get_DE() & 0x0fff) & 0x1000 ) == 0x1000 )
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_obj->Set_HL( CPU_obj->Get_HL() + CPU_obj->Get_DE() ) ;

    if ( CPU_obj->Get_HL() < tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;
    /*
    if ( CPU_obj->Get_HL() == 0 )
        CPU_obj->Set_ZeroFlag() ;
    */
    CPU_obj->Clear_SubFlag();
}

void lr35902::ADD_HL_HL() {
    uint16_t tempReg = CPU_obj->Get_HL() ;
    if ( ( (tempReg & 0x0fff) + (tempReg & 0x0fff) & 0x1000 ) == 0x1000 )
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_obj->Set_HL( CPU_obj->Get_HL() + CPU_obj->Get_HL() ) ;

    if ( CPU_obj->Get_HL() < tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;
    /*
    if ( CPU_obj->Get_HL() == 0 )
        CPU_obj->Set_ZeroFlag() ;
    */
    CPU_obj->Clear_SubFlag();
}

void lr35902::ADD_HL_SP() {
    uint16_t tempReg = CPU_obj->Get_HL() ;
    if ( ( (tempReg & 0x0fff) + (CPU_SP & 0x0fff) & 0x1000 ) == 0x1000 )
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_obj->Set_HL( CPU_obj->Get_HL() + CPU_SP ) ;

    if ( CPU_obj->Get_HL() < tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;
    /*
    if ( CPU_obj->Get_HL() == 0 )
        CPU_obj->Set_ZeroFlag() ;
    */
    CPU_obj->Clear_SubFlag();
}

void lr35902::ADD_SP_imm8() {
    uint16_t tempReg = CPU_SP ;
    if ( ( (tempReg & 0x0fff) + static_cast<uint16_t>( CPU_obj->memory[ CPU_PC + 1 ] ) & 0x1000 ) == 0x1000 )
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_SP += CPU_obj->memory[ ++CPU_PC ] ;
    // std::cout << (int)CPU_obj->memory[ ++CPU_PC ] << std::endl ;

    if ( CPU_SP < tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;

    CPU_obj->Clear_ZeroFlag() ;
    CPU_obj->Clear_SubFlag();
}

void lr35902::ADC_A_B() {
    uint8_t tempReg = CPU_regs[ A ] ;
    if (((tempReg & 0x0f) + (CPU_regs[ B ] & 0x0f) + CPU_obj->Get_CFlag() & 0x10 ) == 0x10)
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_regs[ A ] = CPU_regs[ A ] + CPU_regs[ B ] + CPU_obj->Get_CFlag() ;

    if ( CPU_regs[ A ] <= tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;

    if ( CPU_regs[ A ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Clear_SubFlag();
}

void lr35902::ADC_A_C() {
    uint8_t tempReg = CPU_regs[ A ] ;
    if ( ( (tempReg & 0x0f) + (CPU_regs[ C ] & 0x0f) + CPU_obj->Get_CFlag() & 0x10 ) == 0x10 )
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_regs[ A ] = CPU_regs[ A ] + CPU_regs[ C ] + CPU_obj->Get_CFlag() ;

    if ( CPU_regs[ A ] <= tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;

    if ( CPU_regs[ A ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Clear_SubFlag();
}

void lr35902::ADC_A_D() {
    uint8_t tempReg = CPU_regs[ A ] ;
    if ( ( (tempReg & 0x0f) + (CPU_regs[ D ] & 0x0f) + CPU_obj->Get_CFlag() & 0x10 ) == 0x10 )
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_regs[ A ] = CPU_regs[ A ] + CPU_regs[ D ] + CPU_obj->Get_CFlag() ;

    if ( CPU_regs[ A ] <= tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;

    if ( CPU_regs[ A ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Clear_SubFlag();
}

void lr35902::ADC_A_E() {
    uint8_t tempReg = CPU_regs[ A ] ;
    if ( ( (tempReg & 0x0f) + (CPU_regs[ E ] & 0x0f) + CPU_obj->Get_CFlag() & 0x10 ) == 0x10 )
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_regs[ A ] = CPU_regs[ A ] + CPU_regs[ E ] + CPU_obj->Get_CFlag() ;

    if ( CPU_regs[ A ] <= tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;

    if ( CPU_regs[ A ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Clear_SubFlag();
}

void lr35902::ADC_A_H() {
    uint8_t tempReg = CPU_regs[ A ] ;
    if ( ( (tempReg & 0x0f) + (CPU_regs[ H ] & 0x0f) + CPU_obj->Get_CFlag() & 0x10 ) == 0x10 )
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_regs[ A ] = CPU_regs[ A ] + CPU_regs[ H ] + CPU_obj->Get_CFlag() ;

    if ( CPU_regs[ A ] <= tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;

    if ( CPU_regs[ A ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Clear_SubFlag();
}

void lr35902::ADC_A_L() {
    uint8_t tempReg = CPU_regs[ A ] ;
    if ( ( (tempReg & 0x0f) + (CPU_regs[ L ] & 0x0f) + CPU_obj->Get_CFlag() & 0x10 ) == 0x10 )
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_regs[ A ] = CPU_regs[ A ] + CPU_regs[ L ] + CPU_obj->Get_CFlag() ;

    if ( CPU_regs[ A ] <= tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;

    if ( CPU_regs[ A ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Clear_SubFlag();
}

void lr35902::ADC_A_refHL() {
    uint8_t tempReg = CPU_regs[ A ] ;
    if ( ( (tempReg & 0x0f) + (CPU_obj->memory[ CPU_obj->Get_HL() ] & 0x0f) + CPU_obj->Get_CFlag() & 0x10 ) == 0x10 )
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_regs[ A ] = CPU_regs[ A ] + CPU_obj->memory[ CPU_obj->Get_HL() ] + CPU_obj->Get_CFlag() ;

    if ( CPU_regs[ A ] <= tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;

    if ( CPU_regs[ A ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Clear_SubFlag();
}

void lr35902::ADC_A_A() {
    uint8_t tempReg = CPU_regs[ A ] ;
    if ( ( (tempReg & 0x0f) + (tempReg & 0x0f) + CPU_obj->Get_CFlag() & 0x10 ) == 0x10 )
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_regs[ A ] = CPU_regs[ A ] + CPU_regs[ A ] + CPU_obj->Get_CFlag() ;

    if ( CPU_regs[ A ] <= tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;

    if ( CPU_regs[ A ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Clear_SubFlag();
}

void lr35902::ADC_A_imm8() {
    uint8_t tempReg = CPU_regs[ A ] ;
    uint8_t immVal = CPU_obj->memory[ ++CPU_PC ] ;

    if ( ( (tempReg & 0x0f) + (immVal & 0x0f) + CPU_obj->Get_CFlag() & 0x10 ) == 0x10 )
        CPU_obj->Set_HCFlag() ;
    else
        CPU_obj->Clear_HCFlag() ;

    CPU_regs[ A ] = CPU_regs[ A ] + immVal + CPU_obj->Get_CFlag() ;

    if ( CPU_regs[ A ] <= tempReg )
        CPU_obj->Set_CFlag() ;
    else
        CPU_obj->Clear_CFlag() ;

    if ( CPU_regs[ A ] == 0 )
        CPU_obj->Set_ZeroFlag() ;
    else
        CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Clear_SubFlag();
}

void lr35902::SUB_B() {
    uint8_t tempReg = CPU_regs[ A ] ;

    CPU_regs[ A ] -= CPU_regs[ B ] ;

    (tempReg & 0x0f) < (CPU_regs[ B ] & 0x0f) ? CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
    CPU_regs[ A ] > tempReg ? CPU_obj->Set_CFlag() : CPU_obj->Clear_CFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Set_SubFlag() ;
}

void lr35902::SUB_C() {
    uint8_t tempReg = CPU_regs[ A ] ;

    CPU_regs[ A ] -= CPU_regs[ C ] ;

    (tempReg & 0x0f) < (CPU_regs[ C ] & 0x0f) ? CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
    CPU_regs[ A ] > tempReg ? CPU_obj->Set_CFlag() : CPU_obj->Clear_CFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Set_SubFlag() ;
}

void lr35902::SUB_D() {
    uint8_t tempReg = CPU_regs[ A ] ;

    CPU_regs[ A ] -= CPU_regs[ D ] ;

    (tempReg & 0x0f) < (CPU_regs[ D ] & 0x0f) ? CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
    CPU_regs[ A ] > tempReg ? CPU_obj->Set_CFlag() : CPU_obj->Clear_CFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Set_SubFlag() ;
}

void lr35902::SUB_E() {
    uint8_t tempReg = CPU_regs[ A ] ;

    CPU_regs[ A ] -= CPU_regs[ E ] ;

    (tempReg & 0x0f) < (CPU_regs[ E ] & 0x0f) ? CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
    CPU_regs[ A ] > tempReg ? CPU_obj->Set_CFlag() : CPU_obj->Clear_CFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Set_SubFlag() ;
}

void lr35902::SUB_H() {
    uint8_t tempReg = CPU_regs[ A ] ;

    CPU_regs[ A ] -= CPU_regs[ H ] ;

    (tempReg & 0x0f) < (CPU_regs[ H ] & 0x0f) ? CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
    CPU_regs[ A ] > tempReg ? CPU_obj->Set_CFlag() : CPU_obj->Clear_CFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Set_SubFlag() ;
}

void lr35902::SUB_L() {
    uint8_t tempReg = CPU_regs[ A ] ;

    CPU_regs[ A ] -= CPU_regs[ L ] ;

    (tempReg & 0x0f) < (CPU_regs[ L ] & 0x0f) ? CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
    CPU_regs[ A ] > tempReg ? CPU_obj->Set_CFlag() : CPU_obj->Clear_CFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Set_SubFlag() ;
}

void lr35902::SUB_refHL() {
    uint8_t tempReg = CPU_regs[ A ] ;

    CPU_regs[ A ] -= CPU_obj->memory[ CPU_obj->Get_HL() ] ;

    (tempReg & 0x0f) < (CPU_obj->memory[ CPU_obj->Get_HL() ] & 0x0f) ? CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
    CPU_regs[ A ] > tempReg ? CPU_obj->Set_CFlag() : CPU_obj->Clear_CFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Set_SubFlag() ;
}

void lr35902::SUB_A() {
    CPU_regs[ A ] = 0 ;
    CPU_obj->Set_ZeroFlag() ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Set_SubFlag() ;
}

void lr35902::SUB_imm8() {
    uint8_t tempReg = CPU_regs[ A ] ;
    uint8_t immVal = CPU_obj->memory[ ++CPU_PC ] ;

    CPU_regs[ A ] -= immVal ;

    (tempReg & 0x0f) < (CPU_obj->memory[ CPU_PC ] & 0x0f) ? CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
    CPU_regs[ A ] > tempReg ? CPU_obj->Set_CFlag() : CPU_obj->Clear_CFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Set_SubFlag() ;
}

void lr35902::SBC_A_B() {
    uint8_t tempReg = CPU_regs[ A ] ;

    CPU_regs[ A ] -= CPU_regs[ B ] + CPU_obj->Get_CFlag()  ;

    (tempReg & 0x0f) < (CPU_regs[ B ] & 0x0f) + CPU_obj->Get_CFlag() ? CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
    CPU_regs[ A ] > tempReg ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Set_SubFlag() ;
}

void lr35902::SBC_A_C() {
    uint8_t tempReg = CPU_regs[ A ] ;

    CPU_regs[ A ] -= CPU_regs[ C ] + CPU_obj->Get_CFlag()  ;

    (tempReg & 0x0f) < (CPU_regs[ C ] & 0x0f) + CPU_obj->Get_CFlag() ? CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
    CPU_regs[ A ] > tempReg ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Set_SubFlag() ;
}

void lr35902::SBC_A_D() {
    uint8_t tempReg = CPU_regs[ A ] ;

    CPU_regs[ A ] -= CPU_regs[ D ] + CPU_obj->Get_CFlag()  ;

    (tempReg & 0x0f) < (CPU_regs[ D ] & 0x0f) + CPU_obj->Get_CFlag() ? CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
    CPU_regs[ A ] > tempReg ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Set_SubFlag() ;
}

void lr35902::SBC_A_E() {
    uint8_t tempReg = CPU_regs[ A ] ;

    CPU_regs[ A ] -= CPU_regs[ E ] + CPU_obj->Get_CFlag()  ;

    (tempReg & 0x0f) < (CPU_regs[ E ] & 0x0f) + CPU_obj->Get_CFlag() ? CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
    CPU_regs[ A ] > tempReg ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Set_SubFlag() ;
}

void lr35902::SBC_A_H() {
    uint8_t tempReg = CPU_regs[ A ] ;

    CPU_regs[ A ] -= CPU_regs[ H ] + CPU_obj->Get_CFlag()  ;

    (tempReg & 0x0f) < (CPU_regs[ H ] & 0x0f) + CPU_obj->Get_CFlag() ? CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
    CPU_regs[ A ] > tempReg ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Set_SubFlag() ;
}

void lr35902::SBC_A_L() {
    uint8_t tempReg = CPU_regs[ A ] ;

    CPU_regs[ A ] -= CPU_regs[ L ] + CPU_obj->Get_CFlag()  ;

    (tempReg & 0x0f) < (CPU_regs[ L ] & 0x0f) + CPU_obj->Get_CFlag() ? CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
    CPU_regs[ A ] > tempReg ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Set_SubFlag() ;
}

void lr35902::SBC_A_refHL() {
    uint8_t tempReg = CPU_regs[ A ] ;

    CPU_regs[ A ] -= CPU_obj->memory[ CPU_obj->Get_HL() ] + CPU_obj->Get_CFlag()  ;

    (tempReg & 0x0f) < (CPU_obj->memory[ CPU_obj->Get_HL() ] & 0x0f) + CPU_obj->Get_CFlag() ?
        CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
    CPU_regs[ A ] > tempReg ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Set_SubFlag() ;
}

void lr35902::SBC_A_A() {
    if ( CPU_obj->Get_CFlag() ) {
        CPU_regs[ A ] = 0b11111111 ;
        CPU_obj->Set_HCFlag() ;
        CPU_obj->Set_CFlag() ;
        CPU_obj->Clear_ZeroFlag() ;
    } // if
    else {
        CPU_regs[ A ] = 0 ;
        CPU_obj->Clear_HCFlag() ;
        CPU_obj->Clear_CFlag() ;
        CPU_obj->Set_ZeroFlag() ;
    } // else

    CPU_obj->Set_SubFlag() ;
}

void lr35902::SBC_A_imm8() {
    uint8_t tempReg = CPU_regs[ A ] ;
    uint8_t immVal = CPU_regs[ ++CPU_PC ] ;

    CPU_regs[ A ] -= immVal + CPU_obj->Get_CFlag()  ;

    (tempReg & 0x0f) < (immVal & 0x0f) + CPU_obj->Get_CFlag() ?
    CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
    CPU_regs[ A ] > tempReg ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

    CPU_obj->Set_SubFlag() ;
}

void lr35902::AND_B() {
    CPU_regs[ A ] &= CPU_regs[ B ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Set_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::AND_C() {
    CPU_regs[ A ] &= CPU_regs[ C ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Set_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::AND_D() {
    CPU_regs[ A ] &= CPU_regs[ D ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Set_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::AND_E() {
    CPU_regs[ A ] &= CPU_regs[ E ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Set_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::AND_H() {
    CPU_regs[ A ] &= CPU_regs[ H ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Set_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::AND_L() {
    CPU_regs[ A ] &= CPU_regs[ L ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Set_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::AND_refHL() {
    CPU_regs[ A ] &= CPU_obj->memory[ CPU_obj->Get_HL() ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Set_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::AND_A() {
    CPU_regs[ A ] &= CPU_regs[ A ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Set_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::AND_imm8() {
    CPU_regs[ A ] &= CPU_obj->memory[ ++CPU_PC ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Set_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::XOR_B() {
    CPU_regs[ A ] ^= CPU_regs[ B ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::XOR_C() {
    CPU_regs[ A ] ^= CPU_regs[ C ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::XOR_D() {
    CPU_regs[ A ] ^= CPU_regs[ D ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::XOR_E() {
    CPU_regs[ A ] ^= CPU_regs[ E ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::XOR_H() {
    CPU_regs[ A ] ^= CPU_regs[ H ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::XOR_L() {
    CPU_regs[ A ] ^= CPU_regs[ L ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::XOR_refHL() {
    CPU_regs[ A ] ^= CPU_obj->memory[ CPU_obj->Get_HL() ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::XOR_A() {
    CPU_regs[ A ] ^= CPU_regs[ A ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::XOR_imm8() {
    CPU_regs[ A ] ^= CPU_obj->memory[ ++CPU_PC ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::OR_B() {
    CPU_regs[ A ] |= CPU_regs[ B ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::OR_C() {
    CPU_regs[ A ] |= CPU_regs[ C ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::OR_D() {
    CPU_regs[ A ] |= CPU_regs[ D ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::OR_E() {
    CPU_regs[ A ] |= CPU_regs[ E ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::OR_H() {
    CPU_regs[ A ] |= CPU_regs[ H ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::OR_L() {
    CPU_regs[ A ] |= CPU_regs[ L ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::OR_refHL() {
    CPU_regs[ A ] |= CPU_obj->memory[ CPU_obj->Get_HL() ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::OR_A() {
    CPU_regs[ A ] |= CPU_regs[ A ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::OR_imm8() {
    CPU_regs[ A ] |= CPU_obj->memory[ ++CPU_PC ] ;
    CPU_obj->Clear_CFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
    CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
}

void lr35902::CP_B() {
    CPU_obj->Set_SubFlag() ;
    if ( CPU_regs[ A ] > CPU_regs[ B ] ) {
        CPU_obj->Clear_ZeroFlag() ;
        CPU_obj->Set_HCFlag() ;
        CPU_obj->Clear_CFlag() ;
    } // if
    else if ( CPU_regs[ A ] == CPU_regs[ B ] ) {
        CPU_obj->Set_ZeroFlag() ;
        CPU_obj->Clear_HCFlag() ;
        CPU_obj->Clear_CFlag() ;
    } // else if
    else {
        CPU_obj->Clear_ZeroFlag() ;
        CPU_obj->Clear_HCFlag() ;
        CPU_obj->Set_CFlag() ;
    } // else
}

void lr35902::CP_C() {
    CPU_obj->Set_SubFlag() ;
    if ( CPU_regs[ A ] > CPU_regs[ C ]  ) {
        CPU_obj->Clear_ZeroFlag() ;
        CPU_obj->Set_HCFlag() ;
        CPU_obj->Clear_CFlag() ;
    } // if
    else if ( CPU_regs[ A ] == CPU_regs[ C ] ) {
        CPU_obj->Set_ZeroFlag() ;
        CPU_obj->Clear_HCFlag() ;
        CPU_obj->Clear_CFlag() ;
    } // else if
    else {
        CPU_obj->Clear_ZeroFlag() ;
        CPU_obj->Clear_HCFlag() ;
        CPU_obj->Set_CFlag() ;
    } // else
}

void lr35902::CP_D() {
    CPU_obj->Set_SubFlag() ;
    if ( CPU_regs[ A ] > CPU_regs[ D ]  ) {
        CPU_obj->Clear_ZeroFlag() ;
        CPU_obj->Set_HCFlag() ;
        CPU_obj->Clear_CFlag() ;
    } // if
    else if ( CPU_regs[ A ] == CPU_regs[ D ] ) {
        CPU_obj->Set_ZeroFlag() ;
        CPU_obj->Clear_HCFlag() ;
        CPU_obj->Clear_CFlag() ;
    } // else if
    else {
        CPU_obj->Clear_ZeroFlag() ;
        CPU_obj->Clear_HCFlag() ;
        CPU_obj->Set_CFlag() ;
    } // else
}

void lr35902::CP_E() {
    CPU_obj->Set_SubFlag() ;
    if ( CPU_regs[ A ] > CPU_regs[ E ]  ) {
        CPU_obj->Clear_ZeroFlag() ;
        CPU_obj->Set_HCFlag() ;
        CPU_obj->Clear_CFlag() ;
    } // if
    else if ( CPU_regs[ A ] == CPU_regs[ E ] ) {
        CPU_obj->Set_ZeroFlag() ;
        CPU_obj->Clear_HCFlag() ;
        CPU_obj->Clear_CFlag() ;
    } // else if
    else {
        CPU_obj->Clear_ZeroFlag() ;
        CPU_obj->Clear_HCFlag() ;
        CPU_obj->Set_CFlag() ;
    } // else
}

void lr35902::CP_H() {
    CPU_obj->Set_SubFlag() ;
    if ( CPU_regs[ A ] > CPU_regs[ H ]  ) {
        CPU_obj->Clear_ZeroFlag() ;
        CPU_obj->Set_HCFlag() ;
        CPU_obj->Clear_CFlag() ;
    } // if
    else if ( CPU_regs[ A ] == CPU_regs[ H ] ) {
        CPU_obj->Set_ZeroFlag() ;
        CPU_obj->Clear_HCFlag() ;
        CPU_obj->Clear_CFlag() ;
    } // else if
    else {
        CPU_obj->Clear_ZeroFlag() ;
        CPU_obj->Clear_HCFlag() ;
        CPU_obj->Set_CFlag() ;
    } // else
}

void lr35902::CP_L() {
    CPU_obj->Set_SubFlag() ;
    if ( CPU_regs[ A ] > CPU_regs[ L ]  ) {
        CPU_obj->Clear_ZeroFlag() ;
        CPU_obj->Set_HCFlag() ;
        CPU_obj->Clear_CFlag() ;
    } // if
    else if ( CPU_regs[ A ] == CPU_regs[ L ] ) {
        CPU_obj->Set_ZeroFlag() ;
        CPU_obj->Clear_HCFlag() ;
        CPU_obj->Clear_CFlag() ;
    } // else if
    else {
        CPU_obj->Clear_ZeroFlag() ;
        CPU_obj->Clear_HCFlag() ;
        CPU_obj->Set_CFlag() ;
    } // else
}

void lr35902::CP_refHL() {
    CPU_obj->Set_SubFlag() ;
    if ( CPU_regs[ A ] > CPU_obj->memory[ CPU_obj->Get_HL() ]  ) {
        CPU_obj->Clear_ZeroFlag() ;
        CPU_obj->Set_HCFlag() ;
        CPU_obj->Clear_CFlag() ;
    } // if
    else if ( CPU_regs[ A ] == CPU_obj->memory[ CPU_obj->Get_HL() ] ) {
        CPU_obj->Set_ZeroFlag() ;
        CPU_obj->Clear_HCFlag() ;
        CPU_obj->Clear_CFlag() ;
    } // else if
    else {
        CPU_obj->Clear_ZeroFlag() ;
        CPU_obj->Clear_HCFlag() ;
        CPU_obj->Set_CFlag() ;
    } // else
}

void lr35902::CP_A() {
    CPU_obj->Set_SubFlag() ;
    CPU_obj->Set_ZeroFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_CFlag() ;
}

void lr35902::CP_imm8() {
    uint8_t immVal = CPU_obj->memory[ ++CPU_PC ] ;
    CPU_obj->Set_SubFlag() ;
    if ( CPU_regs[ A ] > immVal  ) {
        CPU_obj->Clear_ZeroFlag() ;
        CPU_obj->Set_HCFlag() ;
        CPU_obj->Clear_CFlag() ;
    } // if
    else if ( CPU_regs[ A ] == immVal ) {
        CPU_obj->Set_ZeroFlag() ;
        CPU_obj->Clear_HCFlag() ;
        CPU_obj->Clear_CFlag() ;
    } // else if
    else {
        CPU_obj->Clear_ZeroFlag() ;
        CPU_obj->Clear_HCFlag() ;
        CPU_obj->Set_CFlag() ;
    } // else
}

void lr35902::RLCA() {
    ( CPU_regs[ A ] & 0b10000000 ) == 0 ? CPU_obj->Clear_CFlag() : CPU_obj->Set_CFlag() ;
    CPU_regs[ A ] = CPU_regs[ A ] << 1 ;
    CPU_regs[ A ] = CPU_regs[ A ] | static_cast<uint8_t>( CPU_obj->Get_CFlag() ) ;

    CPU_obj->Clear_ZeroFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
}

void lr35902::RLA() {
    bool originalCarry = CPU_obj->Get_CFlag() ;
    ( CPU_regs[ A ] & 0b10000000 ) == 0 ? CPU_obj->Clear_CFlag() : CPU_obj->Set_CFlag() ;
    CPU_regs[ A ] = ( CPU_regs[ A ] << 1 ) | originalCarry ;

    CPU_obj->Clear_ZeroFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
}

void lr35902::RRCA() {
    ( CPU_regs[ A ] & 0b00000001 ) == 0 ? CPU_obj->Clear_CFlag() : CPU_obj->Set_CFlag() ;
    CPU_regs[ A ] = CPU_regs[ A ] >> 1 ;
    CPU_regs[ A ] = CPU_regs[ A ] | static_cast<uint8_t>( CPU_obj->Get_CFlag() ) << 7 ;

    CPU_obj->Clear_ZeroFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
}

void lr35902::RRA() {
    bool originalCarry = CPU_obj->Get_CFlag() ;
    ( CPU_regs[ A ] & 0b00000001 ) == 0  ? CPU_obj->Clear_CFlag() : CPU_obj->Set_CFlag() ;
    CPU_regs[ A ] = ( CPU_regs[ A ] >> 1 ) | ( originalCarry << 7 ) ;

    CPU_obj->Clear_ZeroFlag() ;
    CPU_obj->Clear_HCFlag() ;
    CPU_obj->Clear_SubFlag() ;
}