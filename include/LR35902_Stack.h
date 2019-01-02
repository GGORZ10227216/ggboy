//
// Created by orzgg on 12/8/18.
//

#ifndef CPU_LR35902_STACK_H
#define CPU_LR35902_STACK_H

namespace lr35902 {
    void PUSH_BC() ; // 0xC5
    void PUSH_DE() ; // 0xD5
    void PUSH_HL() ; // 0xE5
    void PUSH_AF() ; // 0xF5

    void POP_BC() ; // 0xC1
    void POP_DE() ; // 0xD1
    void POP_HL() ; // 0xE1
    void POP_AF() ; // 0xF1
}

#endif //CPU_LR35902_STACK_H
