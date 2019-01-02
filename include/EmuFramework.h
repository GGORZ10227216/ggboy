//
// Created by ORZGG on 2018/12/31.
//

#include <cpu.h>
#include <MBC.h>
#include <string>
#include <Cartridge.h>

#ifndef CPU_EMUFRAMEWORK_H
#define CPU_EMUFRAMEWORK_H

using namespace lr35902 ;

class EmuFramework {
public :
    EmuFramework( int argc, char* argv[] ) ;
    void StartUI() ;
    bool LoadRom( const char* romPath ) ;
    void StartEmu() ;
    void PauseEmu() ;
    void Reset() ;
    void Stop() ;

    virtual ~EmuFramework();

private :
    LR35902* cpu = nullptr ;
    MBC* mmu = nullptr ;
    Cartridge* cartridge = nullptr ;

    int _argc ;
    char** _argv ;
};


#endif //CPU_EMUFRAMEWORK_H
