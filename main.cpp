#include <cstdio>
#include <EmuFramework.h>

using namespace std;

int main( int argc, char* argv[] ) {
    EmuFramework emu( argc, argv ) ;
    emu.StartUI() ;
    return 0;
}