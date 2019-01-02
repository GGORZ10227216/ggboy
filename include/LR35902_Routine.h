#ifndef LR35902_ROUTINE_H_INCLUDED
#define LR35902_ROUTINE_H_INCLUDED

namespace lr35902 {

void JR_imm8() ; // 0x18
void JR_NZ_imm8() ; //0x20
void JR_Z_imm8() ; // 0x28
void JR_NC_imm8() ; // 0x30
void JR_C_imm8() ; // 0x38

void RET_NZ() ; // 0xC0
void RET_Z() ; // 0xC8
void RET() ; // 0xC9
void RET_NC() ; // 0xD0
void RET_C() ; // 0xD8
void RETI() ; // 0xD9

void RST_00() ; // 0xC7
void RST_08() ; // 0xCF
void RST_10() ; // 0xD7
void RST_18() ; // 0xDF
void RST_20() ; // 0xE7
void RST_28() ; // 0xEF
void RST_30() ; // 0xF7
void RST_38() ; // 0xFF

void JP_NZ_imm16() ; // 0xC2
void JP_imm16() ; // 0xC3
void JP_Z_imm16() ; // 0xCA
void JP_NC_imm16() ; // 0xD2
void JP_C_imm16() ; // 0xDA
void JP_HL() ; // 0xE9

void CALL_NZ_imm16() ; // 0xC4
void CALL_Z_imm16() ; // 0xCC
void CALL_imm16() ; // 0xCD
void CALL_NC_imm16() ; // 0xD4
void CALL_C_imm16() ; // 0xDC

enum { LOW, HIGH } ;

}

#endif // LR35902_ROUTINE_H_INCLUDED
