#include "cpu.h"
#include <MBC.h>

#include <errorCode.h>
#include <cstring>
#include <cpu_state.h>
#include <EmuFramework.h>

using namespace lr35902 ;

#define CPU_regs currentStatus.regs
#define CPU_SP   currentStatus.SP
#define CPU_PC   currentStatus.PC
#define memory _mmu.getMainMemory()

uint16_t LR35902::cpc = 0 ;
uint8_t LR35902::cop = 0 ;


LR35902::LR35902( MBC& mmu ) : _mmu( mmu ) {
    // cpu_debug = fopen( "log.txt", "w+" ) ;

    currentStatus.PC = 0x0100 ;
    currentStatus.SP = 0xFFFE ;
    currentStatus.regs[ A ] = 0x01 ;
    currentStatus.regs[ F ] = 0xB0 ;
    currentStatus.regs[ B ] = 0x00 ;
    currentStatus.regs[ C ] = 0x13 ;
    currentStatus.regs[ D ] = 0x00 ;
    currentStatus.regs[ E ] = 0xD8 ;
    currentStatus.regs[ H ] = 0x01 ;
    currentStatus.regs[ L ] = 0x4D ;
    currentStatus.IME = false ;

    memory[ TIMA ] = 0x00 ;
    memory[ TMA ] = 0x00 ;
    memory[ TAC ] = 0x00 ;
    memory[ IF ] = 0x00 ; // from reddit/emudev
    memory[0xFF10] = 0x80 ; // SOUND, NRxx
    memory[0xFF11] = 0xBF ;
    memory[0xFF12] = 0xF3 ;
    memory[0xFF14] = 0xBF ;
    memory[0xFF16] = 0x3F ;
    memory[0xFF17] = 0x00 ;
    memory[0xFF19] = 0xBF ;
    memory[0xFF1A] = 0x7F ;
    memory[0xFF1B] = 0xFF ;
    memory[0xFF1C] = 0x9F ;
    memory[0xFF1E] = 0xBF ;
    memory[0xFF20] = 0xFF ;
    memory[0xFF21] = 0x00 ;
    memory[0xFF22] = 0x00 ;
    memory[0xFF23] = 0xBF ;
    memory[0xFF24] = 0x77 ;
    memory[0xFF25] = 0xF3 ;
    memory[0xFF26] = 0xF1 ;
    memory[LCDC] = 0x91 ;
    memory[SCY] = 0x00 ;
    memory[SCX] = 0x00 ;
    memory[LY] = 0x00 ;
    memory[BGP] = 0xFC ;
    memory[OBP0] = 0xFF ;
    memory[OBP1] = 0xFF ;
    memory[WY] = 0x00 ;
    memory[WX] = 0x00 ;
    memory[ IE ] = 0x00 ;
}

void LR35902::LogCPU( uint8_t opcode ) {
    fprintf( cpu_debug, "PC: 0x%x opcode: 0x%x\n", CPU_PC, opcode ) ;
    fprintf( cpu_debug, "\tAF: %x BC: %x DE: %x HL: %x SP: %x\n\n", Get_AF(),
        Get_BC(),
        Get_DE(),
        Get_HL(),
        currentStatus.SP
    ) ;

    fprintf( stdout, "PC: 0x%x opcode: 0x%x\n", CPU_PC, opcode ) ;
    fprintf( stdout, "\tAF: %x BC: %x DE: %x HL: %x SP: %x\n\n", Get_AF(),
             Get_BC(),
             Get_DE(),
             Get_HL(),
             currentStatus.SP
    ) ;
}

void LR35902::ExecuteCurrentInstruction() {
    uint8_t opcode = MEMREAD(CPU_PC) ;
    uint16_t pc = CPU_PC ;
    cpc = pc ;
    cop = opcode ;
#ifdef DEBUG
    printf( "[%d] PC=%x opcode=%x\n", instC++, pc, opcode
            /*
            , CPU_regs[ A ]
            , CPU_regs[ B ]
            , CPU_regs[ C ]
            , CPU_regs[ D ]
            , CPU_regs[ E ]
            , CPU_regs[ H ]
            , CPU_regs[ L ]
            , CPU_SP
            , Get_ZeroFlag()
            , Get_SubFlag()
            , Get_HCFlag()
            , Get_CFlag()
             */
    ) ;
#endif
    switch ( opcode ) {
        case 0x00 :
            /* NOP */
            currentStatus.deltaCycle = 4 ;
            break ;
        /* 8-Bit Transfer and Input/Output  */
        case 0x40 : case 0x41 : case 0x42 : case 0x43 : case 0x44 : case 0x45 : case 0x47 :
        case 0x48 : case 0x49 : case 0x4A : case 0x4B : case 0x4C : case 0x4D : case 0x4F :
        case 0x50 : case 0x51 : case 0x52 : case 0x53 : case 0x54 : case 0x55 : case 0x57 :
        case 0x58 : case 0x59 : case 0x5A : case 0x5B : case 0x5C : case 0x5D : case 0x5F :
        case 0x60 : case 0x61 : case 0x62 : case 0x63 : case 0x64 : case 0x65 : case 0x67 :
        case 0x68 : case 0x69 : case 0x6A : case 0x6B : case 0x6C : case 0x6D : case 0x6F :
        case 0x78 : case 0x79 : case 0x7A : case 0x7B : case 0x7C : case 0x7D : case 0x7F :
            CPU_regs[ ( opcode & 0b00111000 ) >> 3 ] = CPU_regs[ ( opcode & 0b00000111 ) ] ;
            currentStatus.deltaCycle = 4 ;
            break ;
        case 0x06 : case 0x0E : case 0x16 : case 0x1E : case 0x26 : case 0x2E : case 0x3E :
            currentStatus.regs[ ( opcode & 0b00111000 ) >> 3 ] = MEMREAD(++CPU_PC) ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0x46 : case 0x4E : case 0x56 : case 0x5E : case 0x66 : case 0x6E : case 0x7E :
            CPU_regs[ ( opcode & 0b00111000 ) >> 3 ] = MEMREAD(Get_HL()) ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0x70 : case 0x71 : case 0x72 : case 0x73 : case 0x74 : case 0x75 : case 0x77 :
            MEMWRITE(Get_HL(), CPU_regs[(opcode & 0b00000111)]) ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0x36 :
            MEMWRITE(Get_HL(),
                     MEMREAD(++CPU_PC)
            ) ;
            currentStatus.deltaCycle = 12 ;
            break ;
        case 0x0A :
            CPU_regs[ A ] = MEMREAD(Get_BC()) ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0x1A :
            CPU_regs[ A ] = MEMREAD(Get_DE()) ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0xF2 :
            CPU_regs[ A ] = MEMREAD(0xFF00 + CPU_regs[C]) ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0xE2 :
            MEMWRITE(0xFF00 + CPU_regs[C], CPU_regs[A]) ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0xF0 :
            CPU_regs[ A ] = MEMREAD(
                    0xFF00 + MEMREAD(++CPU_PC)
            ) ;
            currentStatus.deltaCycle = 12 ;
            break ;
        case 0xE0 :
            MEMWRITE(0xFF00 + MEMREAD(++CPU_PC), CPU_regs[A]) ;
            currentStatus.deltaCycle = 12 ;
            break ;
        case 0xFA : {
            uint16_t addr = Fetch_16bitByMMU( ++CPU_PC ) ;
            CPU_regs[ A ] = MEMREAD(addr);
            ++( CPU_PC ) ;
            currentStatus.deltaCycle = 16 ;
        }   break ;
        case 0xEA : {
            uint16_t addr = Fetch_16bitByMMU( ++CPU_PC ) ;
            MEMWRITE(addr, CPU_regs[A]) ;
            ++CPU_PC ;
            currentStatus.deltaCycle = 16 ;
        }   break ;
        case 0x2A :
            CPU_regs[ A ] = MEMREAD(Get_HL()) ;
            Set_HL(static_cast<uint16_t>(Get_HL() + 1)) ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0x3A :
            CPU_regs[ A ] = MEMREAD(Get_HL()) ;
            Set_HL(static_cast<uint16_t>(Get_HL() - 1)) ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0x02 :
            MEMWRITE(Get_BC(), CPU_regs[A]) ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0x12 :
            MEMWRITE(Get_DE(), CPU_regs[A]) ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0x22 :
            MEMWRITE(Get_HL(), CPU_regs[A]) ;
            Set_HL( Get_HL() + 1 ) ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0x32 :
            MEMWRITE(Get_HL(), CPU_regs[A]) ;
            Set_HL( Get_HL() - 1 ) ;
            currentStatus.deltaCycle = 8 ;
            break ;

        /* 16-Bit Transfer Instructions */
        case 0x01 : case 0x11 : case 0x21 : case 0x31 : {
            uint16_t value = Fetch_16bitByMMU( CPU_PC + 1 ) ;

            switch ( opcode & 0b00110000 ) {
                case 0b00000000 :
                    Set_BC(value) ;
                    break;
                case 0b00010000 :
                    Set_DE(value);
                    break;
                case 0b00100000 :
                    Set_HL(value);
                    break;
                case 0b00110000 :
                    CPU_SP = value;
                    break;
            } // switch

            CPU_PC += 2 ;
            currentStatus.deltaCycle = 12;
        }   break ;
        case 0xF9 :
            CPU_SP = Get_HL() ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0xC5 :
            MEMWRITE(--CPU_SP, CPU_regs[B]) ;
            MEMWRITE(--CPU_SP, CPU_regs[C]) ;
            currentStatus.deltaCycle = 16 ;
            break;
        case 0xD5 :
            MEMWRITE(--CPU_SP, CPU_regs[D]) ;
            MEMWRITE(--CPU_SP, CPU_regs[E]) ;
            currentStatus.deltaCycle = 16 ;
            break;
        case 0xE5 :
            MEMWRITE(--CPU_SP, CPU_regs[H]) ;
            MEMWRITE(--CPU_SP, CPU_regs[L]) ;
            currentStatus.deltaCycle = 16 ;
            break;
        case 0xF5 :
            MEMWRITE(--CPU_SP, CPU_regs[A]) ;
            MEMWRITE(--CPU_SP, CPU_regs[F]) ;
            currentStatus.deltaCycle = 16 ;
            break;
        case 0xC1 :
            CPU_regs[ C ] = MEMREAD(CPU_SP);
            CPU_regs[ B ] = MEMREAD(++CPU_SP) ;
            ++CPU_SP ;
            currentStatus.deltaCycle = 12 ;
            break;
        case 0xD1 :
            CPU_regs[ E ] = MEMREAD(CPU_SP);
            CPU_regs[ D ] = MEMREAD(++CPU_SP) ;
            currentStatus.deltaCycle = 12 ;
            ++CPU_SP ;
            break;
        case 0xE1 :
            CPU_regs[ L ] = MEMREAD(CPU_SP);
            CPU_regs[ H ] = MEMREAD(++CPU_SP) ;
            currentStatus.deltaCycle = 12 ;
            ++CPU_SP ;
            break;
        case 0xF1 :
            CPU_regs[ F ] = MEMREAD(CPU_SP);
            CPU_regs[ A ] = MEMREAD(++CPU_SP) ;
            currentStatus.deltaCycle = 12 ;
            ++CPU_SP ;
            break;
        case 0xF8 : {
            int8_t sOffset = static_cast<int8_t>( MEMREAD(++CPU_PC) ) ;
            uint16_t result = CPU_SP + sOffset ;
            if ( ( CPU_SP & 0xfff ) + sOffset >= 0x1000 ) {
                Set_HCFlag() ;
            } // if
            else
                Clear_HCFlag() ;

            if ( result < CPU_SP ) {
                Set_CFlag() ;
            } // if
            else
                Clear_CFlag() ;

            Clear_ZeroFlag() ;
            Clear_SubFlag() ;
            Set_HL( result ) ;
            currentStatus.deltaCycle = 12 ;
        }   break ;
        case 0x08 : {
            uint16_t addr = Fetch_16bitByMMU( ++CPU_PC ) ;
            MEMWRITE( addr, static_cast<uint8_t>( CPU_SP & 0x00FF ) ) ;
            MEMWRITE( addr + 1, static_cast<uint8_t>( CPU_SP >> 8 ) ) ;
            ++ CPU_PC ;
            currentStatus.deltaCycle = 20 ;
        }   break ;

        /* 8-Bit Arithmetic and Logical Operation */
        case 0x80 : case 0x81 : case 0x82 : case 0x83 : case 0x84 : case 0x85 : case 0x87 : {
            /* ADD A, r */
            uint8_t tempReg = CPU_regs[ A ] ;
            uint8_t regIndex = opcode & 0b00000111 ;
            if ( ( (tempReg & 0x0f) + (CPU_regs[ regIndex ] & 0x0f) & 0x10 ) == 0x10 )
                Set_HCFlag() ;
            else
                Clear_HCFlag() ;

            CPU_regs[ A ] += CPU_regs[ regIndex ] ;

            CPU_regs[ A ] < tempReg ? Set_CFlag() : Clear_CFlag() ;
            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            Clear_SubFlag();
            currentStatus.deltaCycle = 4 ;
        }   break ;
        case 0xC6 : {
            /* ADD A, imm8 */
            uint8_t tempReg = CPU_regs[ A ] ;
            if ( ( (tempReg & 0x0f) + (MEMREAD(CPU_PC + 1) & 0x0f) & 0x10 ) == 0x10 )
                Set_HCFlag() ;
            else
                Clear_HCFlag() ;

            CPU_regs[ A ] += MEMREAD(++(CPU_PC)) ;

            CPU_regs[ A ] < tempReg ? Set_CFlag() : Clear_CFlag() ;
            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            Clear_SubFlag();
            currentStatus.deltaCycle = 8 ;
        }   break ;
        case 0x86 : {
            /* ADD A, (HL) */
            uint8_t tempReg = CPU_regs[ A ] ;
            if ( ( (tempReg & 0x0f) + (MEMREAD(Get_HL()) & 0x0f) & 0x10 ) == 0x10 )
                Set_HCFlag() ;
            else
                Clear_HCFlag() ;

            CPU_regs[ A ] += MEMREAD(Get_HL()) ;

            CPU_regs[ A ] < tempReg ? Set_CFlag() : Clear_CFlag() ;
            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            Clear_SubFlag() ;
            currentStatus.deltaCycle = 8 ;
        }   break ;
        case 0x88 : case 0x89 : case 0x8A : case 0x8B : case 0x8C : case 0x8D : case 0x8F : {
            /* ADC A, r */
            uint8_t tempReg = CPU_regs[ A ] ;
            uint8_t regIndex = opcode & 0b00000111 ;
            uint16_t result = CPU_regs[ A ] + CPU_regs[ regIndex ] + Get_CFlag() ;
            if ( ((tempReg & 0x0f) + (CPU_regs[ regIndex ] & 0x0f) + Get_CFlag()) > 0x0F )
                Set_HCFlag() ;
            else
                Clear_HCFlag() ;

            CPU_regs[ A ] = static_cast<uint8_t> ( result ) ;

            result > 0xFF ? Set_CFlag() : Clear_CFlag() ;
            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            Clear_SubFlag();
            currentStatus.deltaCycle = 4 ;
        }   break ;
        case 0xCE : {
            /* ADC A, imm8 */
            uint8_t tempReg = CPU_regs[ A ] ;
            uint8_t immVal = MEMREAD(++CPU_PC) ;
            uint16_t result = CPU_regs[ A ] + immVal + Get_CFlag() ;

            if ( ( (tempReg & 0x0f) + (immVal & 0x0f) + Get_CFlag() & 0x10 ) == 0x10 )
                Set_HCFlag() ;
            else
                Clear_HCFlag() ;

            CPU_regs[ A ] = static_cast<uint8_t> ( result ) ;

            result > 0xFF ? Set_CFlag() : Clear_CFlag() ;
            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            Clear_SubFlag();
            currentStatus.deltaCycle = 8 ;
        }   break ;
        case 0x8E : {
            /* ADC A, (HL) */
            uint8_t tempReg = CPU_regs[ A ] ;
            uint8_t HL_val = MEMREAD( Get_HL() ) ;
            uint16_t result = CPU_regs[ A ] + HL_val + Get_CFlag() ;

            if ( ((tempReg & 0x0f) + (HL_val & 0xf)) + Get_CFlag() > 0x0F )
                Set_HCFlag() ;
            else
                Clear_HCFlag() ;

            CPU_regs[ A ] = static_cast<uint8_t> ( result ) ;
            result > 0xFF ? Set_CFlag() : Clear_CFlag() ;
            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            Clear_SubFlag();
            currentStatus.deltaCycle = 8 ;
        }   break ;
        case 0x90 : case 0x91 : case 0x92 : case 0x93 : case 0x94 : case 0x95 : case 0x97 : {
            /* SUB r */
            uint8_t tempReg = CPU_regs[ A ] ;
            uint8_t regIndex = static_cast<uint8_t> ( opcode & 0b00000111 ) ;

            CPU_regs[ A ] -= CPU_regs[ regIndex ] ;

            (tempReg & 0x0f) < (CPU_regs[ regIndex ] & 0x0f) ? Set_HCFlag() : Clear_HCFlag() ;
            CPU_regs[ A ] > tempReg ? Set_CFlag() : Clear_CFlag() ;
            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            Set_SubFlag() ;
            currentStatus.deltaCycle = 4 ;
        }   break ;
        case 0xD6 : {
            /* SUB imm8 */
            uint8_t tempReg = CPU_regs[ A ] ;
            uint8_t immVal = MEMREAD(++CPU_PC) ;

            CPU_regs[ A ] -= immVal ;

            (tempReg & 0x0f) < (MEMREAD(CPU_PC) & 0x0f) ? Set_HCFlag() : Clear_HCFlag() ;
            CPU_regs[ A ] > tempReg ? Set_CFlag() : Clear_CFlag() ;
            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            Set_SubFlag() ;
            currentStatus.deltaCycle = 8 ;
        } break ;
        case 0x96 : {
            /* SUB (HL) */
            uint8_t tempReg = CPU_regs[ A ] ;

            CPU_regs[ A ] -= MEMREAD(Get_HL()) ;

            (tempReg & 0x0f) < (MEMREAD(Get_HL()) & 0x0f) ? Set_HCFlag() : Clear_HCFlag() ;
            CPU_regs[ A ] > tempReg ? Set_CFlag() : Clear_CFlag() ;
            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            Set_SubFlag() ;
            currentStatus.deltaCycle = 8 ;
        } break ;
        case 0x98 : case 0x99 : case 0x9A : case 0x9B : case 0x9C : case 0x9D : case 0x9F : {
            /* SBC A, r */
            uint8_t tempReg = CPU_regs[ A ] ;
            uint8_t regIndex = static_cast<uint8_t> ( opcode & 0b00000111 ) ;
            uint16_t result = CPU_regs[ A ] - CPU_regs[ regIndex ] - Get_CFlag() ;

            CPU_regs[ A ] = static_cast<uint8_t> ( result )  ;

            (tempReg & 0x0f) < (CPU_regs[ regIndex ] & 0x0f) + Get_CFlag() ? Set_HCFlag() : Clear_HCFlag() ;
            result > tempReg ? Set_CFlag() : Clear_CFlag() ;
            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            Set_SubFlag() ;
            currentStatus.deltaCycle = 4 ;
        } break ;
        case 0xDE : {
            /* SBC A, imm8 */
            uint8_t tempReg = CPU_regs[ A ] ;
            uint8_t immVal = MEMREAD( ++CPU_PC ) ;
            uint16_t result = CPU_regs[ A ] - immVal - Get_CFlag() ;

            CPU_regs[ A ] = static_cast<uint8_t> ( result )  ;

            (tempReg & 0x0f) < (immVal & 0x0f) + Get_CFlag() ? Set_HCFlag() : Clear_HCFlag() ;
            result > CPU_regs[ A ] ? Set_CFlag() : Clear_CFlag() ;
            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            Set_SubFlag() ;
            currentStatus.deltaCycle = 8 ;
        }   break ;
        case 0x9E : {
            /* SBC A, (HL) */
            uint8_t tempReg = CPU_regs[ A ] ;
            uint8_t content = MEMREAD( Get_HL() ) ;
            uint16_t result = CPU_regs[ A ] - content - Get_CFlag() ;

            CPU_regs[ A ] = static_cast<uint8_t> ( result )  ;

            (tempReg & 0x0f) < (MEMREAD(Get_HL()) & 0x0f) + Get_CFlag() ?
            Set_HCFlag() : Clear_HCFlag() ;
            result > CPU_regs[ A ] ? Set_CFlag() : Clear_CFlag() ;
            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            Set_SubFlag() ;
            currentStatus.deltaCycle = 8 ;
        }   break ;
        case 0xA0 : case 0xA1 : case 0xA2 : case 0xA3 : case 0xA4 : case 0xA5 : case 0xA7 :
            /* AND r */
            CPU_regs[ A ] &= CPU_regs[ static_cast<uint8_t> ( opcode & 0b00000111 ) ] ;
            Clear_CFlag() ;
            Set_HCFlag() ;
            Clear_SubFlag() ;
            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            currentStatus.deltaCycle = 4 ;
        break ;
        case 0xE6 :
            /* AND imm8 */
            CPU_regs[ A ] &= MEMREAD(++CPU_PC) ;
            Clear_CFlag() ;
            Set_HCFlag() ;
            Clear_SubFlag() ;
            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0xA6 :
            /* AND (HL) */
            CPU_regs[ A ] &= MEMREAD(Get_HL()) ;
            Clear_CFlag() ;
            Set_HCFlag() ;
            Clear_SubFlag() ;
            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0xB0 : case 0xB1 : case 0xB2 : case 0xB3 : case 0xB4 : case 0xB5 : case 0xB7 :
            /* OR r */
            CPU_regs[ A ] |= CPU_regs[ static_cast<uint8_t> ( opcode & 0b00000111 ) ] ;
            Clear_CFlag() ;
            Clear_HCFlag() ;
            Clear_SubFlag() ;
            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            currentStatus.deltaCycle = 4 ;
            break ;
        case 0xF6 :
            /* OR imm8 */
            CPU_regs[ A ] |= MEMREAD(++CPU_PC) ;
            Clear_CFlag() ;
            Clear_HCFlag() ;
            Clear_SubFlag() ;
            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0xB6 :
            /* OR (HL) */
            CPU_regs[ A ] |= MEMREAD(Get_HL()) ;
            Clear_CFlag() ;
            Clear_HCFlag() ;
            Clear_SubFlag() ;
            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0xA8 : case 0xA9 : case 0xAA : case 0xAB : case 0xAC : case 0xAD : case 0xAF :
            /* XOR r */
            CPU_regs[ A ] ^= CPU_regs[ static_cast<uint8_t> ( opcode & 0b00000111 ) ] ;
            Clear_CFlag() ;
            Clear_HCFlag() ;
            Clear_SubFlag() ;
            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            currentStatus.deltaCycle = 4 ;
            break ;
        case 0xEE :
            /* XOR imm8 */
            CPU_regs[ A ] ^= MEMREAD(++CPU_PC) ;
            Clear_CFlag() ;
            Clear_HCFlag() ;
            Clear_SubFlag() ;
            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0xAE :
            /* XOR (HL) */
            CPU_regs[ A ] ^= MEMREAD(Get_HL()) ;
            Clear_CFlag() ;
            Clear_HCFlag() ;
            Clear_SubFlag() ;
            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0xB8 : case 0xB9 : case 0xBA : case 0xBB : case 0xBC : case 0xBD : case 0xBF : {
            /*CP (pandoc says its just SUB without result)*/
            uint8_t tempReg = CPU_regs[ A ], newResult = 0 ;
            uint8_t regIndex = static_cast<uint8_t> ( opcode & 0b00000111 ) ;

            newResult = CPU_regs[ A ] - CPU_regs[ regIndex ] ;

            (tempReg & 0x0f) < (CPU_regs[ regIndex ] & 0x0f) ? Set_HCFlag() : Clear_HCFlag() ;
            newResult > tempReg ? Set_CFlag() : Clear_CFlag() ;
            newResult == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            Set_SubFlag() ;
            currentStatus.deltaCycle = 4 ;
        }
            break ;
        case 0xFE : {
            /* CP imm8 */
            /* SUB imm8 */
            uint8_t tempReg = CPU_regs[ A ], newResult = 0 ;
            uint8_t immVal = MEMREAD(++CPU_PC) ;

            newResult = CPU_regs[ A ] - immVal ;

            (tempReg & 0x0f) < (MEMREAD(CPU_PC) & 0x0f) ? Set_HCFlag() : Clear_HCFlag() ;
            newResult > tempReg ? Set_CFlag() : Clear_CFlag() ;
            newResult == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            Set_SubFlag() ;
            currentStatus.deltaCycle = 8 ;
        }   break;
        case 0xBE : {
            /* CP (HL) */
            /* SUB (HL) */
            uint8_t tempReg = CPU_regs[ A ], newResult = 0 ;

            newResult = CPU_regs[ A ] - MEMREAD(Get_HL()) ;

            (tempReg & 0x0f) < (MEMREAD(Get_HL()) & 0x0f) ? Set_HCFlag() : Clear_HCFlag() ;
            newResult > tempReg ? Set_CFlag() : Clear_CFlag() ;
            newResult == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            Set_SubFlag() ;
            currentStatus.deltaCycle = 8 ;
        }
            break ;
        case 0x04 : case 0x0C : case 0x14 : case 0x1C : case 0x24 : case 0x2C : case 0x3C : {
            /* INC r */
            uint8_t regIndex = static_cast<uint8_t>( opcode & 0b00111000 ) >> 3 ;
            ( CPU_regs[ regIndex ] & 0x0F ) == 0x0F ? Set_HCFlag() : Clear_HCFlag() ;
            ++ CPU_regs[ regIndex ] ;
            CPU_regs[ regIndex ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            Clear_SubFlag();
            currentStatus.deltaCycle = 4 ;
        }   break ;
        case 0x34 : {
            /* INC (HL) */
            uint8_t hl_val = MEMREAD(Get_HL()) ;
            ( hl_val & 0x0F ) == 0x0F ? Set_HCFlag() : Clear_HCFlag() ;
            MEMWRITE(Get_HL(), hl_val + 1) ;
            static_cast<uint8_t> ( hl_val + 1 ) == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            Clear_SubFlag() ;
            currentStatus.deltaCycle = 12 ;
        }
            break ;
        case 0x05 : case 0x0D : case 0x15 : case 0x1D : case 0x25 : case 0x2D : case 0x3D : {
            /* DEC r */
            uint8_t regIndex = static_cast<uint8_t>( opcode & 0b00111000 ) >> 3 ;
            ( CPU_regs[ regIndex ] & 0x0F ) == 0 ? Set_HCFlag() : Clear_HCFlag() ;
            -- CPU_regs[ regIndex ] ;
            CPU_regs[ regIndex ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            Set_SubFlag();
            currentStatus.deltaCycle = 4 ;
        }   break ;
        case 0x35 : {
            /* DEC (HL) */
            uint8_t refHL = MEMREAD(Get_HL()) ;
            ( refHL & 0x0F ) == 0 ? Set_HCFlag() : Clear_HCFlag() ;
            MEMWRITE(Get_HL(), refHL - 1) ;
            MEMREAD(Get_HL()) == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            Set_SubFlag();
            currentStatus.deltaCycle = 12 ;
        }
            break ;
        /* 16-Bit Arithmetic Operation */
        case 0x09 : {
            /* ADD HL, BC */
            uint16_t tempReg = Get_HL() ;
            if ( (tempReg & 0x0fff) + (Get_BC() & 0x0fff) >= 0x1000 )
                Set_HCFlag() ;
            else
                Clear_HCFlag() ;

            Set_HL( Get_HL() + Get_BC() ) ;

            if ( Get_HL() < tempReg )
                Set_CFlag() ;
            else
                Clear_CFlag() ;
            Clear_SubFlag();
            currentStatus.deltaCycle = 8 ;
        } break ;
        case 0x19 : {
            /* ADD HL, DE */
            uint16_t tempReg = Get_HL() ;
            if ( ( (tempReg & 0x0fff) + (Get_DE() & 0x0fff) & 0x1000 ) == 0x1000 )
                Set_HCFlag() ;
            else
                Clear_HCFlag() ;

            Set_HL( Get_HL() + Get_DE() ) ;

            if ( Get_HL() < tempReg )
                Set_CFlag() ;
            else
                Clear_CFlag() ;
            Clear_SubFlag();
            currentStatus.deltaCycle = 8 ;
        }   break ;
        case 0x29 : {
            /* ADD HL, HL */
            uint16_t tempReg = Get_HL() ;
            if ( ( (tempReg & 0x0fff) + (tempReg & 0x0fff) & 0x1000 ) == 0x1000 )
                Set_HCFlag() ;
            else
                Clear_HCFlag() ;

            Set_HL( Get_HL() + Get_HL() ) ;

            if ( Get_HL() < tempReg )
                Set_CFlag() ;
            else
                Clear_CFlag() ;
            Clear_SubFlag();
            currentStatus.deltaCycle = 8 ;
        }   break ;
        case 0x39 : {
            /* ADD HL, SP */
            uint16_t tempReg = Get_HL() ;
            if ( ( (tempReg & 0x0fff) + (CPU_SP & 0x0fff) & 0x1000 ) == 0x1000 )
                Set_HCFlag() ;
            else
                Clear_HCFlag() ;

            Set_HL( Get_HL() + CPU_SP ) ;

            if ( Get_HL() < tempReg )
                Set_CFlag() ;
            else
                Clear_CFlag() ;
            Clear_SubFlag();
            currentStatus.deltaCycle = 8 ;
        }   break ;
        case 0xE8 : {
            /* ADD SP, r8 */
            uint16_t tempReg = CPU_SP ;
            if ( ( (tempReg & 0x0fff) + static_cast<uint16_t>( MEMREAD(CPU_PC + 1) ) & 0x1000 ) == 0x1000 )
                Set_HCFlag() ;
            else
                Clear_HCFlag() ;

            CPU_SP += MEMREAD(++CPU_PC) ;

            if ( CPU_SP < tempReg )
                Set_CFlag() ;
            else
                Clear_CFlag() ;

            Clear_ZeroFlag() ;
            Clear_SubFlag();
            currentStatus.deltaCycle = 16 ;
        }   break ;
        case 0x03 :
            /* INC BC */
            Set_BC(static_cast<uint16_t>(Get_BC() + 1)) ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0x13 :
            /* INC DE */
            Set_DE(static_cast<uint16_t>(Get_DE() + 1)) ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0x23 :
            /* INC HL */
            Set_HL(static_cast<uint16_t>(Get_HL() + 1)) ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0x33 :
            /* INC SP */
            ++ CPU_SP ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0x0B :
            /* DEC BC */
            Set_BC(static_cast<uint16_t>(Get_BC() - 1)) ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0x1B :
            /* DEC DE */
            Set_DE(static_cast<uint16_t>(Get_DE() - 1)) ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0x2B :
            /* DEC HL */
            Set_HL(static_cast<uint16_t>(Get_HL() - 1)) ;
            currentStatus.deltaCycle = 8 ;
            break ;
        case 0x3B :
            /* DEC SP */
            -- CPU_SP ;
            currentStatus.deltaCycle = 8 ;
            break ;

        /* Rotate Shift */
        case 0x07 :
            /* RLCA */
            ( CPU_regs[ A ] & 0b10000000 ) == 0 ? Clear_CFlag() : Set_CFlag() ;
            CPU_regs[ A ] = CPU_regs[ A ] << 1 ;
            CPU_regs[ A ] = CPU_regs[ A ] | static_cast<uint8_t>( Get_CFlag() ) ;

            Clear_ZeroFlag() ;
            Clear_HCFlag() ;
            Clear_SubFlag() ;
            /*diff 4*/
            currentStatus.deltaCycle = 4 ;
            break ;
        case 0x0F :
            /* RRCA */
            ( CPU_regs[ A ] & 0b00000001 ) == 0 ? Clear_CFlag() : Set_CFlag() ;
            CPU_regs[ A ] = CPU_regs[ A ] >> 1 ;
            CPU_regs[ A ] = CPU_regs[ A ] | static_cast<uint8_t>( Get_CFlag() ) << 7 ;

            Clear_ZeroFlag() ;
            Clear_HCFlag() ;
            Clear_SubFlag() ;
            /*diff 4*/
            currentStatus.deltaCycle = 4 ;
            break ;
        case 0x17 : {
            /* RLA */
            bool originalCarry = Get_CFlag() ;
            ( CPU_regs[ A ] & 0b10000000 ) == 0 ? Clear_CFlag() : Set_CFlag() ;
            CPU_regs[ A ] = ( CPU_regs[ A ] << 1 ) | originalCarry ;

            Clear_ZeroFlag() ;
            Clear_HCFlag() ;
            Clear_SubFlag() ;
            currentStatus.deltaCycle = 4 ;
        }   break ;
        case 0x1F : {
            /* RRA */
            bool originalCarry = Get_CFlag() ;
            ( CPU_regs[ A ] & 0b00000001 ) == 0  ? Clear_CFlag() : Set_CFlag() ;
            CPU_regs[ A ] = ( CPU_regs[ A ] >> 1 ) | ( originalCarry << 7 ) ;

            Clear_ZeroFlag() ;
            Clear_HCFlag() ;
            Clear_SubFlag() ;
            /*diff 4*/
            currentStatus.deltaCycle = 4 ;
        }   break ;
        /* General-Purpose Arithmetic Operations and CPU Control  */
        case 0x27 :
            /* DAA */
            if ( Get_SubFlag() ) {
                if ( Get_CFlag() )
                    CPU_regs[ A ] += 0x60 ;
                if ( Get_HCFlag() )
                    CPU_regs[ A ] += 0x06 ;
            } // if
            else {
                if ( Get_CFlag() || CPU_regs[ A ] > 0x99 ) {
                    CPU_regs[ A ] += 0x60 ;
                    Set_CFlag() ;
                } // if
                if ( Get_HCFlag() || ( CPU_regs[ A ] & 0x0f ) > 0x09 )
                    CPU_regs[ A ] += 0x06 ;
            }

            CPU_regs[ A ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            Clear_HCFlag() ;
            currentStatus.deltaCycle = 4 ;
            break ;
        case 0x37 :
            /* SCF */
            Set_CFlag() ;
            Clear_HCFlag() ;
            Clear_SubFlag() ;
            currentStatus.deltaCycle = 4 ;
            break ;
        case 0x2F :
            /* CPL */
            CPU_regs[ A ] = ~CPU_regs[ A ] ;
            Set_HCFlag() ;
            Set_SubFlag() ;
            currentStatus.deltaCycle = 4 ;
            break ;
        case 0x3F :
            /* CCF */
            Get_CFlag() ? Clear_CFlag() : Set_CFlag() ;
            Clear_HCFlag() ;
            Clear_SubFlag() ;
            currentStatus.deltaCycle = 4 ;
            break ;
        /* Jump */
        case 0xC3 : {
            /* JP nn */
            CPU_PC = Fetch_16bitByMMU( ++CPU_PC ) ;
            currentStatus.pc_jumping = true ;
            currentStatus.deltaCycle = 16;
        }   break ;
        case 0xC2 : case 0xCA : case 0xD2 : case 0xDA : {
            /* JP cc,  nn */
            if ( GetJumpCondition( opcode ) ) {
                CPU_PC = Fetch_16bitByMMU( ++CPU_PC ) ;
                currentStatus.pc_jumping = true ;
                /*diff 16*/
                currentStatus.deltaCycle = 16 ;
            } // if
            else {
                CPU_PC += 2 ;
                currentStatus.deltaCycle = 12 ;
            } // else

        }   break ;
        case 0x18 : {
            /* JR e */
            int8_t offset = static_cast<int8_t>( MEMREAD(CPU_PC + 1) ) + 2 ;
            CPU_PC += offset ;
            currentStatus.pc_jumping = true ;
            /*diff 12*/
            currentStatus.deltaCycle = 12 ;
        }
            break ;
        case 0x20 : case 0x28 : case 0x30 : case 0x38 : {
            /* JR cc, e */
            int8_t offset = static_cast<int8_t>( MEMREAD(CPU_PC + 1) ) + 2 ;
            if ( GetJumpCondition( opcode ) ) {
                CPU_PC += offset ;
                currentStatus.pc_jumping = true ;
                /*diff 12*/
                currentStatus.deltaCycle = 12 ;
            } // if
            else {
                ++ CPU_PC ;
                currentStatus.deltaCycle = 8 ;
            } // else

        }   break ;
        case 0xE9 :
            /* JP HL */
            CPU_PC = Get_HL() ;
            currentStatus.pc_jumping = true ;
            currentStatus.deltaCycle = 4 ;
            break ;
        /* Call and Return */
        case 0xCD : {
            /* CALL nn */
            uint16_t retPC = CPU_PC + 3 ;

            uint8_t* PC_highlow = reinterpret_cast<uint8_t*>( &( retPC ) ) ;
            MEMWRITE(--CPU_SP, PC_highlow[HIGH]) ;
            MEMWRITE(--CPU_SP, PC_highlow[LOW]) ;

            CPU_PC = MEMREAD( ++CPU_PC ) | ( MEMREAD( ++CPU_PC ) << 8 ) ;

            currentStatus.pc_jumping = true ;
            currentStatus.deltaCycle = 24 ;
        }   break ;
        case 0xC4 : case 0xCC : case 0xD4 : case 0xDC : {
            /* CALL cc, nn */
            bool condition = false ;
            switch ( opcode & 0b00011000 ) {
                case 0x00 :
                    condition = !Get_ZeroFlag() ;
                    break ;
                case 0x08 :
                    condition = Get_ZeroFlag() ;
                    break ;
                case 0x10 :
                    condition = !Get_CFlag() ;
                    break ;
                case 0x18 :
                    condition = Get_CFlag() ;
                    break ;
            } // switch

            if ( condition ) {
                uint16_t retPC = CPU_PC + 3 ;
                uint8_t* PC_highlow = reinterpret_cast<uint8_t*>( &( retPC ) ) ;
                MEMWRITE(--CPU_SP, PC_highlow[HIGH]) ;
                MEMWRITE(--CPU_SP, PC_highlow[LOW]) ;
                CPU_PC = MEMREAD( ++CPU_PC ) | ( MEMREAD( ++CPU_PC ) << 8 ) ;
                currentStatus.pc_jumping = true ;
                /*diff 24*/
                currentStatus.deltaCycle = 24 ;
            } // if
            else {
                CPU_PC += 2 ;
                currentStatus.deltaCycle = 12 ;
            } // else

        }   break ;
        case 0xC9 : {
            /* RET */
            uint8_t* PC_highlow = reinterpret_cast<uint8_t*>( &( CPU_PC ) ) ;
            PC_highlow[ HIGH ] = MEMREAD(CPU_SP + 1) ;
            PC_highlow[ LOW ] = MEMREAD(CPU_SP) ;
            CPU_SP += 2 ;
            currentStatus.pc_jumping = true ;
            /*diff 16*/
            currentStatus.deltaCycle = 16 ;
        }   break ;
        case 0xD9 : {
            /* RETI */
            currentStatus.IME = true ; // same as EI
            uint8_t* PC_highlow = reinterpret_cast<uint8_t*>( &( CPU_PC ) ) ;
            PC_highlow[ HIGH ] = MEMREAD(CPU_SP + 1) ;
            PC_highlow[ LOW ] = MEMREAD(CPU_SP) ;
            CPU_SP += 2 ;
            currentStatus.pc_jumping = true ;
            /*diff 16*/
            currentStatus.deltaCycle = 16 ;
        }   break ;
        case 0xC0 : case 0xC8 : case 0xD0 : case 0xD8 : {
            /* RET cc */
            bool condition = false ;
            switch ( opcode & 0b00011000 ) {
                case 0x00 :
                    condition = !Get_ZeroFlag() ;
                    break ;
                case 0x08 :
                    condition = Get_ZeroFlag() ;
                    break ;
                case 0x10 :
                    condition = !Get_CFlag() ;
                    break ;
                case 0x18 :
                    condition = Get_CFlag() ;
                    break ;
            } // switch

            if ( condition ) {
                uint8_t* PC_highlow = reinterpret_cast<uint8_t*>( &( CPU_PC ) ) ;
                PC_highlow[ HIGH ] = MEMREAD(CPU_SP + 1) ;
                PC_highlow[ LOW ] = MEMREAD(CPU_SP) ;
                CPU_SP += 2 ;
                currentStatus.pc_jumping = true ;
                /*diff 20*/
                currentStatus.deltaCycle = 20 ;
            } // if
            else
                currentStatus.deltaCycle = 8 ;

        }   break ;
        case 0xC7 : case 0xCF : case 0xD7 : case 0xDF : case 0xE7 : case 0xEF : case 0xF7 : case 0xFF : {
            /* RST t */
            uint16_t retPC = CPU_PC + 1 ;
            uint8_t* PC_highlow = reinterpret_cast<uint8_t*>( &( retPC ) ) ;
            MEMWRITE(--CPU_SP, PC_highlow[HIGH]) ;
            MEMWRITE(--CPU_SP, PC_highlow[LOW]) ;
            CPU_PC = 0x0 + ( (opcode & 0b00111000) >> 3 ) * 8 ;
            currentStatus.pc_jumping = true ;
            /*diff 16*/
            currentStatus.deltaCycle = 16 ;
        }   break ;
        case 0xF3 :
            /* DI */
            IME_delay = -2 ;
            currentStatus.deltaCycle = 4 ;
            break ;
        case 0xFB :
            /* EI */
            IME_delay = 2 ;
            currentStatus.deltaCycle = 4 ;
            break ;
        case 0x10 :
            break ;
        case 0x76 :
            halting = true ;
            currentStatus.pc_jumping = true ;
            currentStatus.deltaCycle = 4 ;
            ++CPU_PC ;
            break ;
        case 0xCB :
            RunExtendInstruction( MEMREAD(++CPU_PC) ) ;
            // currentStatus.deltaCycle += 4 ;
            /*According to manual, 0xcb has no cycle*/
            break ;
        default:
            printf( "PC=0x%x Unknown opcode: 0x%x\n", pc, opcode ) ;
            exit( UNKNOWN_OPCODE ) ;
    } // switch

    if ( currentStatus.pc_jumping )
        currentStatus.pc_jumping = false ;
    else
        ++ CPU_PC ;

    if ( IME_delay < 0 ) {
        ++ IME_delay ;
        if ( IME_delay == 0 )
            currentStatus.IME = false ;
    } // if
    else if ( IME_delay > 0 ) {
        -- IME_delay ;
        if ( IME_delay == 0 )
            currentStatus.IME = true ;
    } // else if
#ifdef DEBUG
   // printf( "(%d)\n", currentStatus.deltaCycle ) ;
/*
    printf( "IME=%d", currentStatus.IME ) ;
    printf( " LCDC=%x STAT=%x LY=%x IF=%x IE=%x ",
            MAINMEM[ LCDC ], MAINMEM[ STAT ], MAINMEM[ LY ], MAINMEM[ 0xFF0F ], MAINMEM[ 0xFFFF ]  ) ;
    printf( "%d\n", currentStatus.deltaCycle ) ;
    */
#endif
}

void LR35902::RunExtendInstruction(uint8_t opcode) {
#ifdef DEBUG
    printf( "PC=%x opcode=%x A=%x B=%x C=%x D=%x E=%x H=%x L=%x SP=%x Z:%d N:%d H:%d C:%d\n", currentStatus.PC, opcode
            , CPU_regs[ A ]
            , CPU_regs[ B ]
            , CPU_regs[ C ]
            , CPU_regs[ D ]
            , CPU_regs[ E ]
            , CPU_regs[ H ]
            , CPU_regs[ L ]
            , CPU_SP
            , Get_ZeroFlag()
            , Get_SubFlag()
            , Get_HCFlag()
            , Get_CFlag()
    ) ;
#endif
    switch ( opcode ) {
        case 0x40 : case 0x41 : case 0x42 : case 0x43 : case 0x44 : case 0x45 : case 0x47 :
        case 0x48 : case 0x49 : case 0x4A : case 0x4B : case 0x4C : case 0x4D : case 0x4F :
        case 0x50 : case 0x51 : case 0x52 : case 0x53 : case 0x54 : case 0x55 : case 0x57 :
        case 0x58 : case 0x59 : case 0x5A : case 0x5B : case 0x5C : case 0x5D : case 0x5F :
        case 0x60 : case 0x61 : case 0x62 : case 0x63 : case 0x64 : case 0x65 : case 0x67 :
        case 0x68 : case 0x69 : case 0x6A : case 0x6B : case 0x6C : case 0x6D : case 0x6F :
        case 0x70 : case 0x71 : case 0x72 : case 0x73 : case 0x74 : case 0x75 : case 0x77 :
        case 0x78 : case 0x79 : case 0x7A : case 0x7B : case 0x7C : case 0x7D : case 0x7F :
        case 0x46 : case 0x4E : case 0x56 : case 0x5E : case 0x66 : case 0x6E : case 0x76 : case 0x7E :
        {
            /* BIT b, r */
            uint8_t targetBit = ( opcode & 0b00111000 ) >> 3 ;

            bool isHL = ( opcode & 0b00000111 ) == 0x6 ;

            uint8_t target = isHL ? MEMREAD( Get_HL() ) :
                    currentStatus.regs[ opcode & 0b00000111 ] ;
            uint8_t mask = 0b00000001 << targetBit ;

            Set_HCFlag() ;
            Clear_SubFlag() ;

            ( target & mask ) == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            currentStatus.deltaCycle = isHL ? 16 : 8 ; /* offical->12 */
        }
            break ;
        case 0x00 : case 0x01 : case 0x02 : case 0x03 : case 0x04 : case 0x05 : case 0x07 :
        {
            /* RLC r */

            uint8_t regNum = opcode & 0b00000111 ;

            ( currentStatus.regs[ regNum ] & 0b10000000 ) == 0 ? Clear_CFlag() : Set_CFlag() ;

            currentStatus.regs[ regNum ] =
                    ( currentStatus.regs[ regNum ] << 1 ) | ( currentStatus.regs[ regNum ] >> 7 ) ;

            Clear_HCFlag() ;
            Clear_SubFlag() ;
            currentStatus.regs[ regNum ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            currentStatus.deltaCycle = 8 ;
        }
            break ;
        case 0x06 : {
            /* RLC (HL) */
            uint8_t originalValue = MEMREAD( Get_HL() ) ;
            ( originalValue & 0b10000000 ) == 0 ? Clear_CFlag() : Set_CFlag() ;

            uint8_t newValue = ( originalValue << 1 ) | ( originalValue >> 7 ) ;
            MEMWRITE( Get_HL(), newValue ) ;

            Clear_HCFlag() ;
            Clear_SubFlag() ;
            newValue == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            currentStatus.deltaCycle = 16 ;
        }
            break ;
        case 0x08 : case 0x09 : case 0x0A : case 0x0B : case 0x0C : case 0x0D : case 0x0F :
        {
            /* RRC r */
            uint8_t regNum = opcode & 0b00000111 ;
            currentStatus.regs[ regNum ] =
                    ( currentStatus.regs[ regNum ] >> 1 ) | ( currentStatus.regs[ regNum ] << 7 ) ;

            ( currentStatus.regs[ regNum ] & 0b10000000 ) == 0 ? Clear_CFlag() : Set_CFlag() ;

            Clear_HCFlag() ;
            Clear_SubFlag() ;
            currentStatus.regs[ regNum ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            currentStatus.deltaCycle = 8 ;
        }
            break ;
        case 0x0E : {
            /* RRC (HL) */
            uint8_t originalValue = MEMREAD( Get_HL() ) ;
            uint8_t newValue = ( originalValue << 7 ) | ( originalValue >> 1 ) ;
            MEMWRITE( Get_HL(), newValue ) ;

            ( newValue & 0b10000000 ) == 0 ? Clear_CFlag() : Set_CFlag() ;

            Clear_HCFlag() ;
            Clear_SubFlag() ;
            newValue == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            currentStatus.deltaCycle = 16 ;
        }
            break ;
        case 0x10 : case 0x11 : case 0x12 : case 0x13 : case 0x14 : case 0x15 : case 0x17 :
        {
            /* RL r */
            uint8_t regNum = opcode & 0b00000111 ;
            uint8_t oldRegVal = currentStatus.regs[ regNum ] ;
            currentStatus.regs[ regNum ] = ( currentStatus.regs[ regNum ] << 1 ) | Get_CFlag() ;

            if ( ( oldRegVal & 0b10000000 ) == 0 ) {
                Clear_CFlag() ;
            } // if
            else {
                Set_CFlag() ;
            } // else

            currentStatus.regs[ regNum ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            Clear_HCFlag() ;
            Clear_SubFlag() ;

            currentStatus.deltaCycle = 8 ;
        }
            break ;
        case 0x16 : {
            /* RL (HL) */
            uint8_t originalValue = MEMREAD( Get_HL() ) ;
            uint8_t newValue = ( originalValue << 1 ) | Get_CFlag() ;
            MEMWRITE( Get_HL(), newValue ) ;

            if ( ( originalValue & 0b10000000 ) == 0 ) {
                Clear_CFlag() ;
            } // if
            else {
                Set_CFlag() ;
            } // else

            newValue == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            Clear_HCFlag() ;
            Clear_SubFlag() ;

            currentStatus.deltaCycle = 16 ;
        }
            break ;
        case 0x18 : case 0x19 : case 0x1A : case 0x1B : case 0x1C : case 0x1D : case 0x1F :
        {
            /* RR r */
            uint8_t regNum = opcode & 0b00000111 ;
            uint8_t oldRegVal = currentStatus.regs[ regNum ] ;
            currentStatus.regs[ regNum ] = ( currentStatus.regs[ regNum ] >> 1 )
                    | ( Get_CFlag() << 7 ) ;

            if ( ( oldRegVal & 0b00000001 ) == 0 ) {
                Clear_CFlag() ;
            } // if
            else {
                Set_CFlag() ;
            } // else

            currentStatus.regs[ regNum ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            Clear_HCFlag() ;
            Clear_SubFlag() ;

            /*diff 8*/
            currentStatus.deltaCycle = 8 ;
        }
            break ;
        case 0x1E : {
            /* RR (HL) */
            uint8_t originalValue = MEMREAD( Get_HL() ) ;
            uint8_t newValue = (originalValue >> 1) | (Get_CFlag() << 7) ;
            MEMWRITE( Get_HL(), newValue ) ;

            if ( ( originalValue & 0b00000001 ) == 0 ) {
                Clear_CFlag() ;
            } // if
            else {
                Set_CFlag() ;
            } // else

            newValue == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;

            Clear_HCFlag() ;
            Clear_SubFlag() ;

            currentStatus.deltaCycle = 16 ;
        }
            break ;
        case 0x20 : case 0x21 : case 0x22 : case 0x23 : case 0x24 : case 0x25 : case 0x27 : {
            /* SLA r */
            uint8_t regNum = opcode & 0b00000111 ;
            ( currentStatus.regs[ regNum ] & 0b10000000 ) == 0 ? Clear_CFlag() : Set_CFlag() ;
            currentStatus.regs[ regNum ] <<= 1 ;
            currentStatus.regs[ regNum ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            Clear_HCFlag() ;
            Clear_SubFlag() ;
            currentStatus.deltaCycle = 8 ;
        }
            break ;
        case 0x26 : {
            /* SLA (HL) */
            uint8_t originalValue = MEMREAD( Get_HL() ) ;
            ( originalValue & 0b10000000 ) == 0 ? Clear_CFlag() : Set_CFlag() ;
            uint8_t newValue = originalValue << 1 ;

            MEMWRITE( Get_HL(), newValue ) ;
            newValue == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            Clear_HCFlag() ;
            Clear_SubFlag() ;
            currentStatus.deltaCycle = 16 ;
        }
            break ;
        case 0x28 : case 0x29 : case 0x2A : case 0x2B : case 0x2C : case 0x2D : case 0x2F :
        {
            /* SRA r */
            uint8_t regNum = opcode & 0b00000111 ;
            uint8_t oldVal = currentStatus.regs[ regNum ] ;
            currentStatus.regs[ regNum ] = ( oldVal >> 1 ) | ( oldVal & 0b10000000 ) ;

            oldVal & 0b00000001 ? Set_CFlag() : Clear_CFlag() ;
            currentStatus.regs[ regNum ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            Clear_HCFlag() ;
            Clear_SubFlag() ;
            currentStatus.deltaCycle = 8 ;
        }
            break ;
        case 0x2E : {
            /* SRA (HL) */
            uint8_t originalValue = MEMREAD( Get_HL() ) ;
            uint8_t newValue = (originalValue >> 1) | (originalValue & 0b10000000);
            MEMWRITE( Get_HL(), newValue ) ;

            originalValue & 0b00000001 ? Set_CFlag() : Clear_CFlag() ;
            newValue == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            Clear_HCFlag() ;
            Clear_SubFlag() ;
            currentStatus.deltaCycle = 16 ;
        }
            break ;
        case 0x30 : case 0x31 : case 0x32 : case 0x33 : case 0x34 : case 0x35 : case 0x37 : {
            /* SWAP r */
            uint8_t regNum = opcode & 0b00000111 ;
            uint8_t originalValue = currentStatus.regs[ regNum ] ;
            currentStatus.regs[ regNum ] = originalValue << 4 | originalValue >> 4 ;
            Clear_CFlag() ;
            Clear_HCFlag() ;
            Clear_SubFlag() ;
            currentStatus.regs[ regNum ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            currentStatus.deltaCycle = 8 ;
        }
            break ;
        case 0x36 : {
            /* SWAP (HL) */
            uint8_t originalValue = MEMREAD( Get_HL() ) ;
            uint8_t newValue = originalValue << 4 | originalValue >> 4 ;
            MEMWRITE( Get_HL(), newValue ) ;
            Clear_CFlag() ;
            Clear_HCFlag() ;
            Clear_SubFlag() ;
            newValue == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            currentStatus.deltaCycle = 16 ;
        }
            break ;
        case 0x38 : case 0x39 : case 0x3A : case 0x3B : case 0x3C : case 0x3D : case 0x3F : {
            /* SRL r */
            uint8_t regNum = opcode & 0b00000111 ;
            currentStatus.regs[ regNum ] & 0b00000001 ? Set_CFlag() : Clear_CFlag() ;
            currentStatus.regs[ regNum ] >>= 1  ;
            currentStatus.regs[ regNum ] == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            Clear_HCFlag() ;
            Clear_SubFlag() ;
            currentStatus.deltaCycle = 8 ;
        }
            break ;
        case 0x3E : {
            /* SRL (HL) */
            uint8_t originalValue = MEMREAD( Get_HL() ) ;
            uint8_t newValue = (originalValue >> 1);
            MEMWRITE( Get_HL(), newValue ) ;

            originalValue & 0b00000001 ? Set_CFlag() : Clear_CFlag() ;
            newValue == 0 ? Set_ZeroFlag() : Clear_ZeroFlag() ;
            Clear_HCFlag() ;
            Clear_SubFlag() ;
            currentStatus.deltaCycle = 16 ;
        }
            break ;
        case 0x80 : case 0x81 : case 0x82 : case 0x83 : case 0x84 : case 0x85 : case 0x87 :
        case 0x88 : case 0x89 : case 0x8A : case 0x8B : case 0x8C : case 0x8D : case 0x8F :
        case 0x90 : case 0x91 : case 0x92 : case 0x93 : case 0x94 : case 0x95 : case 0x97 :
        case 0x98 : case 0x99 : case 0x9A : case 0x9B : case 0x9C : case 0x9D : case 0x9F :
        case 0xA0 : case 0xA1 : case 0xA2 : case 0xA3 : case 0xA4 : case 0xA5 : case 0xA7 :
        case 0xA8 : case 0xA9 : case 0xAA : case 0xAB : case 0xAC : case 0xAD : case 0xAF :
        case 0xB0 : case 0xB1 : case 0xB2 : case 0xB3 : case 0xB4 : case 0xB5 : case 0xB7 :
        case 0xB8 : case 0xB9 : case 0xBA : case 0xBB : case 0xBC : case 0xBD : case 0xBF :
        {
            /* RES b, r */
            uint8_t mask = ~( 1 << ( (opcode & 0b00111000) >> 3 ) ) ;
            uint8_t regNum = opcode & 0b00000111 ;

            currentStatus.regs[ regNum ] &= mask ;
            currentStatus.deltaCycle = 8 ;
        }
            break ;
        case 0x86 : case 0x8E : case 0x96 : case 0x9E : case 0xA6 : case 0xAE : case 0xB6 : case 0xBE : {
            /* RES (HL) */
            uint8_t mask = ~( 1 << ( (opcode & 0b00111000) >> 3 ) ) ;
            uint8_t memValue = MEMREAD( Get_HL() ) ;
            memValue &= mask ;
            MEMWRITE( Get_HL(), memValue ) ;
            currentStatus.deltaCycle = 16 ;
        }
            break ;
        case 0xC0 : case 0xC1 : case 0xC2 : case 0xC3 : case 0xC4 : case 0xC5 : case 0xC7 :
        case 0xC8 : case 0xC9 : case 0xCA : case 0xCB : case 0xCC : case 0xCD : case 0xCF :
        case 0xD0 : case 0xD1 : case 0xD2 : case 0xD3 : case 0xD4 : case 0xD5 : case 0xD7 :
        case 0xD8 : case 0xD9 : case 0xDA : case 0xDB : case 0xDC : case 0xDD : case 0xDF :
        case 0xE0 : case 0xE1 : case 0xE2 : case 0xE3 : case 0xE4 : case 0xE5 : case 0xE7 :
        case 0xE8 : case 0xE9 : case 0xEA : case 0xEB : case 0xEC : case 0xED : case 0xEF :
        case 0xF0 : case 0xF1 : case 0xF2 : case 0xF3 : case 0xF4 : case 0xF5 : case 0xF7 :
        case 0xF8 : case 0xF9 : case 0xFA : case 0xFB : case 0xFC : case 0xFD : case 0xFF :
        {
            /* SET b, r */
            uint8_t mask = ( 1 << ( (opcode & 0b00111000) >> 3 ) ) ;
            uint8_t regNum = opcode & 0b00000111 ;

            currentStatus.regs[ regNum ] |= mask ;
            currentStatus.deltaCycle = 8 ;
        }
            break ;
        case 0xC6 : case 0xCE : case 0xD6 : case 0xDE : case 0xE6 : case 0xEE : case 0xF6 : case 0xFE : {
            /* SET (HL) */
            uint8_t mask = ( 1 << ( (opcode & 0b00111000) >> 3 ) ) ;
            uint8_t memValue = MEMREAD( Get_HL() ) ;
            memValue |= mask ;
            MEMWRITE( Get_HL(), memValue ) ;
            currentStatus.deltaCycle = 16 ;
        }
            break ;
        default:
            printf( "Unknown opcode(in extend): 0x%x at 0x%x\n", opcode, CPU_PC ) ;
            exit( UNKNOWN_OPCODE ) ;
    } // switch
}

void LR35902::CheckInterrupts() {
    if ( currentStatus.IME ) {
        uint8_t* mainMem = _mmu.getMainMemory() ;
        uint8_t irtF = mainMem[ IF ] ;

        if ( irtF == 0 )
            return ;

        uint8_t irtE = mainMem[ IE ] ;
        for ( int i = 0 ; i < 5 ; ++i ) {
            if ( TEST_BIT( irtF, i ) && TEST_BIT( irtE, i ) )
                DoInterrupt( i ) ;
        } // for
    } // if
}

void LR35902::DoInterrupt(uint8_t irtNum) {
    currentStatus.IME = false ;
    halting = false ;
    uint8_t& irtF = _mmu.getMainMemory() [ IF ] ;

    irtF &= ~( 1 << irtNum ) ;
    uint16_t nextPC = CPU_PC ;
    MEMWRITE(--CPU_SP, ( nextPC & 0xFF00 ) >> 8 ) ;
    MEMWRITE(--CPU_SP, nextPC & 0x00FF ) ;

    switch ( irtNum ) {
        case 0: CPU_PC = 0x40 ; break ;
        case 1: CPU_PC = 0x48 ; break ;
        case 2: CPU_PC = 0x50 ; break ;
        case 4: CPU_PC = 0x60 ; break ;
    } // Switch
}

void LR35902::ResumeFromState(const CPU_status cs) {
    memcpy( currentStatus.regs, cs.regs, sizeof( uint8_t ) * 8 ) ;
    currentStatus.SP = cs.SP ;
    currentStatus.PC = cs.PC ;
    currentStatus.IME = cs.IME ;
    currentStatus.deltaCycle = cs.deltaCycle ;
}

LR35902::~LR35902() {
    //dtor
}
