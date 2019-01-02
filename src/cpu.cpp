#include "cpu.h"

#include <LR35902_All.h>
#include <cstring>
#include <cpu_state.h>

using namespace lr35902 ;

LR35902::LR35902() {
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

    memory = new uint8_t[ 0x10000 ]() ;

    memory[ TIMA ] = 0x00 ;
    memory[ TMA ] = 0x00 ;
    memory[ TAC ] = 0x00 ;
    memory[ IF ] = 0xe0 ; // from reddit/emudev
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

void LR35902::ExecuteCurrentInstruction() {
    uint8_t opcode = memory[ CPU_PC ] ;
    switch ( opcode ) {
        /* 8-Bit Transfer and Input/Output  */
        case 0x40 : case 0x41 : case 0x42 : case 0x43 : case 0x44 : case 0x45 : case 0x47 :
        case 0x48 : case 0x49 : case 0x4A : case 0x4B : case 0x4C : case 0x4D : case 0x4F :
        case 0x50 : case 0x51 : case 0x52 : case 0x53 : case 0x54 : case 0x55 : case 0x57 :
        case 0x58 : case 0x59 : case 0x5A : case 0x5B : case 0x5C : case 0x5D : case 0x5F :
        case 0x60 : case 0x61 : case 0x62 : case 0x63 : case 0x64 : case 0x65 : case 0x67 :
        case 0x68 : case 0x69 : case 0x6A : case 0x6B : case 0x6C : case 0x6D : case 0x6F :
        case 0x78 : case 0x79 : case 0x7A : case 0x7B : case 0x7C : case 0x7D : case 0x7F :
            CPU_regs[ ( opcode & 0b00111000 ) >> 3 ] = CPU_regs[ ( opcode & 0b00000111 ) ] ;
            CPU_obj->currentStatus.deltaCycle += 4 ;
            break ;
        case 0x06 : case 0x0E : case 0x16 : case 0x1E : case 0x26 : case 0x2E : case 0x3E :
            currentStatus.regs[ ( opcode & 0b00111000 ) >> 3 ] = memory[ ++CPU_PC ] ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0x46 : case 0x4E : case 0x56 : case 0x5E : case 0x66 : case 0x6E : case 0x7E :
            CPU_regs[ ( opcode & 0b00111000 ) >> 3 ] = CPU_obj->memory[ CPU_obj->Get_HL() ] ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0x70 : case 0x71 : case 0x72 : case 0x73 : case 0x74 : case 0x75 : case 0x77 :
            CPU_obj->memory[ CPU_obj->Get_HL() ] = CPU_regs[ ( opcode & 0b00000111 ) ] ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0x36 :
            CPU_obj->memory[ CPU_obj->Get_HL() ] = CPU_obj->memory[ ++(CPU_PC) ] ; // pc-->imm8
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0x0A :
            CPU_regs[ A ] = CPU_obj->memory[ CPU_obj->Get_BC() ] ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0x1A :
            CPU_regs[ A ] = CPU_obj->memory[ CPU_obj->Get_DE() ] ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0xF2 :
            CPU_regs[ A ] = CPU_obj->memory[ 0xFF00 + CPU_regs[ C ] ] ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0xE2 :
            CPU_obj->memory[ 0xFF00 + CPU_regs[ C ] ] = CPU_regs[ A ] ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0xF0 :
            CPU_regs[ A ] = CPU_obj->memory[ 0xFF00 + CPU_obj->memory[ ++( CPU_PC ) ] ] ;
            CPU_obj->currentStatus.deltaCycle += 12 ;
            break ;
        case 0xE0 :
            CPU_obj->memory[ 0xFF00 + CPU_obj->memory[ ++( CPU_PC ) ] ] = CPU_regs[ A ] ;
            CPU_obj->currentStatus.deltaCycle += 12 ;
            break ;
        case 0xFA : {
            uint16_t* addr = reinterpret_cast<uint16_t*>( CPU_obj->memory + ++( CPU_PC ) ) ;
            CPU_regs[ A ] = CPU_obj->memory[ *addr ] ;
            ++( CPU_PC ) ;
            CPU_obj->currentStatus.deltaCycle += 16 ;
        }   break ;
        case 0xEA : {
            uint16_t* addr = reinterpret_cast<uint16_t*>( CPU_obj->memory + ++( CPU_PC ) ) ;
            CPU_obj->memory[ *addr ] = CPU_regs[ A ] ;
            ++( CPU_PC ) ;
            CPU_obj->currentStatus.deltaCycle += 16 ;
        }   break ;
        case 0x2A :
            CPU_regs[ A ] = CPU_obj->memory[ CPU_obj->Get_HL() ] ;
            CPU_obj->Set_HL(static_cast<uint16_t>(CPU_obj->Get_HL() + 1)) ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0x3A :
            CPU_regs[ A ] = CPU_obj->memory[ CPU_obj->Get_HL() ] ;
            CPU_obj->Set_HL(static_cast<uint16_t>(CPU_obj->Get_HL() - 1)) ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0x02 :
            CPU_obj->memory[ CPU_obj->Get_BC() ] = CPU_regs[ A ] ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0x12 :
            CPU_obj->memory[ CPU_obj->Get_DE() ] = CPU_regs[ A ] ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0x22 :
            CPU_obj->memory[ CPU_obj->Get_HL() ] = CPU_regs[ A ] ;
            CPU_obj->Set_HL( CPU_obj->Get_HL() + 1 ) ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0x32 :
            CPU_obj->memory[ CPU_obj->Get_HL() ] = CPU_regs[ A ] ;
            CPU_obj->Set_HL( CPU_obj->Get_HL() - 1 ) ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;

        /* 16-Bit Transfer Instructions */
        case 0x01 : case 0x11 : case 0x21 : case 0x31 : {
            uint16_t *value = reinterpret_cast<uint16_t *>( CPU_obj->memory + ++(CPU_PC));

            switch ( opcode & 0b00110000 ) {
                case 0b00000000 :
                    CPU_obj->Set_BC(*value);
                    break;
                case 0b00010000 :
                    CPU_obj->Set_DE(*value);
                    break;
                case 0b00100000 :
                    CPU_obj->Set_HL(*value);
                    break;
                case 0b00110000 :
                    CPU_SP = *value;
                    break;
            } // switch

            ++(CPU_PC);
            CPU_obj->currentStatus.deltaCycle += 12;
        }   break ;
        case 0xF9 :
            CPU_SP = CPU_obj->Get_HL() ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0xC5 :
            memory[ --CPU_SP ] = CPU_regs[ B ] ;
            memory[ --CPU_SP ] = CPU_regs[ C ] ;
            CPU_obj->currentStatus.deltaCycle += 16 ;
            break;
        case 0xD5 :
            memory[ --CPU_SP ] = CPU_regs[ D ] ;
            memory[ --CPU_SP ] = CPU_regs[ E ] ;
            CPU_obj->currentStatus.deltaCycle += 16 ;
            break;
        case 0xE5 :
            memory[ --CPU_SP ] = CPU_regs[ H ] ;
            memory[ --CPU_SP ] = CPU_regs[ L ] ;
            CPU_obj->currentStatus.deltaCycle += 16 ;
            break;
        case 0xF5 :
            memory[ --CPU_SP ] = CPU_regs[ A ] ;
            memory[ --CPU_SP ] = CPU_regs[ F ] ;
            CPU_obj->currentStatus.deltaCycle += 16 ;
            break;
        case 0xC1 :
            CPU_regs[ C ] = CPU_obj->memory[ CPU_SP ] ;
            CPU_regs[ B ] = CPU_obj->memory[ ++CPU_SP ] ;
            ++CPU_SP ;
            CPU_obj->currentStatus.deltaCycle += 12 ;
            break;
        case 0xD1 :
            CPU_regs[ E ] = CPU_obj->memory[ CPU_SP ] ;
            CPU_regs[ D ] = CPU_obj->memory[ ++CPU_SP ] ;
            CPU_obj->currentStatus.deltaCycle += 12 ;
            ++CPU_SP ;
            break;
        case 0xE1 :
            CPU_regs[ L ] = CPU_obj->memory[ CPU_SP ] ;
            CPU_regs[ H ] = CPU_obj->memory[ ++CPU_SP ] ;
            CPU_obj->currentStatus.deltaCycle += 12 ;
            ++CPU_SP ;
            break;
        case 0xF1 :
            CPU_regs[ F ] = CPU_obj->memory[ CPU_SP ] ;
            CPU_regs[ A ] = CPU_obj->memory[ ++CPU_SP ] ;
            CPU_obj->currentStatus.deltaCycle += 12 ;
            ++CPU_SP ;
            break;
        case 0xF8 : {
            uint16_t result = CPU_SP + CPU_obj->memory[ ++( CPU_PC ) ] ;

            if ( result > 0b00001111 ) {
                CPU_obj->Set_HCFlag() ;
            } // if
            if ( result > 0b11111111 ) {
                CPU_obj->Set_CFlag() ;
            } // if

            CPU_obj->Set_HL( result ) ;
            CPU_obj->currentStatus.deltaCycle += 12 ;
        }   break ;
        case 0x08 : {
            uint16_t* addr = (uint16_t*)( CPU_obj->memory + ++( CPU_PC ) ) ;
            *( (uint16_t*)(CPU_obj->memory) + *addr ) = CPU_SP ;
            ++( CPU_PC ) ;
            CPU_obj->currentStatus.deltaCycle += 20 ;
        }   break ;

        /* 8-Bit Arithmetic and Logical Operation */
        case 0x80 : case 0x81 : case 0x82 : case 0x83 : case 0x84 : case 0x85 : case 0x87 : {
            /* ADD A, r */
            uint8_t tempReg = CPU_regs[ A ] ;
            uint8_t regIndex = opcode & 0b00000111 ;
            if ( ( (tempReg & 0x0f) + (CPU_regs[ regIndex ] & 0x0f) & 0x10 ) == 0x10 )
                CPU_obj->Set_HCFlag() ;
            else
                CPU_obj->Clear_HCFlag() ;

            CPU_regs[ A ] += CPU_regs[ regIndex ] ;

            CPU_regs[ A ] < tempReg ? CPU_obj->Set_CFlag() : CPU_obj->Clear_CFlag() ;
            CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

            CPU_obj->Clear_SubFlag();
            CPU_obj->currentStatus.deltaCycle += 4 ;
        }   break ;
        case 0xC6 : {
            /* ADD A, imm8 */
            uint8_t tempReg = CPU_regs[ A ] ;
            if ( ( (tempReg & 0x0f) + (CPU_obj->memory[ CPU_PC + 1 ] & 0x0f) & 0x10 ) == 0x10 )
                CPU_obj->Set_HCFlag() ;
            else
                CPU_obj->Clear_HCFlag() ;

            CPU_regs[ A ] += CPU_obj->memory[ ++(CPU_PC) ] ;

            CPU_regs[ A ] < tempReg ? CPU_obj->Set_CFlag() : CPU_obj->Clear_CFlag() ;
            CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

            CPU_obj->Clear_SubFlag();
            CPU_obj->currentStatus.deltaCycle += 8 ;
        }   break ;
        case 0x86 : {
            /* ADD A, (HL) */
            uint8_t tempReg = CPU_regs[ A ] ;
            if ( ( (tempReg & 0x0f) + (CPU_obj->memory[ CPU_obj->Get_HL() ] & 0x0f) & 0x10 ) == 0x10 )
                CPU_obj->Set_HCFlag() ;
            else
                CPU_obj->Clear_HCFlag() ;

            CPU_regs[ A ] += CPU_obj->memory[ CPU_obj->Get_HL() ] ;

            CPU_regs[ A ] < tempReg ? CPU_obj->Set_CFlag() : CPU_obj->Clear_CFlag() ;
            CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

            CPU_obj->Clear_SubFlag() ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
        }   break ;
        case 0x88 : case 0x89 : case 0x8A : case 0x8B : case 0x8C : case 0x8D : case 0x8F : {
            /* ADC A, r */
            uint8_t tempReg = CPU_regs[ A ] ;
            uint8_t regIndex = opcode & 0b00000111 ;
            if (((tempReg & 0x0f) + (CPU_regs[ regIndex ] & 0x0f) + CPU_obj->Get_CFlag() & 0x10 ) == 0x10)
                CPU_obj->Set_HCFlag() ;
            else
                CPU_obj->Clear_HCFlag() ;

            CPU_regs[ A ] = CPU_regs[ A ] + CPU_regs[ regIndex ] + CPU_obj->Get_CFlag() ;

            CPU_regs[ A ] <= tempReg ? CPU_obj->Set_CFlag() : CPU_obj->Clear_CFlag() ;
            CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

            CPU_obj->Clear_SubFlag();
            CPU_obj->currentStatus.deltaCycle += 4 ;
        }   break ;
        case 0xCE : {
            /* ADC A, imm8 */
            uint8_t tempReg = CPU_regs[ A ] ;
            uint8_t immVal = CPU_obj->memory[ ++CPU_PC ] ;

            if ( ( (tempReg & 0x0f) + (immVal & 0x0f) + CPU_obj->Get_CFlag() & 0x10 ) == 0x10 )
                CPU_obj->Set_HCFlag() ;
            else
                CPU_obj->Clear_HCFlag() ;

            CPU_regs[ A ] = CPU_regs[ A ] + immVal + CPU_obj->Get_CFlag() ;

            CPU_regs[ A ] <= tempReg ? CPU_obj->Set_CFlag() : CPU_obj->Clear_CFlag() ;
            CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

            CPU_obj->Clear_SubFlag();
            CPU_obj->currentStatus.deltaCycle += 8 ;
        }   break ;
        case 0x8E : {
            /* ADC A, (HL) */
            uint8_t tempReg = CPU_regs[ A ] ;
            if ( ( (tempReg & 0x0f) + (CPU_obj->memory[ CPU_obj->Get_HL() ] & 0x0f) + CPU_obj->Get_CFlag() & 0x10 ) == 0x10 )
                CPU_obj->Set_HCFlag() ;
            else
                CPU_obj->Clear_HCFlag() ;

            CPU_regs[ A ] = CPU_regs[ A ] + CPU_obj->memory[ CPU_obj->Get_HL() ] + CPU_obj->Get_CFlag() ;

            CPU_regs[ A ] <= tempReg ? CPU_obj->Set_CFlag() : CPU_obj->Clear_CFlag() ;
            CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

            CPU_obj->Clear_SubFlag();
            CPU_obj->currentStatus.deltaCycle += 8 ;
        }   break ;
        case 0x90 : case 0x91 : case 0x92 : case 0x93 : case 0x94 : case 0x95 : case 0x97 : {
            /* SUB r */
            uint8_t tempReg = CPU_regs[ A ] ;
            uint8_t regIndex = static_cast<uint8_t> ( opcode & 0b00000111 ) ;

            CPU_regs[ A ] -= CPU_regs[ regIndex ] ;

            (tempReg & 0x0f) < (CPU_regs[ regIndex ] & 0x0f) ? CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
            CPU_regs[ A ] > tempReg ? CPU_obj->Set_CFlag() : CPU_obj->Clear_CFlag() ;
            CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

            CPU_obj->Set_SubFlag() ;
            CPU_obj->currentStatus.deltaCycle += 4 ;
        }   break ;
        case 0xD6 : {
            /* SUB imm8 */
            uint8_t tempReg = CPU_regs[ A ] ;
            uint8_t immVal = CPU_obj->memory[ ++CPU_PC ] ;

            CPU_regs[ A ] -= immVal ;

            (tempReg & 0x0f) < (CPU_obj->memory[ CPU_PC ] & 0x0f) ? CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
            CPU_regs[ A ] > tempReg ? CPU_obj->Set_CFlag() : CPU_obj->Clear_CFlag() ;
            CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

            CPU_obj->Set_SubFlag() ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
        } break ;
        case 0x96 : {
            /* SUB (HL) */
            uint8_t tempReg = CPU_regs[ A ] ;

            CPU_regs[ A ] -= CPU_obj->memory[ CPU_obj->Get_HL() ] ;

            (tempReg & 0x0f) < (CPU_obj->memory[ CPU_obj->Get_HL() ] & 0x0f) ? CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
            CPU_regs[ A ] > tempReg ? CPU_obj->Set_CFlag() : CPU_obj->Clear_CFlag() ;
            CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

            CPU_obj->Set_SubFlag() ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
        } break ;
        case 0x98 : case 0x99 : case 0x9A : case 0x9B : case 0x9C : case 0x9D : case 0x9F : {
            /* SBC A, r */
            uint8_t tempReg = CPU_regs[ A ] ;
            uint8_t regIndex = static_cast<uint8_t> ( opcode & 0b00000111 ) ;

            CPU_regs[ A ] -= CPU_regs[ regIndex ] + CPU_obj->Get_CFlag()  ;

            (tempReg & 0x0f) < (CPU_regs[ regIndex ] & 0x0f) + CPU_obj->Get_CFlag() ? CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
            CPU_regs[ A ] > tempReg ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
            CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

            CPU_obj->Set_SubFlag() ;
            CPU_obj->currentStatus.deltaCycle += 4 ;
        } break ;
        case 0xDE : {
            /* SBC A, imm8 */
            uint8_t tempReg = CPU_regs[ A ] ;
            uint8_t immVal = CPU_regs[ ++CPU_PC ] ;

            CPU_regs[ A ] -= immVal + CPU_obj->Get_CFlag()  ;

            (tempReg & 0x0f) < (immVal & 0x0f) + CPU_obj->Get_CFlag() ?
            CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
            CPU_regs[ A ] > tempReg ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
            CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

            CPU_obj->Set_SubFlag() ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
        }   break ;
        case 0x9E : {
            /* SBC A, (HL) */
            uint8_t tempReg = CPU_regs[ A ] ;

            CPU_regs[ A ] -= CPU_obj->memory[ CPU_obj->Get_HL() ] + CPU_obj->Get_CFlag()  ;

            (tempReg & 0x0f) < (CPU_obj->memory[ CPU_obj->Get_HL() ] & 0x0f) + CPU_obj->Get_CFlag() ?
            CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
            CPU_regs[ A ] > tempReg ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
            CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;

            CPU_obj->Set_SubFlag() ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
        }   break ;
        case 0xA0 : case 0xA1 : case 0xA2 : case 0xA3 : case 0xA4 : case 0xA5 : case 0xA7 :
            /* AND r */
            CPU_regs[ A ] &= CPU_regs[ static_cast<uint8_t> ( opcode & 0b00000111 ) ] ;
            CPU_obj->Clear_CFlag() ;
            CPU_obj->Set_HCFlag() ;
            CPU_obj->Clear_SubFlag() ;
            CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
            CPU_obj->currentStatus.deltaCycle += 4 ;
        break ;
        case 0xE6 :
            /* AND imm8 */
            CPU_regs[ A ] &= CPU_obj->memory[ ++CPU_PC ] ;
            CPU_obj->Clear_CFlag() ;
            CPU_obj->Set_HCFlag() ;
            CPU_obj->Clear_SubFlag() ;
            CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0xA6 :
            /* AND (HL) */
            CPU_regs[ A ] &= CPU_obj->memory[ CPU_obj->Get_HL() ] ;
            CPU_obj->Clear_CFlag() ;
            CPU_obj->Set_HCFlag() ;
            CPU_obj->Clear_SubFlag() ;
            CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0xB0 : case 0xB1 : case 0xB2 : case 0xB3 : case 0xB4 : case 0xB5 : case 0xB7 :
            /* OR r */
            CPU_regs[ A ] |= CPU_regs[ static_cast<uint8_t> ( opcode & 0b00000111 ) ] ;
            CPU_obj->Clear_CFlag() ;
            CPU_obj->Clear_HCFlag() ;
            CPU_obj->Clear_SubFlag() ;
            CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
            CPU_obj->currentStatus.deltaCycle += 4 ;
            break ;
        case 0xF6 :
            /* OR imm8 */
            CPU_regs[ A ] |= CPU_obj->memory[ ++CPU_PC ] ;
            CPU_obj->Clear_CFlag() ;
            CPU_obj->Clear_HCFlag() ;
            CPU_obj->Clear_SubFlag() ;
            CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0xB6 :
            /* OR (HL) */
            CPU_regs[ A ] |= CPU_obj->memory[ CPU_obj->Get_HL() ] ;
            CPU_obj->Clear_CFlag() ;
            CPU_obj->Clear_HCFlag() ;
            CPU_obj->Clear_SubFlag() ;
            CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0xA8 : case 0xA9 : case 0xAA : case 0xAB : case 0xAC : case 0xAD : case 0xAF :
            /* XOR r */
            CPU_regs[ A ] ^= CPU_regs[ static_cast<uint8_t> ( opcode & 0b00000111 ) ] ;
            CPU_obj->Clear_CFlag() ;
            CPU_obj->Clear_HCFlag() ;
            CPU_obj->Clear_SubFlag() ;
            CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
            CPU_obj->currentStatus.deltaCycle += 4 ;
            break ;
        case 0xEE :
            /* XOR imm8 */
            CPU_regs[ A ] ^= CPU_obj->memory[ ++CPU_PC ] ;
            CPU_obj->Clear_CFlag() ;
            CPU_obj->Clear_HCFlag() ;
            CPU_obj->Clear_SubFlag() ;
            CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0xAE :
            /* XOR (HL) */
            CPU_regs[ A ] ^= CPU_obj->memory[ CPU_obj->Get_HL() ] ;
            CPU_obj->Clear_CFlag() ;
            CPU_obj->Clear_HCFlag() ;
            CPU_obj->Clear_SubFlag() ;
            CPU_regs[ A ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0xB8 : case 0xB9 : case 0xBA : case 0xBB : case 0xBC : case 0xBD : case 0xBF :
            /* CP r */
            CPU_obj->Set_SubFlag() ;
            if ( CPU_regs[ A ] > CPU_regs[ static_cast<uint8_t> ( opcode & 0b00000111 ) ] ) {
                CPU_obj->Clear_ZeroFlag() ;
                CPU_obj->Set_HCFlag() ;
                CPU_obj->Clear_CFlag() ;
            } // if
            else if ( CPU_regs[ A ] == CPU_regs[ static_cast<uint8_t> ( opcode & 0b00000111 ) ] ) {
                CPU_obj->Set_ZeroFlag() ;
                CPU_obj->Clear_HCFlag() ;
                CPU_obj->Clear_CFlag() ;
            } // else if
            else {
                CPU_obj->Clear_ZeroFlag() ;
                CPU_obj->Clear_HCFlag() ;
                CPU_obj->Set_CFlag() ;
            } // else

            CPU_obj->currentStatus.deltaCycle += 4 ;
            break ;
        case 0xFE : {
            /* CP imm8 */
            uint8_t immVal = CPU_obj->memory[ ++CPU_PC ] ;
            CPU_obj->Set_SubFlag() ;
            if ( CPU_regs[ A ] > immVal  ) {
                CPU_obj->Clear_ZeroFlag() ;
                CPU_obj->Set_HCFlag() ;
                CPU_obj->Clear_CFlag() ;
            } // if
            else if ( CPU_regs[ A ] == immVal ) {
                CPU_obj->Set_ZeroFlag() ;
                CPU_obj->Clear_HCFlag() ;
                CPU_obj->Clear_CFlag() ;
            } // else if
            else {
                CPU_obj->Clear_ZeroFlag() ;
                CPU_obj->Clear_HCFlag() ;
                CPU_obj->Set_CFlag() ;
            } // else

            CPU_obj->currentStatus.deltaCycle += 8 ;
        }   break;
        case 0xBE :
            /* CP (HL) */
            CPU_obj->Set_SubFlag() ;
            if ( CPU_regs[ A ] > CPU_obj->memory[ CPU_obj->Get_HL() ]  ) {
                CPU_obj->Clear_ZeroFlag() ;
                CPU_obj->Set_HCFlag() ;
                CPU_obj->Clear_CFlag() ;
            } // if
            else if ( CPU_regs[ A ] == CPU_obj->memory[ CPU_obj->Get_HL() ] ) {
                CPU_obj->Set_ZeroFlag() ;
                CPU_obj->Clear_HCFlag() ;
                CPU_obj->Clear_CFlag() ;
            } // else if
            else {
                CPU_obj->Clear_ZeroFlag() ;
                CPU_obj->Clear_HCFlag() ;
                CPU_obj->Set_CFlag() ;
            } // else

            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0x04 : case 0x0C : case 0x14 : case 0x1C : case 0x24 : case 0x2C : case 0x3C : {
            /* ADD r */
            uint8_t regIndex = static_cast<uint8_t>( opcode & 0b00111000 ) >> 3 ;
            CPU_regs[ regIndex ] >= 0b00001111 ? CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
            ++ CPU_regs[ regIndex ] ;
            CPU_regs[ regIndex ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
            CPU_obj->Clear_SubFlag();
            CPU_obj->currentStatus.deltaCycle += 4 ;
        }   break ;
        case 0x34 :
            /* ADD (HL) */
            CPU_obj->memory[ CPU_obj->Get_HL() ] >= 0b00001111 ? CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
            ++ CPU_obj->memory[ CPU_obj->Get_HL() ] ;
            CPU_obj->memory[ CPU_obj->Get_HL() ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
            CPU_obj->Clear_SubFlag() ;
            CPU_obj->currentStatus.deltaCycle += 12 ;
            break ;
        case 0x05 : case 0x0D : case 0x15 : case 0x1D : case 0x25 : case 0x2D : case 0x3D : {
            /* DEC r */
            uint8_t regIndex = static_cast<uint8_t>( opcode & 0b00111000 ) >> 3 ;
            CPU_regs[ regIndex ] == 0b00010000 || CPU_regs[ regIndex ] == 0x0 ? CPU_obj->Set_HCFlag() : CPU_obj->Clear_HCFlag() ;
            -- CPU_regs[ regIndex ] ;
            CPU_regs[ regIndex ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
            CPU_obj->Set_SubFlag();
            CPU_obj->currentStatus.deltaCycle += 4 ;
        }   break ;
        case 0x35 :
            /* DEC (HL) */
            if ( CPU_obj->memory[ CPU_obj->Get_HL() ] == 0b00010000 || CPU_obj->memory[ CPU_obj->Get_HL() ] == 0x0 )
                CPU_obj->Set_HCFlag();
            else
                CPU_obj->Clear_HCFlag();

            -- CPU_obj->memory[ CPU_obj->Get_HL() ] ;
            CPU_obj->memory[ CPU_obj->Get_HL() ] == 0 ? CPU_obj->Set_ZeroFlag() : CPU_obj->Clear_ZeroFlag() ;
            CPU_obj->Set_SubFlag();
            CPU_obj->currentStatus.deltaCycle += 12 ;
            break ;
        /* 16-Bit Arithmetic Operation */
        case 0x09 : {
            /* ADD HL, BC */
            uint16_t tempReg = CPU_obj->Get_HL() ;
            if ( ( (tempReg & 0x0fff) + (CPU_obj->Get_BC() & 0x0fff) & 0x1000 ) == 0x1000 )
                CPU_obj->Set_HCFlag() ;
            else
                CPU_obj->Clear_HCFlag() ;

            CPU_obj->Set_HL( CPU_obj->Get_HL() + CPU_obj->Get_BC() ) ;

            if ( CPU_obj->Get_HL() < tempReg )
                CPU_obj->Set_CFlag() ;
            else
                CPU_obj->Clear_CFlag() ;
            CPU_obj->Clear_SubFlag();
            CPU_obj->currentStatus.deltaCycle += 8 ;
        } break ;
        case 0x19 : {
            /* ADD HL, DE */
            uint16_t tempReg = CPU_obj->Get_HL() ;
            if ( ( (tempReg & 0x0fff) + (CPU_obj->Get_DE() & 0x0fff) & 0x1000 ) == 0x1000 )
                CPU_obj->Set_HCFlag() ;
            else
                CPU_obj->Clear_HCFlag() ;

            CPU_obj->Set_HL( CPU_obj->Get_HL() + CPU_obj->Get_DE() ) ;

            if ( CPU_obj->Get_HL() < tempReg )
                CPU_obj->Set_CFlag() ;
            else
                CPU_obj->Clear_CFlag() ;
            CPU_obj->Clear_SubFlag();
            CPU_obj->currentStatus.deltaCycle += 8 ;
        }   break ;
        case 0x29 : {
            /* ADD HL, HL */
            uint16_t tempReg = CPU_obj->Get_HL() ;
            if ( ( (tempReg & 0x0fff) + (tempReg & 0x0fff) & 0x1000 ) == 0x1000 )
                CPU_obj->Set_HCFlag() ;
            else
                CPU_obj->Clear_HCFlag() ;

            CPU_obj->Set_HL( CPU_obj->Get_HL() + CPU_obj->Get_HL() ) ;

            if ( CPU_obj->Get_HL() < tempReg )
                CPU_obj->Set_CFlag() ;
            else
                CPU_obj->Clear_CFlag() ;
            CPU_obj->Clear_SubFlag();
            CPU_obj->currentStatus.deltaCycle += 8 ;
        }   break ;
        case 0x39 : {
            /* ADD HL, SP */
            uint16_t tempReg = CPU_obj->Get_HL() ;
            if ( ( (tempReg & 0x0fff) + (CPU_SP & 0x0fff) & 0x1000 ) == 0x1000 )
                CPU_obj->Set_HCFlag() ;
            else
                CPU_obj->Clear_HCFlag() ;

            CPU_obj->Set_HL( CPU_obj->Get_HL() + CPU_SP ) ;

            if ( CPU_obj->Get_HL() < tempReg )
                CPU_obj->Set_CFlag() ;
            else
                CPU_obj->Clear_CFlag() ;
            CPU_obj->Clear_SubFlag();
            CPU_obj->currentStatus.deltaCycle += 8 ;
        }   break ;
        case 0xE8 : {
            /* ADD SP, r8 */
            uint16_t tempReg = CPU_SP ;
            if ( ( (tempReg & 0x0fff) + static_cast<uint16_t>( CPU_obj->memory[ CPU_PC + 1 ] ) & 0x1000 ) == 0x1000 )
                CPU_obj->Set_HCFlag() ;
            else
                CPU_obj->Clear_HCFlag() ;

            CPU_SP += CPU_obj->memory[ ++CPU_PC ] ;

            if ( CPU_SP < tempReg )
                CPU_obj->Set_CFlag() ;
            else
                CPU_obj->Clear_CFlag() ;

            CPU_obj->Clear_ZeroFlag() ;
            CPU_obj->Clear_SubFlag();
            CPU_obj->currentStatus.deltaCycle += 16 ;
        }   break ;
        case 0x03 :
            /* INC BC */
            CPU_obj->Set_BC(static_cast<uint16_t>(CPU_obj->Get_BC() + 1)) ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0x13 :
            /* INC DE */
            CPU_obj->Set_DE(static_cast<uint16_t>(CPU_obj->Get_DE() + 1)) ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0x23 :
            /* INC HL */
            CPU_obj->Set_HL(static_cast<uint16_t>(CPU_obj->Get_HL() + 1)) ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0x33 :
            /* INC SP */
            ++ CPU_SP ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0x0B :
            /* DEC BC */
            CPU_obj->Set_BC(static_cast<uint16_t>(CPU_obj->Get_BC() - 1)) ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0x1B :
            /* DEC DE */
            CPU_obj->Set_DE(static_cast<uint16_t>(CPU_obj->Get_DE() - 1)) ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0x2B :
            /* DEC HL */
            CPU_obj->Set_HL(static_cast<uint16_t>(CPU_obj->Get_HL() - 1)) ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;
        case 0x3B :
            /* DEC SP */
            -- CPU_SP ;
            CPU_obj->currentStatus.deltaCycle += 8 ;
            break ;

        /* Rotate Shift */
        case 0x07 :
            /* RLCA */
            ( CPU_regs[ A ] & 0b10000000 ) == 0 ? CPU_obj->Clear_CFlag() : CPU_obj->Set_CFlag() ;
            CPU_regs[ A ] = CPU_regs[ A ] << 1 ;
            CPU_regs[ A ] = CPU_regs[ A ] | static_cast<uint8_t>( CPU_obj->Get_CFlag() ) ;

            CPU_obj->Clear_ZeroFlag() ;
            CPU_obj->Clear_HCFlag() ;
            CPU_obj->Clear_SubFlag() ;
            CPU_obj->currentStatus.deltaCycle += 4 ;
            break ;
        case 0x0F :
            /* RRCA */
            ( CPU_regs[ A ] & 0b00000001 ) == 0 ? CPU_obj->Clear_CFlag() : CPU_obj->Set_CFlag() ;
            CPU_regs[ A ] = CPU_regs[ A ] >> 1 ;
            CPU_regs[ A ] = CPU_regs[ A ] | static_cast<uint8_t>( CPU_obj->Get_CFlag() ) << 7 ;

            CPU_obj->Clear_ZeroFlag() ;
            CPU_obj->Clear_HCFlag() ;
            CPU_obj->Clear_SubFlag() ;
            CPU_obj->currentStatus.deltaCycle += 4 ;
            break ;
        case 0x17 : {
            /* RLA */
            bool originalCarry = CPU_obj->Get_CFlag() ;
            ( CPU_regs[ A ] & 0b10000000 ) == 0 ? CPU_obj->Clear_CFlag() : CPU_obj->Set_CFlag() ;
            CPU_regs[ A ] = ( CPU_regs[ A ] << 1 ) | originalCarry ;

            CPU_obj->Clear_ZeroFlag() ;
            CPU_obj->Clear_HCFlag() ;
            CPU_obj->Clear_SubFlag() ;
            CPU_obj->currentStatus.deltaCycle += 4 ;
        }   break ;
        case 0x1F : {
            /* RRA */
            bool originalCarry = CPU_obj->Get_CFlag() ;
            ( CPU_regs[ A ] & 0b00000001 ) == 0  ? CPU_obj->Clear_CFlag() : CPU_obj->Set_CFlag() ;
            CPU_regs[ A ] = ( CPU_regs[ A ] >> 1 ) | ( originalCarry << 7 ) ;

            CPU_obj->Clear_ZeroFlag() ;
            CPU_obj->Clear_HCFlag() ;
            CPU_obj->Clear_SubFlag() ;
            CPU_obj->currentStatus.deltaCycle += 4 ;
        }   break ;
        /* General-Purpose Arithmetic Operations and CPU Control  */
        case 0x27 :
            /* DAA */
            if ( CPU_obj->Get_SubFlag() ) {
                if ( CPU_obj->Get_CFlag() )
                    CPU_regs[ A ] += 0x60 ;
                if ( CPU_obj->Get_HCFlag() )
                    CPU_regs[ A ] += 0x06 ;
            } // if
            else {
                if ( CPU_obj->Get_CFlag() || CPU_regs[ A ] > 0x99 ) {
                    CPU_regs[ A ] += 0x60 ;
                    CPU_obj->Set_CFlag() ;
                } // if
                if ( CPU_obj->Get_HCFlag() || ( CPU_regs[ A ] & 0x0f ) > 0x09 )
                    CPU_regs[ A ] += 0x06 ;
            }

            if ( CPU_regs[ A ] == 0 )
                CPU_obj->Set_ZeroFlag() ;
            CPU_obj->Clear_HCFlag() ;
            CPU_obj->currentStatus.deltaCycle += 4 ;
            break ;
        case 0x37 :
            /* SCF */
            CPU_obj->Set_CFlag() ;
            CPU_obj->Clear_HCFlag() ;
            CPU_obj->Clear_SubFlag() ;
            CPU_obj->currentStatus.deltaCycle += 4 ;
            break ;
        case 0x2F :
            /* CPL */
            CPU_regs[ A ] = ~CPU_regs[ A ] ;
            CPU_obj->Set_HCFlag() ;
            CPU_obj->Set_SubFlag() ;
            CPU_obj->currentStatus.deltaCycle += 4 ;
            break ;
        case 0x3F :
            /* CCF */
            CPU_obj->Get_CFlag() ? CPU_obj->Clear_CFlag() : CPU_obj->Set_CFlag() ;
            CPU_obj->Clear_HCFlag() ;
            CPU_obj->Clear_SubFlag() ;
            CPU_obj->currentStatus.deltaCycle += 4 ;
            break ;
        /* Jump */
        case 0xC3 : {
            /* JP nn */
            uint16_t* immVal = reinterpret_cast<uint16_t*>( CPU_obj->memory + ++CPU_PC ) ;
            CPU_PC = *immVal ;
            ++ CPU_PC ;
            CPU_obj->currentStatus.deltaCycle += 4;
        }   break ;
        case 0xC2 : case 0xCA : case 0xD2 : case 0xDA : {
            /* JP cc,  nn */
            bool condition = false ;
            switch ( opcode & 0b00011000 ) {
                case 0x00 :
                    condition = !CPU_obj->Get_ZeroFlag() ;
                    break ;
                case 0x08 :
                    condition = CPU_obj->Get_ZeroFlag() ;
                    break ;
                case 0x10 :
                    condition = !CPU_obj->Get_CFlag() ;
                    break ;
                case 0x18 :
                    condition = CPU_obj->Get_CFlag() ;
                    break ;
            } // switch

            if ( condition ) {
                uint16_t* immVal = reinterpret_cast<uint16_t*>( CPU_obj->memory + ++CPU_PC ) ;
                CPU_PC = *immVal ;
                CPU_obj->currentStatus.deltaCycle += 16 ;
            } // if
            else {
                ++ CPU_PC ;
                CPU_obj->currentStatus.deltaCycle += 12 ;
            } // else

            ++ CPU_PC ;
        }   break ;
        case 0x18 :
            /* JR e */
            CPU_PC = CPU_PC + static_cast<int8_t>( CPU_obj->memory[ ++CPU_PC ] ) ;
            CPU_obj->currentStatus.deltaCycle += 12 ;
            break ;
        case 0x20 : case 0x28 : case 0x30 : case 0x38 : {
            /* JR cc, e */
            bool condition = false ;
            switch ( opcode & 0b00011000 ) {
                case 0x00 :
                    condition = !CPU_obj->Get_ZeroFlag() ;
                    break ;
                case 0x08 :
                    condition = CPU_obj->Get_ZeroFlag() ;
                    break ;
                case 0x10 :
                    condition = !CPU_obj->Get_CFlag() ;
                    break ;
                case 0x18 :
                    condition = CPU_obj->Get_CFlag() ;
                    break ;
            } // switch

            if ( condition ) {
                CPU_PC = CPU_PC + static_cast<int8_t>( CPU_obj->memory[ ++CPU_PC ] ) ;
                CPU_obj->currentStatus.deltaCycle += 12 ;
            } // if
            else {
                ++CPU_PC ;
                CPU_obj->currentStatus.deltaCycle += 8 ;
            } // else
        }   break ;
        case 0xE9 :
            /* JP HL */
            CPU_PC = CPU_obj->Get_HL() ;
            ++ CPU_PC ;
            CPU_obj->currentStatus.deltaCycle += 4 ;
            break ;
        /* Call and Return */
        case 0xCD : {
            /* CALL nn */
            uint8_t* PC_highlow = reinterpret_cast<uint8_t*>( &( CPU_PC ) ) ;
            CPU_obj->memory[ CPU_SP - 1 ] = PC_highlow[ HIGH ] ;
            CPU_obj->memory[ CPU_SP - 2 ] = PC_highlow[ LOW ] ;
            CPU_SP -= 2 ;
            CPU_PC = *(uint16_t*)( CPU_obj->memory + ++CPU_PC ) ;
            ++ CPU_PC ;
            CPU_obj->currentStatus.deltaCycle += 24 ;
        }   break ;
        case 0xC4 : case 0xCC : case 0xD4 : case 0xDC : {
            /* CALL cc, nn */
            bool condition = false ;
            switch ( opcode & 0b00011000 ) {
                case 0x00 :
                    condition = !CPU_obj->Get_ZeroFlag() ;
                    break ;
                case 0x08 :
                    condition = CPU_obj->Get_ZeroFlag() ;
                    break ;
                case 0x10 :
                    condition = !CPU_obj->Get_CFlag() ;
                    break ;
                case 0x18 :
                    condition = CPU_obj->Get_CFlag() ;
                    break ;
            } // switch

            if ( condition ) {
                uint8_t* PC_highlow = reinterpret_cast<uint8_t*>( &( CPU_PC ) ) ;
                CPU_obj->memory[ CPU_SP - 1 ] = PC_highlow[ HIGH ] ;
                CPU_obj->memory[ CPU_SP - 2 ] = PC_highlow[ LOW ] ;
                CPU_SP -= 2 ;
                CPU_PC = *(uint16_t*)( CPU_obj->memory + ++CPU_PC ) ;
                CPU_obj->currentStatus.deltaCycle += 24 ;
            } // if
            else {
                ++ CPU_PC ;
                CPU_obj->currentStatus.deltaCycle += 12 ;
            } // else

            ++ CPU_PC ;
        }   break ;
        case 0xC9 : {
            /* RET */
            uint8_t* PC_highlow = reinterpret_cast<uint8_t*>( &( CPU_PC ) ) ;
            PC_highlow[ HIGH ] = CPU_obj->memory[ CPU_SP + 1 ] ;
            PC_highlow[ LOW ] = CPU_obj->memory[ CPU_SP ] ;
            CPU_SP += 2 ;
            CPU_obj->currentStatus.deltaCycle += 16 ;
        }   break ;
        case 0xD9 : {
            /* RETI */
            CPU_obj->currentStatus.IME = true ; // same as EI
            uint8_t* PC_highlow = reinterpret_cast<uint8_t*>( &( CPU_PC ) ) ;
            PC_highlow[ HIGH ] = CPU_obj->memory[ CPU_SP + 1 ] ;
            PC_highlow[ LOW ] = CPU_obj->memory[ CPU_SP ] ;
            CPU_SP += 2 ;
            CPU_obj->currentStatus.deltaCycle += 16 ;
        }   break ;
        case 0xC0 : case 0xC8 : case 0xD0 : case 0xD8 : {
            /* RET cc */
            bool condition = false ;
            switch ( opcode & 0b00011000 ) {
                case 0x00 :
                    condition = !CPU_obj->Get_ZeroFlag() ;
                    break ;
                case 0x08 :
                    condition = CPU_obj->Get_ZeroFlag() ;
                    break ;
                case 0x10 :
                    condition = !CPU_obj->Get_CFlag() ;
                    break ;
                case 0x18 :
                    condition = CPU_obj->Get_CFlag() ;
                    break ;
            } // switch

            if ( condition ) {
                uint8_t* PC_highlow = reinterpret_cast<uint8_t*>( &( CPU_PC ) ) ;
                PC_highlow[ HIGH ] = CPU_obj->memory[ CPU_SP + 1 ] ;
                PC_highlow[ LOW ] = CPU_obj->memory[ CPU_SP ] ;
                CPU_SP += 2 ;
                currentStatus.afterReturn = true ;
                CPU_obj->currentStatus.deltaCycle += 20 ;
            } // if
            else
                CPU_obj->currentStatus.deltaCycle += 8 ;

        }   break ;
        case 0xC7 : case 0xCF : case 0xD7 : case 0xDF : case 0xE7 : case 0xEF : case 0xF7 : case 0xFF : {
            /* RST t */
            uint8_t* PC_highlow = reinterpret_cast<uint8_t*>( &( CPU_PC ) ) ;
            CPU_obj->memory[ CPU_SP - 1 ] = PC_highlow[ HIGH ] ;
            CPU_obj->memory[ CPU_SP - 2 ] = PC_highlow[ LOW ] ;
            CPU_SP -= 2 ;
            CPU_PC = 0x0 + ( (opcode & 0b00111000) >> 3 ) * 8 ;
            CPU_obj->currentStatus.deltaCycle += 16 ;
        }   break ;
        default:
            printf( "Unknown opcode: 0x%x\n", opcode ) ;
            exit( -1 ) ;
    } // switch
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
    delete memory ;
}

void (*LR35902::unprefixed_opcode[ 256 ])(void) = {
        NOP, // 0
        LD_BC_imm16, // 0x1
        LD_refBC_A, // 0x2
        INC_BC, // 0x3
        INC_B, // 0x4
        DEC_B, // 0x5
        LD_B_imm8, // 0x6
        RLCA, // 0x07
        LD_FFZZ_offset16_SP, // 0x8
        ADD_HL_BC, // 0x9
        LD_A_refBC, // 0xa
        DEC_BC, // 0xb
        INC_C, // 0xc
        DEC_C, // 0xd
        LD_C_imm8, // 0xe
        RRCA, // 0x0f
        STOP, // 0x10
        LD_DE_imm16, // 0x11
        LD_refDE_A, // 0x12
        INC_DE, // 0x13
        INC_D, // 0x14
        DEC_D, // 0x15
        LD_D_imm8, // 0x16
        RLA, // 0x17
        JR_imm8, // 0x18
        ADD_HL_DE, // 0x19
        LD_A_refDE, // 0x1a
        DEC_DE, // 0x1b
        INC_E, // 0x1c
        DEC_E, // 0x1d
        LD_E_imm8, // 0x1e
        RRA, // 0x1f
        JR_NZ_imm8, // 0x20
        LD_HL_imm16, // 0x21
        LD_refHL_A_INC, // 0x22
        INC_HL, // 0x23
        INC_H, // 0x24
        DEC_H, // 0x25
        LD_H_imm8, // 0x26
        DAA, // 0x27
        JR_Z_imm8, // 0x28
        ADD_HL_HL, // 0x29
        LD_A_refHL_INC, // 0x2a
        DEC_HL, // 0x2b
        INC_L, // 0x2c
        DEC_L, // 0x2d
        LD_L_imm8, // 0x2e
        CPL, // 0x2f
        JR_NC_imm8, // 0x30
        LD_SP_imm16, // 0x31
        LD_refHL_A_DEC, // 0x32
        INC_SP, // 0x33
        INC_refHL, // 0x34
        DEC_refHL, // 0x35
        LD_refHL_imm8, // 0x36
        SCF, // 0x37
        JR_C_imm8, // 0x38
        ADD_HL_SP, // 0x39
        LD_A_refHL_DEC, // 0x3a
        DEC_SP, // 0x3b
        INC_A, // 0x3c
        DEC_A, // 0x3d
        LD_A_imm8, // 0x3e
        CCF, // 0x3f
        LD_B_B, // 0x40
        LD_B_C, // 0x41
        LD_B_D, // 0x42
        LD_B_E, // 0x43
        LD_B_H, // 0x44
        LD_B_L, // 0x45
        LD_B_refHL, // 0x46
        LD_B_A, // 0x47
        LD_C_B, // 0x48
        LD_C_C, // 0x49
        LD_C_D, // 0x4a
        LD_C_E, // 0x4b
        LD_C_H, // 0x4c
        LD_C_L, // 0x4d
        LD_C_refHL, // 0x4e
        LD_C_A, // 0x4f
        LD_D_B, // 0x50
        LD_D_C, // 0x51
        LD_D_D, // 0x52
        LD_D_E, // 0x53
        LD_D_H, // 0x54
        LD_D_L, // 0x55
        LD_D_refHL, // 0x56
        LD_D_A, // 0x57
        LD_E_B, // 0x58
        LD_E_C, // 0x59
        LD_E_D, // 0x5a
        LD_E_E, // 0x5b
        LD_E_H, // 0x5c
        LD_E_L, // 0x5d
        LD_E_refHL, // 0x5e
        LD_E_A, // 0x5f
        LD_H_B, // 0x60
        LD_H_C, // 0x61
        LD_H_D, // 0x62
        LD_H_E, // 0x63
        LD_H_H, // 0x64
        LD_H_L, // 0x65
        LD_H_refHL, // 0x66
        LD_H_A, // 0x67
        LD_L_B, // 0x68
        LD_L_C, // 0x69
        LD_L_D, // 0x6a
        LD_L_E, // 0x6b
        LD_L_H, // 0x6c
        LD_L_L, // 0x6d
        LD_L_refHL, // 0x6e
        LD_L_A, // 0x6f
        LD_refHL_B, // 0x70
        LD_refHL_C, // 0x71
        LD_refHL_D, // 0x72
        LD_refHL_E, // 0x73
        LD_refHL_H, // 0x74
        LD_refHL_L, // 0x75
        HALT, // 0x76
        LD_refHL_A, // 0x77
        LD_A_B, // 0x78
        LD_A_C, // 0x79
        LD_A_D, // 0x7a
        LD_A_E, // 0x7b
        LD_A_H, // 0x7c
        LD_A_L, // 0x7d
        LD_A_refHL, // 0x7e
        LD_A_A, // 0x7f
        ADD_A_B, // 0x80
        ADD_A_C, // 0x81
        ADD_A_D, // 0x82
        ADD_A_E, // 0x83
        ADD_A_H, // 0x84
        ADD_A_L, // 0x85
        ADD_A_refHL, // 0x86
        ADD_A_A, // 0x87
        ADC_A_B, // 0x88
        ADC_A_C, // 0x89
        ADC_A_D, // 0x8a
        ADC_A_E, // 0x8b
        ADC_A_H, // 0x8c
        ADC_A_L, // 0x8d
        ADC_A_refHL, // 0x8e
        ADC_A_A, // 0x8f
        SUB_B, // 0x90
        SUB_C, // 0x91
        SUB_D, // 0x92
        SUB_E, // 0x93
        SUB_H, // 0x94
        SUB_L, // 0x95
        SUB_refHL, // 0x96
        SUB_A, // 0x97
        SBC_A_B, // 0x98
        SBC_A_C, // 0x99
        SBC_A_D, // 0x9a
        SBC_A_E, // 0x9b
        SBC_A_H, // 0x9c
        SBC_A_L, // 0x9d
        SBC_A_refHL, // 0x9e
        SBC_A_A, // 0x9f
        AND_B, // 0xa0
        AND_C, // 0xa1
        AND_D, // 0xa2
        AND_E, // 0xa3
        AND_H, // 0xa4
        AND_L, // 0xa5
        AND_refHL, // 0xa6
        AND_A, // 0xa7
        XOR_B, // 0xa8
        XOR_C, // 0xa9
        XOR_D, // 0xaa
        XOR_E, // 0xab
        XOR_H, // 0xac
        XOR_L, // 0xad
        XOR_refHL, // 0xae
        XOR_A, // 0xaf
        OR_B, // 0xb0
        OR_C, // 0xb1
        OR_D, // 0xb2
        OR_E, // 0xb3
        OR_H, // 0xb4
        OR_L, // 0xb5
        OR_refHL, // 0xb6
        OR_A, // 0xb7
        CP_B, // 0xb8
        CP_C, // 0xb9
        CP_D, // 0xba
        CP_E, // 0xbb
        CP_H, // 0xbc
        CP_L, // 0xbd
        CP_refHL, // 0xbe
        CP_A, // 0xbf
        RET_NZ, // 0xc0
        POP_BC, // 0xc1
        JP_NZ_imm16, // 0xc2
        JP_imm16, // 0xc3
        CALL_NZ_imm16, // 0xc4
        PUSH_BC, // 0xc5
        ADD_A_imm8, // 0xc6
        RST_00, // 0xc7
        RET_Z, // 0xc8
        RET, // 0xc9
        JP_Z_imm16, // 0xca
        PRIFIX, // 0xcb
        CALL_Z_imm16, // 0xcc
        CALL_imm16, // 0xcd
        ADC_A_imm8, // 0xce
        RST_08, // 0xcf
        RET_NC, // 0xd0
        POP_DE, // 0xd1
        JP_NC_imm16, // 0xd2
        NOP, // d3
        CALL_NC_imm16, // 0xd4
        PUSH_DE, // 0xd5
        SUB_imm8, // 0xd6
        RST_10, // 0xd7
        RET_C, // 0xd8
        RETI, // 0xd9
        JP_C_imm16, // 0xda
        NOP, // db
        CALL_C_imm16, // 0xdc
        NOP, // dd
        SBC_A_imm8, // 0xde
        RST_18, // 0xdf
        LD_FFZZ_offset8_A, // 0xe0
        POP_HL, // 0xe1
        LD_FFZZ_offset_regC_A, // 0xe2
        NOP, // e3
        NOP, // e4
        PUSH_HL, // 0xe5
        AND_imm8, // 0xe6
        RST_20, // 0xe7
        ADD_SP_imm8, // 0xe8
        JP_HL, // 0xe9
        LD_offset16_A, // 0xea
        NOP, // eb
        NOP, // ec
        NOP, // ed
        XOR_imm8, // 0xee
        RST_28, // 0xef
        LD_A_FFZZ_offset8, // 0xf0
        POP_AF, // 0xf1
        LD_A_FFZZ_offset_regC, // 0xf2
        DI, // 0xf3
        NOP, // f4
        PUSH_AF, // 0xf5
        OR_imm8, // 0xf6
        RST_30, // 0xf7
        LD_HL_SP_offset_sign8, // 0xf8
        LD_SP_HL, // 0xf9
        LD_A_ref_offset16, // 0xfa
        EI, // 0xfb
        NOP, // fc
        NOP, // fd
        CP_imm8, // 0xfe
        RST_38 // 0xff
} ;

