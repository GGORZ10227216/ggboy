#include <cstdio>
#include <cpu.h>

using namespace std;

LR35902* CPU_obj = nullptr ;

int main() {
    CPU_obj = new LR35902() ;
    printf( "%d %d\n", ( ( 0xa5 & 0x6c ) == 0x6c ), ( 0xa5 != 0) ) ;
    return 0;
}