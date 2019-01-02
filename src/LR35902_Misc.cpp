#include "LR35902_Misc.h"

#include <cpu.h>

void lr35902::NOP() {
    /*do nothing*/
    return ;
} // NOP()

void lr35902::STOP() { NOP() ;}

void lr35902::HALT() { NOP() ;}

void lr35902::PRIFIX() { NOP() ; }

void lr35902::DI() {
    CPU_obj->currentStatus.IME = false ;
}

void lr35902::EI() {
    CPU_obj->currentStatus.IME = true ;
}

