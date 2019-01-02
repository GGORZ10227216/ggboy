//
// Created by orzgg on 12/14/18.
//

#include <cpu_state.h>
#include <cpu.h>

CPU_status::CPU_status() {
    memset( regs, '\0', sizeof(uint8_t) * 8 ) ;
    SP = 0x00 ;
    PC = 0x00 ;
    IME = true ;
} // CPU_state()

CPU_status::CPU_status( const LR35902* srcCpu ) {
    memcpy( regs, srcCpu->currentStatus.regs, sizeof( uint8_t ) * 8 ) ;
    SP = srcCpu->currentStatus.SP ;
    PC = srcCpu->currentStatus.PC ;
    IME = srcCpu->currentStatus.IME ;
    deltaCycle = srcCpu->currentStatus.deltaCycle ;
} // CPU_state()