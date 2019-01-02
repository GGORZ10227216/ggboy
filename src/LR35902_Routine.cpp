//
// Created by orzgg on 12/6/18.
//

#include <LR35902_Routine.h>

#include <cpu.h>
#include <iostream>

/* JR */

void lr35902::JR_imm8() {
    // Not sure what behavior is right :
    // PC = PC + 8 (use this approach now)
    // PC = PCimm + 8
    CPU_PC = CPU_PC + static_cast<int8_t>( CPU_obj->memory[ ++CPU_PC ] ) ;
}

void lr35902::JR_NZ_imm8() {
    if ( !CPU_obj->Get_ZeroFlag() )
        CPU_PC = CPU_PC + static_cast<int8_t>( CPU_obj->memory[ ++CPU_PC ] ) ;
    else
        ++CPU_PC ;
}

void lr35902::JR_Z_imm8() {
    if ( CPU_obj->Get_ZeroFlag() )
        CPU_PC = CPU_PC + static_cast<int8_t>( CPU_obj->memory[ ++CPU_PC ] ) ;
    else
        ++CPU_PC ;
}

void lr35902::JR_NC_imm8() {
    if ( !CPU_obj->Get_CFlag() )
        CPU_PC = CPU_PC + static_cast<int8_t>( CPU_obj->memory[ ++CPU_PC ] ) ;
    else
        ++CPU_PC ;
}

void lr35902::JR_C_imm8() {
    if ( CPU_obj->Get_CFlag() )
        CPU_PC = CPU_PC + static_cast<int8_t>( CPU_obj->memory[ ++CPU_PC ] ) ;
    else
        ++CPU_PC ;
}

/* RET */

void lr35902::RET_NZ() {
    if ( !CPU_obj->Get_ZeroFlag() ) {
        uint8_t* PC_highlow = (uint8_t*) &( CPU_PC ) ;
        PC_highlow[ HIGH ] = CPU_obj->memory[ CPU_SP + 1 ] ;
        PC_highlow[ LOW ] = CPU_obj->memory[ CPU_SP ] ;
        CPU_SP += 2 ;
    } // if
    else {}
}

void lr35902::RET_Z() {
    if ( CPU_obj->Get_ZeroFlag() ) {
        uint8_t* PC_highlow = (uint8_t*) &( CPU_PC ) ;
        PC_highlow[ HIGH ] = CPU_obj->memory[ CPU_SP + 1 ] ;
        PC_highlow[ LOW ] = CPU_obj->memory[ CPU_SP ] ;
        CPU_SP += 2 ;
    } // if
    else {}
}

void lr35902::RET() {
    uint8_t* PC_highlow = (uint8_t*) &( CPU_PC ) ;
    PC_highlow[ HIGH ] = CPU_obj->memory[ CPU_SP + 1 ] ;
    PC_highlow[ LOW ] = CPU_obj->memory[ CPU_SP ] ;
    CPU_SP += 2 ;
}

void lr35902::RET_NC() {
    if ( !CPU_obj->Get_CFlag() ) {
        uint8_t* PC_highlow = (uint8_t*) &( CPU_PC ) ;
        PC_highlow[ HIGH ] = CPU_obj->memory[ CPU_SP + 1 ] ;
        PC_highlow[ LOW ] = CPU_obj->memory[ CPU_SP ] ;
        CPU_SP += 2 ;
    } // if
    else {}
}

void lr35902::RET_C() {
    if ( CPU_obj->Get_CFlag() ) {
        uint8_t* PC_highlow = (uint8_t*) &( CPU_PC ) ;
        PC_highlow[ HIGH ] = CPU_obj->memory[ CPU_SP + 1 ] ;
        PC_highlow[ LOW ] = CPU_obj->memory[ CPU_SP ] ;
        CPU_SP += 2 ;
    } // if
    else {}
}

void lr35902::RETI() {
    CPU_obj->memory[ 0xFFFF ] = 0b00011111 ; // same as EI
    uint8_t* PC_highlow = (uint8_t*) &( CPU_PC ) ;
    PC_highlow[ HIGH ] = CPU_obj->memory[ CPU_SP + 1 ] ;
    PC_highlow[ LOW ] = CPU_obj->memory[ CPU_SP ] ;
    CPU_SP += 2 ;
}

/* RST */

void lr35902::RST_00() {
    uint8_t* PC_highlow = (uint8_t*) &( CPU_PC ) ;
    CPU_obj->memory[ CPU_SP - 1 ] = PC_highlow[ HIGH ] ;
    CPU_obj->memory[ CPU_SP - 2 ] = PC_highlow[ LOW ] ;
    CPU_SP -= 2 ;
    CPU_PC = 0x0 ;
}

void lr35902::RST_08() {
    uint8_t* PC_highlow = (uint8_t*) &( CPU_PC ) ;
    CPU_obj->memory[ CPU_SP - 1 ] = PC_highlow[ HIGH ] ;
    CPU_obj->memory[ CPU_SP - 2 ] = PC_highlow[ LOW ] ;
    CPU_SP -= 2 ;
    CPU_PC = 0x08 ;
}

void lr35902::RST_10() {
    uint8_t* PC_highlow = (uint8_t*) &( CPU_PC ) ;
    CPU_obj->memory[ CPU_SP - 1 ] = PC_highlow[ HIGH ] ;
    CPU_obj->memory[ CPU_SP - 2 ] = PC_highlow[ LOW ] ;
    CPU_SP -= 2 ;
    CPU_PC = 0x10 ;
}

void lr35902::RST_18() {
    uint8_t* PC_highlow = (uint8_t*) &( CPU_PC ) ;
    CPU_obj->memory[ CPU_SP - 1 ] = PC_highlow[ HIGH ] ;
    CPU_obj->memory[ CPU_SP - 2 ] = PC_highlow[ LOW ] ;
    CPU_SP -= 2 ;
    CPU_PC = 0x18 ;
}

void lr35902::RST_20() {
    uint8_t* PC_highlow = (uint8_t*) &( CPU_PC ) ;
    CPU_obj->memory[ CPU_SP - 1 ] = PC_highlow[ HIGH ] ;
    CPU_obj->memory[ CPU_SP - 2 ] = PC_highlow[ LOW ] ;
    CPU_SP -= 2 ;
    CPU_PC = 0x20 ;
}

void lr35902::RST_28() {
    uint8_t* PC_highlow = (uint8_t*) &( CPU_PC ) ;
    CPU_obj->memory[ CPU_SP - 1 ] = PC_highlow[ HIGH ] ;
    CPU_obj->memory[ CPU_SP - 2 ] = PC_highlow[ LOW ] ;
    CPU_SP -= 2 ;
    CPU_PC = 0x28 ;
}

void lr35902::RST_30() {
    uint8_t* PC_highlow = (uint8_t*) &( CPU_PC ) ;
    CPU_obj->memory[ CPU_SP - 1 ] = PC_highlow[ HIGH ] ;
    CPU_obj->memory[ CPU_SP - 2 ] = PC_highlow[ LOW ] ;
    CPU_SP -= 2 ;
    CPU_PC = 0x30 ;
}

void lr35902::RST_38() {
    uint8_t* PC_highlow = (uint8_t*) &( CPU_PC ) ;
    CPU_obj->memory[ CPU_SP - 1 ] = PC_highlow[ HIGH ] ;
    CPU_obj->memory[ CPU_SP - 2 ] = PC_highlow[ LOW ] ;
    CPU_SP -= 2 ;
    CPU_PC = 0x38 ;
}

/* JP */

void lr35902::JP_NZ_imm16() {
    if ( !CPU_obj->Get_ZeroFlag() ) {
        uint16_t immVal = *(uint16_t*)( CPU_obj->memory + ++CPU_PC ) ;
        CPU_PC = immVal ;
    } // if
    else
        ++ CPU_PC ;
    ++ CPU_PC ;
}

void lr35902::JP_imm16() {
    uint16_t immVal = *(uint16_t*)( CPU_obj->memory + ++CPU_PC ) ;
    CPU_PC = immVal ;
    ++ CPU_PC ;
}

void lr35902::JP_Z_imm16() {
    if ( CPU_obj->Get_ZeroFlag() ) {
        uint16_t immVal = *(uint16_t*)( CPU_obj->memory + ++CPU_PC ) ;
        CPU_PC = immVal ;
    } // if
    else
        ++ CPU_PC ;
    ++ CPU_PC ;
}

void lr35902::JP_NC_imm16() {
    if ( !CPU_obj->Get_CFlag() ) {
        uint16_t immVal = *(uint16_t*)( CPU_obj->memory + ++CPU_PC ) ;
        CPU_PC = immVal ;
    } // if
    else
        ++ CPU_PC ;
    ++ CPU_PC ;
}

void lr35902::JP_C_imm16() {
    if ( CPU_obj->Get_CFlag() ) {
        uint16_t immVal = *(uint16_t*)( CPU_obj->memory + ++CPU_PC ) ;
        CPU_PC = immVal ;
    } // if
    else
        ++ CPU_PC ;
    ++ CPU_PC ;
}

void lr35902::JP_HL() {
    CPU_PC = CPU_obj->Get_HL() ;
    ++ CPU_PC ;
}

/* CALL */

void lr35902::CALL_NZ_imm16() {
    if ( !CPU_obj->Get_ZeroFlag() ) {
        uint8_t* PC_highlow = (uint8_t*) &( CPU_PC ) ;
        CPU_obj->memory[ CPU_SP - 1 ] = PC_highlow[ HIGH ] ;
        CPU_obj->memory[ CPU_SP - 2 ] = PC_highlow[ LOW ] ;
        CPU_SP -= 2 ;
        CPU_PC = *(uint16_t*)( CPU_obj->memory + ++CPU_PC ) ;
    } // if
    else
        ++ CPU_PC ;
    ++ CPU_PC ;
}

void lr35902::CALL_Z_imm16() {
    if ( CPU_obj->Get_ZeroFlag() ) {
        uint8_t* PC_highlow = (uint8_t*) &( CPU_PC ) ;
        CPU_obj->memory[ CPU_SP - 1 ] = PC_highlow[ HIGH ] ;
        CPU_obj->memory[ CPU_SP - 2 ] = PC_highlow[ LOW ] ;
        CPU_SP -= 2 ;
        CPU_PC = *(uint16_t*)( CPU_obj->memory + ++CPU_PC ) ;
    } // if
    else
        ++ CPU_PC ;
    ++ CPU_PC ;
}

void lr35902::CALL_imm16() {
    uint8_t* PC_highlow = (uint8_t*) &( CPU_PC ) ;
    CPU_obj->memory[ CPU_SP - 1 ] = PC_highlow[ HIGH ] ;
    CPU_obj->memory[ CPU_SP - 2 ] = PC_highlow[ LOW ] ;
    CPU_SP -= 2 ;
    CPU_PC = *(uint16_t*)( CPU_obj->memory + ++CPU_PC ) ;
    ++ CPU_PC ;
}

void lr35902::CALL_NC_imm16() {
    if ( !CPU_obj->Get_CFlag() ) {
        uint8_t* PC_highlow = (uint8_t*) &( CPU_PC ) ;
        CPU_obj->memory[ CPU_SP - 1 ] = PC_highlow[ HIGH ] ;
        CPU_obj->memory[ CPU_SP - 2 ] = PC_highlow[ LOW ] ;
        CPU_SP -= 2 ;
        CPU_PC = *(uint16_t*)( CPU_obj->memory + ++CPU_PC ) ;
    } // if
    else
        ++ CPU_PC ;
    ++ CPU_PC ;
}

void lr35902::CALL_C_imm16() {
    if ( CPU_obj->Get_CFlag() ) {
        uint8_t* PC_highlow = (uint8_t*) &( CPU_PC ) ;
        CPU_obj->memory[ CPU_SP - 1 ] = PC_highlow[ HIGH ] ;
        CPU_obj->memory[ CPU_SP - 2 ] = PC_highlow[ LOW ] ;
        CPU_SP -= 2 ;
        CPU_PC = *(uint16_t*)( CPU_obj->memory + ++CPU_PC ) ;
    } // if
    else
        ++ CPU_PC ;
    ++ CPU_PC ;
}


