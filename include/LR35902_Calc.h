#ifndef LR35902_CALC_H_INCLUDED
#define LR35902_CALC_H_INCLUDED

namespace lr35902 {

void DAA() ; // 0x27
void SCF() ; // 0x37
void CPL() ; // 0x2F
void CCF() ; // 0x3F

void INC_B() ; // 0x04
void INC_C() ; // 0x0C
void INC_D() ; // 0x14
void INC_E() ; // 0x1C
void INC_H() ; // 0x24
void INC_L() ; // 0x2C
void INC_refHL() ; // 0x34
void INC_A() ; // 0x3C

void INC_BC() ; // 0x03
void INC_DE() ; // 0x13
void INC_HL() ; // 0x23
void INC_SP() ; // 0x33

void DEC_B() ; // 0x05
void DEC_C() ; // 0x0D
void DEC_D() ; // 0x15
void DEC_E() ; // 0x1D
void DEC_H() ; // 0x25
void DEC_L() ; // 0x2D
void DEC_refHL() ; // 0x35
void DEC_A() ; // 0x3D

void DEC_BC() ; // 0x0B
void DEC_DE() ; // 0x1B
void DEC_HL() ; // 0x2B
void DEC_SP() ; // 0x3B

void ADD_A_B() ; // 0x80
void ADD_A_C() ; // 0x81
void ADD_A_D() ; // 0x82
void ADD_A_E() ; // 0x83
void ADD_A_H() ; // 0x84
void ADD_A_L() ; // 0x85
void ADD_A_refHL() ; // 0x86
void ADD_A_A() ; // 0x87
void ADD_A_imm8() ; // 0xC6

void ADD_HL_BC() ; // 0x09
void ADD_HL_DE() ; // 0x19
void ADD_HL_HL() ; // 0x29
void ADD_HL_SP() ; // 0x39
void ADD_SP_imm8() ; // 0xE8

void ADC_A_B() ; // 0x88
void ADC_A_C() ; // 0x89
void ADC_A_D() ; // 0x8A
void ADC_A_E() ; // 0x8B
void ADC_A_H() ; // 0x8C
void ADC_A_L() ; // 0x8D
void ADC_A_refHL() ; // 0x8E
void ADC_A_A() ; // 0x8F
void ADC_A_imm8() ; // 0xCE

void SUB_B() ; // 0x90
void SUB_C() ; // 0x91
void SUB_D() ; // 0x92
void SUB_E() ; // 0x93
void SUB_H() ; // 0x94
void SUB_L() ; // 0x95
void SUB_refHL() ; // 0x96
void SUB_A() ; // 0x97
void SUB_imm8() ; // 0xD6

void SBC_A_B() ; // 0x98
void SBC_A_C() ; // 0x99
void SBC_A_D() ; // 0x9A
void SBC_A_E() ; // 0x9B
void SBC_A_H() ; // 0x9C
void SBC_A_L() ; // 0x9D
void SBC_A_refHL() ; // 0x9E
void SBC_A_A() ; // 0x9F
void SBC_A_imm8() ; // 0xDE

void AND_B() ; // 0xA0
void AND_C() ; // 0xA1
void AND_D() ; // 0xA2
void AND_E() ; // 0xA3
void AND_H() ; // 0xA4
void AND_L() ; // 0xA5
void AND_refHL() ; // 0xA6
void AND_A() ; // 0xA7
void AND_imm8() ; // 0xE6

void XOR_B() ; // 0xA8
void XOR_C() ; // 0xA9
void XOR_D() ; // 0xAA
void XOR_E() ; // 0xAB
void XOR_H() ; // 0xAC
void XOR_L() ; // 0xAD
void XOR_refHL() ; // 0xAE
void XOR_A() ; // 0xAF
void XOR_imm8() ; // 0xEE

void OR_B() ; // 0xB0
void OR_C() ; // 0xB1
void OR_D() ; // 0xB2
void OR_E() ; // 0xB3
void OR_H() ; // 0xB4
void OR_L() ; // 0xB5
void OR_refHL() ; // 0xB6
void OR_A() ; // 0xB7
void OR_imm8() ; // 0xF6

void CP_B() ; // 0xB8
void CP_C() ; // 0xB9
void CP_D() ; // 0xBA
void CP_E() ; // 0xBB
void CP_H() ; // 0xBC
void CP_L() ; // 0xBD
void CP_refHL() ; // 0xBE
void CP_A() ; // 0xBF
void CP_imm8() ; // 0xFE

void RLCA() ; // 0x07
void RRCA() ; // 0x0F
void RLA() ;  // 0x17
void RRA() ;  // 0x1F

}


#endif // LR35902_CALC_H_INCLUDED
