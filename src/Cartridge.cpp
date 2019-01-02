//
// Created by ORZGG on 2018/12/30.
//

#include "Cartridge.h"
#include <errorCode.h>

#include <iostream>
#include <cstdlib>

using namespace lr35902 ;

Cartridge::Cartridge( const char* romPath ) {
    FILE* romFile = fopen( romPath, "r" ) ;

    if ( romFile != nullptr ) {
        fseek( romFile, 0, SEEK_END ) ;
        rawRomSize = ftell( romFile ) ;
        rewind( romFile ) ;

        rawRomData = new uint8_t[ rawRomSize ] ;
        fread( rawRomData, sizeof(uint8_t), rawRomSize, romFile ) ;
        fclose( romFile ) ;
        isValid = true ;

        ParseHeader() ;
    } // if
    else {
        std::cerr << "Cartridge initialize failed!! Can not open the rom file." << std::endl ;
        exit( CANT_OPEN_ROMFILE ) ;
    } // else
}

void Cartridge::ParseHeader() {
    romName.append( (char*)(rawRomData + TITLE), 16 ) ; // Rom title
    for ( int i = 0 ; i < 4 ; ++i )
        manufactureCode[ i ] = rawRomData[ MANU_CODE + i ] ; // Manufacture Code( 4 byte )

    uint8_t rawMBC_Code = rawRomData[ MBC_CODE ] ;
    switch ( rawMBC_Code ) {
        case 0x0 :
            mbcCode = 0 ; // romOnly
            break ;
        case 0x01 : case 0x02 : case 0x03 :
            mbcCode = 1 ; // MBC1
            break ;
        case 0x05 : case 0x06 :
            mbcCode = 2 ; // MBC2
            break ;
        default:
            std::cerr << "Cartridge initialize failed!! MBC not support." << std::endl ;
            exit(MBC_NOT_SUPPORT) ;
    } // switch

    uint8_t rawRomSizeCode = rawRomData[ ROM_SIZE ] ;
    switch ( rawRomSizeCode ) {
        case 0x0 : case 0x1 : case 0x2 : case 0x3 : case 0x4 : case 0x5 : case 0x7 : case 0x8 :
            romBankNum = static_cast<uint16_t>( rawRomSizeCode == 0 ? 0 : 1 << ( rawRomSizeCode + 1 ) ) ;
            break ;
        case 0x52 :
            romBankNum = 72 ;
            break ;
        case 0x53 :
            romBankNum = 80 ;
            break ;
        case 0x54 :
            romBankNum = 96 ;
            break ;
        default:
            std::cerr << "Cartridge initialize failed!! Unknown ROM size code." << std::endl ;
            exit(UNKNOWN_ROM_SIZE) ;
    } // switch

    uint8_t rawRamSizeCode = rawRomData[ RAM_SIZE ] ;
    switch ( rawRamSizeCode ) {
        case 0x0 :
            extraRamSize = 0 ; // no external RAM
            break ;
        case 0x01 :
            extraRamSize = 2048 ; // 2KBytes RAM
            break ;
        case 0x02 :
            extraRamSize = 8192 ; // 8KBytes RAM
            break ;
        case 0x03 :
            extraRamSize = 32768 ; // 32KBytes RAM
            break ;
        case 0x04 :
            extraRamSize =  131072 ; // 128KBytes RAM
            break ;
        case 0x05 :
            extraRamSize =  65535 ; // 64KBytes RAM
            break ;
        default:
            std::cerr << "Cartridge initialize failed!! Unknown RAM size code." << std::endl ;
            exit(UNKNOWN_RAM_SIZE) ;
    } // switch

    isCgbOnly = rawRomData[ CGB_FLAG ] == 0xC0 ;
}

uint32_t Cartridge::getRawRomSize() const {
    return rawRomSize;
}

const std::string &Cartridge::getRomName() const {
    return romName;
}

const bool Cartridge::getIsCgbOnly() const {
    return isCgbOnly;
}

int Cartridge::GetMBC_Code() const {
    return mbcCode;
}

uint16_t Cartridge::getRomBankNum() const {
    return romBankNum;
}

uint32_t Cartridge::getExtraRamSize() const {
    return extraRamSize;
}

bool Cartridge::IsValid() const {
    return isValid ;
}

uint8_t *Cartridge::getRawRomData() const {
    return rawRomData;
}
