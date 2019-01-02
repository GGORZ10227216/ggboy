//
// Created by ORZGG on 2018/12/30.
//

#ifndef CPU_ERRORCODE_H
#define CPU_ERRORCODE_H

namespace lr35902 {
    enum errCode {
        UNKNOWN_OPCODE   = -1,
        CANT_OPEN_ROMFILE = -2,
        MBC_NOT_SUPPORT = -3,
        UNKNOWN_ROM_SIZE = -4,
        UNKNOWN_RAM_SIZE = -5,
    } ;
} // lr35902


#endif //CPU_ERRORCODE_H
