//
// Created by ORZGG on 2018/12/30.
//

#include <stdint.h>
#include <string>

#ifndef CPU_CARTRIDGE_H
#define CPU_CARTRIDGE_H

namespace lr35902 {
    class Cartridge {
    public:
        Cartridge() = delete ;
        Cartridge( const char* romPath ) ;

        bool IsValid() const;
        uint32_t getRawRomSize() const;
        const std::string &getRomName() const;
        const bool getIsCgbOnly() const;
        int GetMBC_Code() const;
        uint16_t getRomBankNum() const;
        uint32_t getExtraRamSize() const;

    private:
        uint8_t* rawRomData = nullptr ;
    public:
        uint8_t *getRawRomData() const;

    private:
        uint32_t rawRomSize = 0 ;

        /*Rom info*/
        std::string romName ;
        uint8_t mbcCode ;
        uint16_t romBankNum ;
        uint8_t manufactureCode[ 4 ] ;
        uint32_t  extraRamSize ;

        bool isValid = false, isCgbOnly ;

        void ParseHeader() ;
        enum headerFieldName {
            LOGO = 0x104,
            TITLE = 0x134,
            MANU_CODE = 0x13F,
            CGB_FLAG = 0x143,
            NEW_LICE_CODE = 0x144,
            SGB_FLAG = 0x146,
            MBC_CODE = 0x147,
            ROM_SIZE = 0x148,
            RAM_SIZE = 0x149,
            JAPAN_ONLY = 0x14A,
            OLD_LICE_CODE = 0x14B,
            ROM_VER = 0x14C,
            CHECKSUM = 0x14D,
            G_CHECKSUM = 0x14E
        } ;
    };
}


#endif //CPU_CARTRIDGE_H
