cmake_minimum_required(VERSION 3.12)
project(cpu)

set(CMAKE_CXX_STANDARD 14)


#Set your directories.  The dot representes the root application folder.
#Thus my the path to my domain folder:
set(HEADER_FILES ./include)
set(SOURCE_FILES ./src/cpu.cpp
        include/cpu_state.h src/CPU_status.cpp src/MBC.cpp include/MBC.h src/Cartridge.cpp include/Cartridge.h include/errorCode.h src/EmuFramework.cpp include/EmuFramework.h src/MBC1.cpp include/MBC1.h src/MBC2.cpp include/MBC2.h include/MBC_header.h)

#Include the directories and now your cpp files will recognize your headers
include_directories(${HEADER_FILES})

add_executable(cpu main.cpp ${SOURCE_FILES})