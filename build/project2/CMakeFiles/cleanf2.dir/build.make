# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build

# Include any dependencies generated for this target.
include project2/CMakeFiles/cleanf2.dir/depend.make

# Include the progress variables for this target.
include project2/CMakeFiles/cleanf2.dir/progress.make

# Include the compile flags for this target's objects.
include project2/CMakeFiles/cleanf2.dir/flags.make

project2/CMakeFiles/cleanf2.dir/clean/clean2.cc.o: project2/CMakeFiles/cleanf2.dir/flags.make
project2/CMakeFiles/cleanf2.dir/clean/clean2.cc.o: ../project2/clean/clean2.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object project2/CMakeFiles/cleanf2.dir/clean/clean2.cc.o"
	cd /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build/project2 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cleanf2.dir/clean/clean2.cc.o -c /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/project2/clean/clean2.cc

project2/CMakeFiles/cleanf2.dir/clean/clean2.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cleanf2.dir/clean/clean2.cc.i"
	cd /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build/project2 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/project2/clean/clean2.cc > CMakeFiles/cleanf2.dir/clean/clean2.cc.i

project2/CMakeFiles/cleanf2.dir/clean/clean2.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cleanf2.dir/clean/clean2.cc.s"
	cd /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build/project2 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/project2/clean/clean2.cc -o CMakeFiles/cleanf2.dir/clean/clean2.cc.s

project2/CMakeFiles/cleanf2.dir/clean/clean2.cc.o.requires:

.PHONY : project2/CMakeFiles/cleanf2.dir/clean/clean2.cc.o.requires

project2/CMakeFiles/cleanf2.dir/clean/clean2.cc.o.provides: project2/CMakeFiles/cleanf2.dir/clean/clean2.cc.o.requires
	$(MAKE) -f project2/CMakeFiles/cleanf2.dir/build.make project2/CMakeFiles/cleanf2.dir/clean/clean2.cc.o.provides.build
.PHONY : project2/CMakeFiles/cleanf2.dir/clean/clean2.cc.o.provides

project2/CMakeFiles/cleanf2.dir/clean/clean2.cc.o.provides.build: project2/CMakeFiles/cleanf2.dir/clean/clean2.cc.o


# Object files for target cleanf2
cleanf2_OBJECTS = \
"CMakeFiles/cleanf2.dir/clean/clean2.cc.o"

# External object files for target cleanf2
cleanf2_EXTERNAL_OBJECTS =

project2/cleanf2: project2/CMakeFiles/cleanf2.dir/clean/clean2.cc.o
project2/cleanf2: project2/CMakeFiles/cleanf2.dir/build.make
project2/cleanf2: project2/CMakeFiles/cleanf2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable cleanf2"
	cd /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build/project2 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cleanf2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
project2/CMakeFiles/cleanf2.dir/build: project2/cleanf2

.PHONY : project2/CMakeFiles/cleanf2.dir/build

project2/CMakeFiles/cleanf2.dir/requires: project2/CMakeFiles/cleanf2.dir/clean/clean2.cc.o.requires

.PHONY : project2/CMakeFiles/cleanf2.dir/requires

project2/CMakeFiles/cleanf2.dir/clean:
	cd /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build/project2 && $(CMAKE_COMMAND) -P CMakeFiles/cleanf2.dir/cmake_clean.cmake
.PHONY : project2/CMakeFiles/cleanf2.dir/clean

project2/CMakeFiles/cleanf2.dir/depend:
	cd /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/project2 /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build/project2 /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build/project2/CMakeFiles/cleanf2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : project2/CMakeFiles/cleanf2.dir/depend

