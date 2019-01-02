#ifndef LIBLR35902_H
#define LIBLR35902_H

#include <cstdio>

namespace lr35902 {

void NOP() ; // 0x0
void STOP() ; // 0x10
void HALT() ; // 0x76
void PRIFIX() ; // 0xCB
void DI() ; // 0xF3
void EI() ; // 0xFB

} // lr35902

#endif // LIBLR35902_H
