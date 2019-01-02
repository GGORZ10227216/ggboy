//
// Created by orzgg on 12/8/18.
//

#include "LR35902_Stack.h"

#include <cpu.h>
#include <iostream>

void lr35902::PUSH_BC() {
    CPU_obj->memory[ --CPU_SP ] = CPU_regs[ B ] ;
    CPU_obj->memory[ --CPU_SP ] = CPU_regs[ C ] ;
}

void lr35902::PUSH_DE() {
    CPU_obj->memory[ --CPU_SP ] = CPU_regs[ D ] ;
    CPU_obj->memory[ --CPU_SP ] = CPU_regs[ E ] ;
}

void lr35902::PUSH_HL() {
    CPU_obj->memory[ --CPU_SP ] = CPU_regs[ H ] ;
    CPU_obj->memory[ --CPU_SP ] = CPU_regs[ L ] ;
}

void lr35902::PUSH_AF() {
    CPU_obj->memory[ --CPU_SP ] = CPU_regs[ A ] ;
    CPU_obj->memory[ --CPU_SP ] = CPU_regs[ F ] ;
}

void lr35902::POP_BC() {
    CPU_regs[ C ] = CPU_obj->memory[ CPU_SP ] ;
    CPU_regs[ B ] = CPU_obj->memory[ ++CPU_SP ] ;
    ++CPU_SP ;
}

void lr35902::POP_DE() {
    CPU_regs[ E ] = CPU_obj->memory[ CPU_SP ] ;
    CPU_regs[ D ] = CPU_obj->memory[ ++CPU_SP ] ;
    ++CPU_SP ;
}

void lr35902::POP_HL() {
    CPU_regs[ L ] = CPU_obj->memory[ CPU_SP ] ;
    CPU_regs[ H ] = CPU_obj->memory[ ++CPU_SP ] ;
    ++CPU_SP ;
}

void lr35902::POP_AF() {
    CPU_regs[ F ] = CPU_obj->memory[ CPU_SP ] ;
    CPU_regs[ A ] = CPU_obj->memory[ ++CPU_SP ] ;
    ++CPU_SP ;
    CPU_regs[ F ] = 0b11110000 ; // set all flags
}