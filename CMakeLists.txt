cmake_minimum_required(VERSION 3.10)
project(ggboy)

set(CMAKE_CXX_STANDARD 14)


#Set your directories.  The dot representes the root application folder.
#Thus my the path to my domain folder:

set(EmuInclude ./include/)
file(GLOB EmuSrc ./src/*.cpp)
set(AudioInclude ./Gb_Snd_Emu/gb_apu/ ./Gb_Snd_Emu)
file(GLOB AudioSrc ./Gb_Snd_Emu/gb_apu/*.cpp ./Gb_Snd_Emu/Sound_Queue.cpp)

#Include the directories and now your cpp files will recognize your headers
include_directories(${EmuInclude} ${AudioInclude})
include_directories(${SDL2_INCLUDE_DIRS})

add_definitions( "-DNDEBUG" )
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "-Ofast -flto")
set(CMAKE_BUILD_TYPE Release)

add_executable(ggboy main.cpp ${EmuSrc} ${AudioSrc})
TARGET_LINK_LIBRARIES(ggboy -lSDL2main -lSDL2)