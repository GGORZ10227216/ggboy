#include "LR35902_LD.h"

#include <cpu.h>
#include <iostream>

/*----------------------------- lr35902::LD imm8 to X -----------------------------*/
void lr35902::LD_B_imm8() {
    CPU_regs[ B ] = CPU_obj->memory[ ++(CPU_PC) ] ; // pc-->imm8
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_B_imm8()

void lr35902::LD_C_imm8() {
    CPU_regs[ C ] = CPU_obj->memory[ ++(CPU_PC) ] ; // pc-->imm8
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_B_imm8()

void lr35902::LD_D_imm8() {
    CPU_regs[ D ] = CPU_obj->memory[ ++(CPU_PC) ] ; // pc-->imm8
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_B_imm8()

void lr35902::LD_E_imm8() {
    CPU_regs[ E ] = CPU_obj->memory[ ++(CPU_PC) ] ; // pc-->imm8
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_B_imm8()

void lr35902::LD_H_imm8() {
    CPU_regs[ H ] = CPU_obj->memory[ ++(CPU_PC) ] ; // pc-->imm8
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_B_imm8()

void lr35902::LD_L_imm8() {
    CPU_regs[ L ] = CPU_obj->memory[ ++(CPU_PC) ] ; // pc-->imm8
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_B_imm8()

void lr35902::LD_A_imm8() {
    CPU_regs[ A ] = CPU_obj->memory[ ++(CPU_PC) ] ; // pc-->imm8
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_B_imm8()

void lr35902::LD_refHL_imm8() {
    CPU_obj->memory[ CPU_obj->Get_HL() ] = CPU_obj->memory[ ++(CPU_PC) ] ; // pc-->imm8
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_B_imm8()

/*----------------------------- lr35902::LD X to B -----------------------------*/
void lr35902::LD_B_B() {
    /*Dst is as same as Src, just NOP*/
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_B_B()

void lr35902::LD_B_C() {
    CPU_regs[ B ] = CPU_regs[ C ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_B_C()

void lr35902::LD_B_D() {
    CPU_regs[ B ] = CPU_regs[ D ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_B_D()

void lr35902::LD_B_E() {
    CPU_regs[ B ] = CPU_regs[ E ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_B_C()

void lr35902::LD_B_H() {
    CPU_regs[ B ] = CPU_regs[ H ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_B_C()

void lr35902::LD_B_L() {
    CPU_regs[ B ] = CPU_regs[ L ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_B_C()

void lr35902::LD_B_refHL() {
    CPU_regs[ B ] = CPU_obj->memory[ CPU_obj->Get_HL() ] ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_B_C()

void lr35902::LD_B_A() {
    CPU_regs[ B ] = CPU_regs[ A ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_B_C()

/*----------------------------- lr35902::LD X to C -----------------------------*/
void lr35902::LD_C_B() {
    CPU_regs[ C ] = CPU_regs[ B ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_C_B()

void lr35902::LD_C_C() {
    /*Dst is as same as Src, just NOP*/
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_C_C()

void lr35902::LD_C_D() {
    CPU_regs[ C ] = CPU_regs[ D ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_C_D()

void lr35902::LD_C_E() {
    CPU_regs[ C ] = CPU_regs[ E ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_C_C()

void lr35902::LD_C_H() {
    CPU_regs[ C ] = CPU_regs[ H ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_C_C()

void lr35902::LD_C_L() {
    CPU_regs[ C ] = CPU_regs[ L ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_C_C()

void lr35902::LD_C_refHL() {
    CPU_regs[ C ] = CPU_obj->memory[ CPU_obj->Get_HL() ] ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_C_C()

void lr35902::LD_C_A() {
    CPU_regs[ C ] = CPU_regs[ A ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_C_C()

/*----------------------------- lr35902::LD X to D -----------------------------*/
void lr35902::LD_D_B() {
    CPU_regs[ D ] = CPU_regs[ B ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_D_B()

void lr35902::LD_D_C() {
    CPU_regs[ D ] = CPU_regs[ C ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_D_C()

void lr35902::LD_D_D() {
    /*NOP*/
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_D_D()

void lr35902::LD_D_E() {
    CPU_regs[ D ] = CPU_regs[ E ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_D_C()

void lr35902::LD_D_H() {
    CPU_regs[ D ] = CPU_regs[ H ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_D_C()

void lr35902::LD_D_L() {
    CPU_regs[ D ] = CPU_regs[ L ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_D_C()

void lr35902::LD_D_refHL() {
    CPU_regs[ D ] = CPU_obj->memory[ CPU_obj->Get_HL() ] ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_D_C()

void lr35902::LD_D_A() {
    CPU_regs[ D ] = CPU_regs[ A ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_D_C()

/*----------------------------- lr35902::LD X to E -----------------------------*/
void lr35902::LD_E_B() {
    CPU_regs[ E ] = CPU_regs[ B ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_D_B()

void lr35902::LD_E_C() {
    CPU_regs[ E ] = CPU_regs[ C ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_D_C()

void lr35902::LD_E_D() {
    CPU_regs[ E ] = CPU_regs[ D ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_D_D()

void lr35902::LD_E_E() {
    /*NOP*/
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_D_C()

void lr35902::LD_E_H() {
    CPU_regs[ E ] = CPU_regs[ H ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_D_C()

void lr35902::LD_E_L() {
    CPU_regs[ E ] = CPU_regs[ L ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_D_C()

void lr35902::LD_E_refHL() {
    CPU_regs[ E ] = CPU_obj->memory[ CPU_obj->Get_HL() ] ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_D_C()

void lr35902::LD_E_A() {
    CPU_regs[ E ] = CPU_regs[ A ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_D_C()

/*----------------------------- lr35902::LD X to H -----------------------------*/
void lr35902::LD_H_B() {
    CPU_regs[ H ] = CPU_regs[ B ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_H_B()

void lr35902::LD_H_C() {
    CPU_regs[ H ] = CPU_regs[ C ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_H_C()

void lr35902::LD_H_D() {
    CPU_regs[ H ] = CPU_regs[ D ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_H_D()

void lr35902::LD_H_E() {
    CPU_regs[ H ] = CPU_regs[ E ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_H_C()

void lr35902::LD_H_H() {
    /*NOP*/
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_H_C()

void lr35902::LD_H_L() {
    CPU_regs[ H ] = CPU_regs[ L ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_H_C()

void lr35902::LD_H_refHL() {
    CPU_regs[ H ] = CPU_obj->memory[ CPU_obj->Get_HL() ] ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_H_C()

void lr35902::LD_H_A() {
    CPU_regs[ H ] = CPU_regs[ A ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_H_C()

/*----------------------------- lr35902::LD X to L -----------------------------*/
void lr35902::LD_L_B() {
    CPU_regs[ L ] = CPU_regs[ B ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_D_B()

void lr35902::LD_L_C() {
    CPU_regs[ L ] = CPU_regs[ C ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_D_C()

void lr35902::LD_L_D() {
    CPU_regs[ L ] = CPU_regs[ D ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_D_D()

void lr35902::LD_L_E() {
    CPU_regs[ L ] = CPU_regs[ E ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_D_C()

void lr35902::LD_L_H() {
    CPU_regs[ L ] = CPU_regs[ H ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_D_C()

void lr35902::LD_L_L() {
    /*NOP*/
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_D_C()

void lr35902::LD_L_refHL() {
    CPU_regs[ L ] = CPU_obj->memory[ CPU_obj->Get_HL() ] ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_D_C()

void lr35902::LD_L_A() {
    CPU_regs[ L ] = CPU_regs[ A ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_D_C()

/*----------------------------- lr35902::LD X to ref(HL) -----------------------------*/
void lr35902::LD_refHL_B() {
    CPU_obj->memory[ CPU_obj->Get_HL() ] = CPU_regs[ B ] ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_HL_B()

void lr35902::LD_refHL_C() {
    CPU_obj->memory[ CPU_obj->Get_HL() ] = CPU_regs[ C ] ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_HL_C()

void lr35902::LD_refHL_D() {
    CPU_obj->memory[ CPU_obj->Get_HL() ] = CPU_regs[ D ] ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_HL_D()

void lr35902::LD_refHL_E() {
    CPU_obj->memory[ CPU_obj->Get_HL() ] = CPU_regs[ E ] ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_HL_E()

void lr35902::LD_refHL_H() {
    CPU_obj->memory[ CPU_obj->Get_HL() ] = CPU_regs[ H ] ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_HL_H()

void lr35902::LD_refHL_L() {
    CPU_obj->memory[ CPU_obj->Get_HL() ] = CPU_regs[ L ] ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_HL_L()

void lr35902::LD_refHL_A() {
    CPU_obj->memory[ CPU_obj->Get_HL() ] = CPU_regs[ A ] ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_HL_A()

void lr35902::LD_refHL_A_INC() {
    CPU_obj->memory[ CPU_obj->Get_HL() ] = CPU_regs[ A ] ;
    CPU_obj->Set_HL( CPU_obj->Get_HL() + 1 ) ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_HL_A_INC()

void lr35902::LD_refHL_A_DEC() {
    CPU_obj->memory[ CPU_obj->Get_HL() ] = CPU_regs[ A ] ;
    CPU_obj->Set_HL( CPU_obj->Get_HL() - 1 ) ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_HL_A_DEC()

/*----------------------------- lr35902::LD X to A -----------------------------*/
void lr35902::LD_A_B() {
    CPU_regs[ A ] = CPU_regs[ B ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_A_B()

void lr35902::LD_A_C() {
    CPU_regs[ A ] = CPU_regs[ C ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_A_C()

void lr35902::LD_A_D() {
    CPU_regs[ A ] = CPU_regs[ D ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_A_B()

void lr35902::LD_A_E() {
    CPU_regs[ A ] = CPU_regs[ E ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_A_E()

void lr35902::LD_A_H() {
    CPU_regs[ A ] = CPU_regs[ H ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_A_H()

void lr35902::LD_A_L()  {
    CPU_regs[ A ] = CPU_regs[ L ] ;
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_A_H()

void lr35902::LD_A_refHL()  {
    CPU_regs[ A ] = CPU_obj->memory[ CPU_obj->Get_HL() ] ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_A_H()

void lr35902::LD_A_refHL_INC() {
    CPU_regs[ A ] = CPU_obj->memory[ CPU_obj->Get_HL() ] ;
    CPU_obj->Set_HL(static_cast<uint16_t>(CPU_obj->Get_HL() + 1)) ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_A_refHL_INC()

void lr35902::LD_A_refHL_DEC() {
    CPU_regs[ A ] = CPU_obj->memory[ CPU_obj->Get_HL() ] ;
    CPU_obj->Set_HL(static_cast<uint16_t>(CPU_obj->Get_HL() - 1)) ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_A_refHL_DEC()

void lr35902::LD_A_A() {
    CPU_obj->currentStatus.deltaCycle += 4 ;
} // lr35902::LD_A_A()

void lr35902::LD_A_refBC() {
    CPU_regs[ A ] = CPU_obj->memory[ CPU_obj->Get_BC() ] ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_A_refBC()

void lr35902::LD_A_refDE() {
    CPU_regs[ A ] = CPU_obj->memory[ CPU_obj->Get_DE() ] ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_A_refDE()

void lr35902::LD_A_FFZZ_offset8() {
    CPU_regs[ A ] = CPU_obj->memory[ 0xFF00 + CPU_obj->memory[ ++( CPU_PC ) ] ] ;
    CPU_obj->currentStatus.deltaCycle += 12 ;
} // lr35902::LD_A_FFZZ_offset8()

void lr35902::LD_A_FFZZ_offset_regC() {
    CPU_regs[ A ] = CPU_obj->memory[ 0xFF00 + CPU_regs[ C ] ] ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_A_FFZZ_offset_regC()

void lr35902::LD_A_ref_offset16() {
    uint16_t* addr = (uint16_t*)( CPU_obj->memory + ++( CPU_PC ) ) ;
    CPU_regs[ A ] = CPU_obj->memory[ *addr ] ;
    ++( CPU_PC ) ;
    CPU_obj->currentStatus.deltaCycle += 16 ;
} // lr35902::LD_A_ref_offset16()

/*----------------------------- lr35902::LD X to ref(HL) -----------------------------*/

void lr35902::LD_refBC_A() {
    CPU_obj->memory[ CPU_obj->Get_BC() ] = CPU_regs[ A ] ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_refBC_A()

void lr35902::LD_refDE_A() {
    CPU_obj->memory[ CPU_obj->Get_DE() ] = CPU_regs[ A ] ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_refDE_A()

void lr35902::LD_FFZZ_offset8_A() {
    CPU_obj->memory[ 0xFF00 + CPU_obj->memory[ ++( CPU_PC ) ] ] = CPU_regs[ A ] ;
    CPU_obj->currentStatus.deltaCycle += 12 ;
} // lr35902::LD_FFZZ_offset8_A()

void lr35902::LD_FFZZ_offset_regC_A() {
    CPU_obj->memory[ 0xFF00 + CPU_regs[ C ] ] = CPU_regs[ A ] ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_FFZZ_offset_regC_A()

void lr35902::LD_offset16_A() {
    uint16_t* addr = (uint16_t*)( CPU_obj->memory + ++( CPU_PC ) ) ;
    CPU_obj->memory[ *addr ] = CPU_regs[ A ] ;
    ++( CPU_PC ) ;
    CPU_obj->currentStatus.deltaCycle += 16 ;
} // lr35902::LD_offset16_A()

void lr35902::LD_BC_imm16() {
    uint16_t* value = (uint16_t*)( CPU_obj->memory + ++( CPU_PC ) ) ;
    CPU_obj->Set_BC( *value ) ;
    ++( CPU_PC ) ;
    CPU_obj->currentStatus.deltaCycle += 12 ;
} // lr35902::LD_BC_imm16()

void lr35902::LD_DE_imm16() {
    uint16_t* value = (uint16_t*)( CPU_obj->memory + ++( CPU_PC ) ) ;
    CPU_obj->Set_DE( *value ) ;
    ++( CPU_PC ) ;
    CPU_obj->currentStatus.deltaCycle += 12 ;
} // lr35902::LD_DE_imm16()

void lr35902::LD_HL_imm16() {
    uint16_t* value = (uint16_t*)( CPU_obj->memory + ++( CPU_PC ) ) ;
    CPU_obj->Set_HL( *value ) ;
    ++( CPU_PC ) ;
    CPU_obj->currentStatus.deltaCycle += 12 ;
} // lr35902::LD_HL_imm16()

void lr35902::LD_SP_imm16() {
    uint16_t* value = (uint16_t*)( CPU_obj->memory + ++( CPU_PC ) ) ;
    CPU_obj->Set_SP( *value ) ;
    ++( CPU_PC ) ;
    CPU_obj->currentStatus.deltaCycle += 12 ;
} // lr35902::LD_SP_imm16()

void lr35902::LD_FFZZ_offset16_SP() {
    uint16_t* addr = (uint16_t*)( CPU_obj->memory + ++( CPU_PC ) ) ;
    *( (uint16_t*)(CPU_obj->memory) + *addr ) = CPU_SP ;
    ++( CPU_PC ) ;
    CPU_obj->currentStatus.deltaCycle += 20 ;
} // lr35902::LD_FFZZ_offset16_SP()

void lr35902::LD_HL_SP_offset_sign8() {
    uint16_t result = CPU_SP + CPU_obj->memory[ ++( CPU_PC ) ] ;

    if ( result > 0b00001111 ) {
        CPU_obj->Set_HCFlag() ;
    } // if
    if ( result > 0b11111111 ) {
        CPU_obj->Set_CFlag() ;
    } // if

    CPU_obj->Set_HL( result ) ;
    CPU_obj->currentStatus.deltaCycle += 12 ;
} // lr35902::LD_HL_SP_offset_sign8()

void lr35902::LD_SP_HL() {
    CPU_SP = CPU_obj->Get_HL() ;
    CPU_obj->currentStatus.deltaCycle += 8 ;
} // lr35902::LD_SP_HL()
