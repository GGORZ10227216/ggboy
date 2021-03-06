enum instructions {
    NOP = 0x0,
    LD_BC_imm16 = 0x1,
    LD_refBC_A = 0x2,
    INC_BC = 0x3,
    INC_B = 0x4,
    DEC_B = 0x5,
    LD_B_imm8 = 0x6,
    RLCA = 0x07,
    LD_FFZZ_offset16_SP = 0x8,
    ADD_HL_BC = 0x9,
    LD_A_refBC = 0xa,
    DEC_BC = 0xb,
    INC_C = 0xc,
    DEC_C = 0xd,
    LD_C_imm8 = 0xe,
    RRCA = 0xf,
    STOP = 0x10,
    LD_DE_imm16 = 0x11,
    LD_refDE_A = 0x12,
    INC_DE = 0x13,
    INC_D = 0x14,
    DEC_D = 0x15,
    LD_D_imm8 = 0x16,
    RLA = 0x17,
    JR_imm8 = 0x18,
    ADD_HL_DE = 0x19,
    LD_A_refDE = 0x1a,
    DEC_DE = 0x1b,
    INC_E = 0x1c,
    DEC_E = 0x1d,
    LD_E_imm8 = 0x1e,
    RRA = 0x1f,
    JR_NZ_imm8 = 0x20,
    LD_HL_imm16 = 0x21,
    LD_refHL_A_INC = 0x22,
    INC_HL = 0x23,
    INC_H = 0x24,
    DEC_H = 0x25,
    LD_H_imm8 = 0x26,
    DAA = 0x27,
    JR_Z_imm8 = 0x28,
    ADD_HL_HL = 0x29,
    LD_A_refHL_INC = 0x2a,
    DEC_HL = 0x2b,
    INC_L = 0x2c,
    DEC_L = 0x2d,
    LD_L_imm8 = 0x2e,
    CPL = 0x2f,
    JR_NC_imm8 = 0x30,
    LD_SP_imm16 = 0x31,
    LD_refHL_A_DEC = 0x32,
    INC_SP = 0x33,
    INC_refHL = 0x34,
    DEC_refHL = 0x35,
    LD_refHL_imm8 = 0x36,
    SCF = 0x37,
    JR_C_imm8 = 0x38,
    ADD_HL_SP = 0x39,
    LD_A_refHL_DEC = 0x3a,
    DEC_SP = 0x3b,
    INC_A = 0x3c,
    DEC_A = 0x3d,
    LD_A_imm8 = 0x3e,
    CCF = 0x3f,
    LD_B_B = 0x40,
    LD_B_C = 0x41,
    LD_B_D = 0x42,
    LD_B_E = 0x43,
    LD_B_H = 0x44,
    LD_B_L = 0x45,
    LD_B_refHL = 0x46,
    LD_B_A = 0x47,
    LD_C_B = 0x48,
    LD_C_C = 0x49,
    LD_C_D = 0x4a,
    LD_C_E = 0x4b,
    LD_C_H = 0x4c,
    LD_C_L = 0x4d,
    LD_C_refHL = 0x4e,
    LD_C_A = 0x4f,
    LD_D_B = 0x50,
    LD_D_C = 0x51,
    LD_D_D = 0x52,
    LD_D_E = 0x53,
    LD_D_H = 0x54,
    LD_D_L = 0x55,
    LD_D_refHL = 0x56,
    LD_D_A = 0x57,
    LD_E_B = 0x58,
    LD_E_C = 0x59,
    LD_E_D = 0x5a,
    LD_E_E = 0x5b,
    LD_E_H = 0x5c,
    LD_E_L = 0x5d,
    LD_E_refHL = 0x5e,
    LD_E_A = 0x5f,
    LD_H_B = 0x60,
    LD_H_C = 0x61,
    LD_H_D = 0x62,
    LD_H_E = 0x63,
    LD_H_H = 0x64,
    LD_H_L = 0x65,
    LD_H_refHL = 0x66,
    LD_H_A = 0x67,
    LD_L_B = 0x68,
    LD_L_C = 0x69,
    LD_L_D = 0x6a,
    LD_L_E = 0x6b,
    LD_L_H = 0x6c,
    LD_L_L = 0x6d,
    LD_L_refHL = 0x6e,
    LD_L_A = 0x6f,
    LD_refHL_B = 0x70,
    LD_refHL_C = 0x71,
    LD_refHL_D = 0x72,
    LD_refHL_E = 0x73,
    LD_refHL_H = 0x74,
    LD_refHL_L = 0x75,
    HALT = 0x76,
    LD_refHL_A = 0x77,
    LD_A_B = 0x78,
    LD_A_C = 0x79,
    LD_A_D = 0x7a,
    LD_A_E = 0x7b,
    LD_A_H = 0x7c,
    LD_A_L = 0x7d,
    LD_A_refHL = 0x7e,
    LD_A_A = 0x7f,
    ADD_A_B = 0x80,
    ADD_A_C = 0x81,
    ADD_A_D = 0x82,
    ADD_A_E = 0x83,
    ADD_A_H = 0x84,
    ADD_A_L = 0x85,
    ADD_A_refHL = 0x86,
    ADD_A_A = 0x87,
    ADC_A_B = 0x88,
    ADC_A_C = 0x89,
    ADC_A_D = 0x8a,
    ADC_A_E = 0x8b,
    ADC_A_H = 0x8c,
    ADC_A_L = 0x8d,
    ADC_A_refHL = 0x8e,
    ADC_A_A = 0x8f,
    SUB_B = 0x90,
    SUB_C = 0x91,
    SUB_D = 0x92,
    SUB_E = 0x93,
    SUB_H = 0x94,
    SUB_L = 0x95,
    SUB_refHL = 0x96,
    SUB_A = 0x97,
    SBC_A_B = 0x98,
    SBC_A_C = 0x99,
    SBC_A_D = 0x9a,
    SBC_A_E = 0x9b,
    SBC_A_H = 0x9c,
    SBC_A_L = 0x9d,
    SBC_A_refHL = 0x9e,
    SBC_A_A = 0x9f,
    AND_B = 0xa0,
    AND_C = 0xa1,
    AND_D = 0xa2,
    AND_E = 0xa3,
    AND_H = 0xa4,
    AND_L = 0xa5,
    AND_refHL = 0xa6,
    AND_A = 0xa7,
    XOR_B = 0xa8,
    XOR_C = 0xa9,
    XOR_D = 0xaa,
    XOR_E = 0xab,
    XOR_H = 0xac,
    XOR_L = 0xad,
    XOR_refHL = 0xae,
    XOR_A = 0xaf,
    OR_B = 0xb0,
    OR_C = 0xb1,
    OR_D = 0xb2,
    OR_E = 0xb3,
    OR_H = 0xb4,
    OR_L = 0xb5,
    OR_refHL = 0xb6,
    OR_A = 0xb7,
    CP_B = 0xb8,
    CP_C = 0xb9,
    CP_D = 0xba,
    CP_E = 0xbb,
    CP_H = 0xbc,
    CP_L = 0xbd,
    CP_refHL = 0xbe,
    CP_A = 0xbf,
    RET_NZ = 0xc0,
    POP_BC = 0xc1,
    JP_NZ_imm16 = 0xc2,
    JP_imm16 = 0xc3,
    CALL_NZ_imm16 = 0xc4,
    PUSH_BC = 0xc5,
    ADD_A_imm8 = 0xc6,
    RST_00 = 0xc7,
    RET_Z = 0xc8,
    RET = 0xc9,
    JP_Z_imm16 = 0xca,
    PRIFIX = 0xcb,
    CALL_Z_imm16 = 0xcc,
    CALL_imm16 = 0xcd,
    ADC_A_imm8 = 0xce,
    RST_08 = 0xcf,
    RET_NC = 0xd0,
    POP_DE = 0xd1,
    JP_NC_imm16 = 0xd2,
    CALL_NC_imm16 = 0xd4,
    PUSH_DE = 0xd5,
    SUB_imm8 = 0xd6,
    RST_10 = 0xd7,
    RET_C = 0xd8,
    RETI = 0xd9,
    JP_C_imm16 = 0xda,
    CALL_C_imm16 = 0xdc,
    SBC_A_imm8 = 0xde,
    RST_18 = 0xdf,
    LD_FFZZ_offset8_A = 0xe0,
    POP_HL = 0xe1,
    LD_FFZZ_offset_regC_A = 0xe2,
    PUSH_HL = 0xe5,
    AND_imm8 = 0xe6,
    RST_20 = 0xe7,
    ADD_SP_imm8 = 0xe8,
    JP_HL = 0xe9,
    LD_offset16_A = 0xea,
    XOR_imm8 = 0xee,
    RST_28 = 0xef,
    LD_A_FFZZ_offset8 = 0xf0,
    POP_AF = 0xf1,
    LD_A_FFZZ_offset_regC = 0xf2,
    DI = 0xf3,
    PUSH_AF = 0xf5,
    OR_imm8 = 0xf6,
    RST_30 = 0xf7,
    LD_HL_SP_offset_sign8 = 0xf8,
    LD_SP_HL = 0xf9,
    LD_A_ref_offset16 = 0xfa,
    EI = 0xfb,
    CP_imm8 = 0xfe,
    RST_38 = 0xff
} ;