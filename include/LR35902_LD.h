#ifndef LR35902_LD_H
#define LR35902_LD_H

#include <cstdio>

namespace lr35902 {

/*LD imm8 to X*/
void LD_B_imm8() ; // 0x06
void LD_C_imm8() ; // 0x0E
void LD_D_imm8() ; // 0x16
void LD_E_imm8() ; // 0x1E
void LD_H_imm8() ; // 0x26
void LD_L_imm8() ; // 0x2E
void LD_A_imm8() ; // 0x3E

/*LD X to B*/

void LD_B_B() ;  // 0x40
void LD_B_C() ;  // 0x41
void LD_B_D() ;  // 0x42
void LD_B_E() ;  // 0x43
void LD_B_H() ;  // 0x44
void LD_B_L() ;  // 0x45
void LD_B_A() ;  // 0x47

/*LD X to C*/

void LD_C_B() ;  // 0x48
void LD_C_C() ;  // 0x49
void LD_C_D() ;  // 0x4A
void LD_C_E() ;  // 0x4B
void LD_C_H() ;  // 0x4C
void LD_C_L() ;  // 0x4D
void LD_C_A() ;  // 0x4F

/*LD X to D*/

void LD_D_B() ;  // 0x50
void LD_D_C() ;  // 0x51
void LD_D_D() ;  // 0x52
void LD_D_E() ;  // 0x53
void LD_D_H() ;  // 0x54
void LD_D_L() ;  // 0x55
void LD_D_A() ;  // 0x57

/*LD X to E*/

void LD_E_B() ;  // 0x58
void LD_E_C() ;  // 0x59
void LD_E_D() ;  // 0x5A
void LD_E_E() ;  // 0x5B
void LD_E_H() ;  // 0x5C
void LD_E_L() ;  // 0x5D
void LD_E_A() ;  // 0x5F

/*LD X to H*/

void LD_H_B() ;  // 0x60
void LD_H_C() ;  // 0x61
void LD_H_D() ;  // 0x62
void LD_H_E() ;  // 0x63
void LD_H_H() ;  // 0x64
void LD_H_L() ;  // 0x65
void LD_H_A() ;  // 0x67

/*LD X to L*/

void LD_L_B() ;  // 0x68
void LD_L_C() ;  // 0x69
void LD_L_D() ;  // 0x6A
void LD_L_E() ;  // 0x6B
void LD_L_H() ;  // 0x6C
void LD_L_L() ;  // 0x6D
void LD_L_A() ;  // 0x6F

/*LD X to ref(HL)*/
void LD_refHL_imm8() ; // 0x36
void LD_refHL_B() ;  // 0x70
void LD_refHL_C() ;  // 0x71
void LD_refHL_D() ;  // 0x72
void LD_refHL_E() ;  // 0x73
void LD_refHL_H() ;  // 0x74
void LD_refHL_L() ;  // 0x75
/*HALT()--->0x76*/
void LD_refHL_A() ;  // 0x77
void LD_refHL_A_INC() ; // 0x22
void LD_refHL_A_DEC() ; // 0x32

/*LD X to A*/
void LD_A_B() ;  // 0x78
void LD_A_C() ;  // 0x79
void LD_A_D() ;  // 0x7A
void LD_A_E() ;  // 0x7B
void LD_A_H() ;  // 0x7C
void LD_A_L() ;  // 0x7D
void LD_A_A() ;  // 0x7F
void LD_A_refBC() ; // 0x0A
void LD_A_refDE() ; // 0x1A
void LD_A_FFZZ_offset8() ; // 0xF0
void LD_A_FFZZ_offset_regC() ; // 0xF2
void LD_A_ref_offset16() ; // 0xFA


/*LD refHL to X*/

void LD_B_refHL() ; // 0x46
void LD_C_refHL() ; // 0x4E
void LD_D_refHL() ; // 0x56
void LD_E_refHL() ; // 0x5E
void LD_H_refHL() ; // 0x66
void LD_L_refHL() ; // 0x6E
void LD_A_refHL() ; // 0x7E
void LD_A_refHL_INC() ; // 0x2A
void LD_A_refHL_DEC() ; // 0x3A


/*LD X to refBC*/
void LD_refBC_A() ; // 0x02

/*LD X to refDE*/
void LD_refDE_A() ; // 0x12

/*LD X value to FFZZ*/
void LD_FFZZ_offset8_A() ; // 0xE0
void LD_FFZZ_offset_regC_A() ; // 0xE2
void LD_offset16_A() ; // 0xEA

/*LD 16bit*/
void LD_BC_imm16() ; // 0x01
void LD_DE_imm16() ; // 0x11
void LD_HL_imm16() ; // 0x21
void LD_SP_imm16() ; // 0x31
void LD_FFZZ_offset16_SP() ; // 0x08
void LD_HL_SP_offset_sign8() ; // 0xF8
void LD_SP_HL() ; // 0xF9

} // lr35902

#endif // LR35902_LD_H
