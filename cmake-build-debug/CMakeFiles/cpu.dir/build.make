# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/orzgg/clion-2018.3/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/orzgg/clion-2018.3/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/orzgg/CLionProjects/cpu

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/orzgg/CLionProjects/cpu/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/cpu.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/cpu.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cpu.dir/flags.make

CMakeFiles/cpu.dir/main.cpp.o: CMakeFiles/cpu.dir/flags.make
CMakeFiles/cpu.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/orzgg/CLionProjects/cpu/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/cpu.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cpu.dir/main.cpp.o -c /home/orzgg/CLionProjects/cpu/main.cpp

CMakeFiles/cpu.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cpu.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/orzgg/CLionProjects/cpu/main.cpp > CMakeFiles/cpu.dir/main.cpp.i

CMakeFiles/cpu.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cpu.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/orzgg/CLionProjects/cpu/main.cpp -o CMakeFiles/cpu.dir/main.cpp.s

CMakeFiles/cpu.dir/src/cpu.cpp.o: CMakeFiles/cpu.dir/flags.make
CMakeFiles/cpu.dir/src/cpu.cpp.o: ../src/cpu.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/orzgg/CLionProjects/cpu/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/cpu.dir/src/cpu.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cpu.dir/src/cpu.cpp.o -c /home/orzgg/CLionProjects/cpu/src/cpu.cpp

CMakeFiles/cpu.dir/src/cpu.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cpu.dir/src/cpu.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/orzgg/CLionProjects/cpu/src/cpu.cpp > CMakeFiles/cpu.dir/src/cpu.cpp.i

CMakeFiles/cpu.dir/src/cpu.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cpu.dir/src/cpu.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/orzgg/CLionProjects/cpu/src/cpu.cpp -o CMakeFiles/cpu.dir/src/cpu.cpp.s

CMakeFiles/cpu.dir/src/LR35902_Calc.cpp.o: CMakeFiles/cpu.dir/flags.make
CMakeFiles/cpu.dir/src/LR35902_Calc.cpp.o: ../src/LR35902_Calc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/orzgg/CLionProjects/cpu/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/cpu.dir/src/LR35902_Calc.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cpu.dir/src/LR35902_Calc.cpp.o -c /home/orzgg/CLionProjects/cpu/src/LR35902_Calc.cpp

CMakeFiles/cpu.dir/src/LR35902_Calc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cpu.dir/src/LR35902_Calc.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/orzgg/CLionProjects/cpu/src/LR35902_Calc.cpp > CMakeFiles/cpu.dir/src/LR35902_Calc.cpp.i

CMakeFiles/cpu.dir/src/LR35902_Calc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cpu.dir/src/LR35902_Calc.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/orzgg/CLionProjects/cpu/src/LR35902_Calc.cpp -o CMakeFiles/cpu.dir/src/LR35902_Calc.cpp.s

CMakeFiles/cpu.dir/src/LR35902_LD.cpp.o: CMakeFiles/cpu.dir/flags.make
CMakeFiles/cpu.dir/src/LR35902_LD.cpp.o: ../src/LR35902_LD.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/orzgg/CLionProjects/cpu/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/cpu.dir/src/LR35902_LD.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cpu.dir/src/LR35902_LD.cpp.o -c /home/orzgg/CLionProjects/cpu/src/LR35902_LD.cpp

CMakeFiles/cpu.dir/src/LR35902_LD.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cpu.dir/src/LR35902_LD.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/orzgg/CLionProjects/cpu/src/LR35902_LD.cpp > CMakeFiles/cpu.dir/src/LR35902_LD.cpp.i

CMakeFiles/cpu.dir/src/LR35902_LD.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cpu.dir/src/LR35902_LD.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/orzgg/CLionProjects/cpu/src/LR35902_LD.cpp -o CMakeFiles/cpu.dir/src/LR35902_LD.cpp.s

CMakeFiles/cpu.dir/src/LR35902_Misc.cpp.o: CMakeFiles/cpu.dir/flags.make
CMakeFiles/cpu.dir/src/LR35902_Misc.cpp.o: ../src/LR35902_Misc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/orzgg/CLionProjects/cpu/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/cpu.dir/src/LR35902_Misc.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cpu.dir/src/LR35902_Misc.cpp.o -c /home/orzgg/CLionProjects/cpu/src/LR35902_Misc.cpp

CMakeFiles/cpu.dir/src/LR35902_Misc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cpu.dir/src/LR35902_Misc.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/orzgg/CLionProjects/cpu/src/LR35902_Misc.cpp > CMakeFiles/cpu.dir/src/LR35902_Misc.cpp.i

CMakeFiles/cpu.dir/src/LR35902_Misc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cpu.dir/src/LR35902_Misc.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/orzgg/CLionProjects/cpu/src/LR35902_Misc.cpp -o CMakeFiles/cpu.dir/src/LR35902_Misc.cpp.s

CMakeFiles/cpu.dir/src/LR35902_Routine.cpp.o: CMakeFiles/cpu.dir/flags.make
CMakeFiles/cpu.dir/src/LR35902_Routine.cpp.o: ../src/LR35902_Routine.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/orzgg/CLionProjects/cpu/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/cpu.dir/src/LR35902_Routine.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cpu.dir/src/LR35902_Routine.cpp.o -c /home/orzgg/CLionProjects/cpu/src/LR35902_Routine.cpp

CMakeFiles/cpu.dir/src/LR35902_Routine.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cpu.dir/src/LR35902_Routine.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/orzgg/CLionProjects/cpu/src/LR35902_Routine.cpp > CMakeFiles/cpu.dir/src/LR35902_Routine.cpp.i

CMakeFiles/cpu.dir/src/LR35902_Routine.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cpu.dir/src/LR35902_Routine.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/orzgg/CLionProjects/cpu/src/LR35902_Routine.cpp -o CMakeFiles/cpu.dir/src/LR35902_Routine.cpp.s

CMakeFiles/cpu.dir/src/LR35902_Stack.cpp.o: CMakeFiles/cpu.dir/flags.make
CMakeFiles/cpu.dir/src/LR35902_Stack.cpp.o: ../src/LR35902_Stack.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/orzgg/CLionProjects/cpu/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/cpu.dir/src/LR35902_Stack.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cpu.dir/src/LR35902_Stack.cpp.o -c /home/orzgg/CLionProjects/cpu/src/LR35902_Stack.cpp

CMakeFiles/cpu.dir/src/LR35902_Stack.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cpu.dir/src/LR35902_Stack.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/orzgg/CLionProjects/cpu/src/LR35902_Stack.cpp > CMakeFiles/cpu.dir/src/LR35902_Stack.cpp.i

CMakeFiles/cpu.dir/src/LR35902_Stack.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cpu.dir/src/LR35902_Stack.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/orzgg/CLionProjects/cpu/src/LR35902_Stack.cpp -o CMakeFiles/cpu.dir/src/LR35902_Stack.cpp.s

CMakeFiles/cpu.dir/src/CPU_status.cpp.o: CMakeFiles/cpu.dir/flags.make
CMakeFiles/cpu.dir/src/CPU_status.cpp.o: ../src/CPU_status.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/orzgg/CLionProjects/cpu/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/cpu.dir/src/CPU_status.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cpu.dir/src/CPU_status.cpp.o -c /home/orzgg/CLionProjects/cpu/src/CPU_status.cpp

CMakeFiles/cpu.dir/src/CPU_status.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cpu.dir/src/CPU_status.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/orzgg/CLionProjects/cpu/src/CPU_status.cpp > CMakeFiles/cpu.dir/src/CPU_status.cpp.i

CMakeFiles/cpu.dir/src/CPU_status.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cpu.dir/src/CPU_status.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/orzgg/CLionProjects/cpu/src/CPU_status.cpp -o CMakeFiles/cpu.dir/src/CPU_status.cpp.s

# Object files for target cpu
cpu_OBJECTS = \
"CMakeFiles/cpu.dir/main.cpp.o" \
"CMakeFiles/cpu.dir/src/cpu.cpp.o" \
"CMakeFiles/cpu.dir/src/LR35902_Calc.cpp.o" \
"CMakeFiles/cpu.dir/src/LR35902_LD.cpp.o" \
"CMakeFiles/cpu.dir/src/LR35902_Misc.cpp.o" \
"CMakeFiles/cpu.dir/src/LR35902_Routine.cpp.o" \
"CMakeFiles/cpu.dir/src/LR35902_Stack.cpp.o" \
"CMakeFiles/cpu.dir/src/CPU_status.cpp.o"

# External object files for target cpu
cpu_EXTERNAL_OBJECTS =

cpu: CMakeFiles/cpu.dir/main.cpp.o
cpu: CMakeFiles/cpu.dir/src/cpu.cpp.o
cpu: CMakeFiles/cpu.dir/src/LR35902_Calc.cpp.o
cpu: CMakeFiles/cpu.dir/src/LR35902_LD.cpp.o
cpu: CMakeFiles/cpu.dir/src/LR35902_Misc.cpp.o
cpu: CMakeFiles/cpu.dir/src/LR35902_Routine.cpp.o
cpu: CMakeFiles/cpu.dir/src/LR35902_Stack.cpp.o
cpu: CMakeFiles/cpu.dir/src/CPU_status.cpp.o
cpu: CMakeFiles/cpu.dir/build.make
cpu: CMakeFiles/cpu.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/orzgg/CLionProjects/cpu/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable cpu"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cpu.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cpu.dir/build: cpu

.PHONY : CMakeFiles/cpu.dir/build

CMakeFiles/cpu.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cpu.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cpu.dir/clean

CMakeFiles/cpu.dir/depend:
	cd /home/orzgg/CLionProjects/cpu/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/orzgg/CLionProjects/cpu /home/orzgg/CLionProjects/cpu /home/orzgg/CLionProjects/cpu/cmake-build-debug /home/orzgg/CLionProjects/cpu/cmake-build-debug /home/orzgg/CLionProjects/cpu/cmake-build-debug/CMakeFiles/cpu.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cpu.dir/depend

