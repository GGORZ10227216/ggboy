cmake_minimum_required(VERSION 3.12)
project(cpu)

set(CMAKE_CXX_STANDARD 14)


#Set your directories.  The dot representes the root application folder.
#Thus my the path to my domain folder:
set(HEADER_FILES ./include)
set(SOURCE_FILES ./src/cpu.cpp
                 ./src/LR35902_Calc.cpp
                 ./src/LR35902_LD.cpp
                 ./src/LR35902_Misc.cpp
        src/LR35902_Routine.cpp include/LR35902_Stack.h src/LR35902_Stack.cpp include/cpu_state.h src/CPU_status.cpp)

#Include the directories and now your cpp files will recognize your headers
include_directories(${HEADER_FILES})

add_executable(cpu main.cpp ${SOURCE_FILES})