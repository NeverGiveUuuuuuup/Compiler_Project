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
include CMakeFiles/boost.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/boost.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/boost.dir/flags.make

CMakeFiles/boost.dir/src/CCPrinter.cc.o: CMakeFiles/boost.dir/flags.make
CMakeFiles/boost.dir/src/CCPrinter.cc.o: ../src/CCPrinter.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/boost.dir/src/CCPrinter.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/boost.dir/src/CCPrinter.cc.o -c /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/src/CCPrinter.cc

CMakeFiles/boost.dir/src/CCPrinter.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boost.dir/src/CCPrinter.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/src/CCPrinter.cc > CMakeFiles/boost.dir/src/CCPrinter.cc.i

CMakeFiles/boost.dir/src/CCPrinter.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boost.dir/src/CCPrinter.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/src/CCPrinter.cc -o CMakeFiles/boost.dir/src/CCPrinter.cc.s

CMakeFiles/boost.dir/src/CCPrinter.cc.o.requires:

.PHONY : CMakeFiles/boost.dir/src/CCPrinter.cc.o.requires

CMakeFiles/boost.dir/src/CCPrinter.cc.o.provides: CMakeFiles/boost.dir/src/CCPrinter.cc.o.requires
	$(MAKE) -f CMakeFiles/boost.dir/build.make CMakeFiles/boost.dir/src/CCPrinter.cc.o.provides.build
.PHONY : CMakeFiles/boost.dir/src/CCPrinter.cc.o.provides

CMakeFiles/boost.dir/src/CCPrinter.cc.o.provides.build: CMakeFiles/boost.dir/src/CCPrinter.cc.o


CMakeFiles/boost.dir/src/IR.cc.o: CMakeFiles/boost.dir/flags.make
CMakeFiles/boost.dir/src/IR.cc.o: ../src/IR.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/boost.dir/src/IR.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/boost.dir/src/IR.cc.o -c /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/src/IR.cc

CMakeFiles/boost.dir/src/IR.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boost.dir/src/IR.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/src/IR.cc > CMakeFiles/boost.dir/src/IR.cc.i

CMakeFiles/boost.dir/src/IR.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boost.dir/src/IR.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/src/IR.cc -o CMakeFiles/boost.dir/src/IR.cc.s

CMakeFiles/boost.dir/src/IR.cc.o.requires:

.PHONY : CMakeFiles/boost.dir/src/IR.cc.o.requires

CMakeFiles/boost.dir/src/IR.cc.o.provides: CMakeFiles/boost.dir/src/IR.cc.o.requires
	$(MAKE) -f CMakeFiles/boost.dir/build.make CMakeFiles/boost.dir/src/IR.cc.o.provides.build
.PHONY : CMakeFiles/boost.dir/src/IR.cc.o.provides

CMakeFiles/boost.dir/src/IR.cc.o.provides.build: CMakeFiles/boost.dir/src/IR.cc.o


CMakeFiles/boost.dir/src/IRMutator.cc.o: CMakeFiles/boost.dir/flags.make
CMakeFiles/boost.dir/src/IRMutator.cc.o: ../src/IRMutator.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/boost.dir/src/IRMutator.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/boost.dir/src/IRMutator.cc.o -c /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/src/IRMutator.cc

CMakeFiles/boost.dir/src/IRMutator.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boost.dir/src/IRMutator.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/src/IRMutator.cc > CMakeFiles/boost.dir/src/IRMutator.cc.i

CMakeFiles/boost.dir/src/IRMutator.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boost.dir/src/IRMutator.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/src/IRMutator.cc -o CMakeFiles/boost.dir/src/IRMutator.cc.s

CMakeFiles/boost.dir/src/IRMutator.cc.o.requires:

.PHONY : CMakeFiles/boost.dir/src/IRMutator.cc.o.requires

CMakeFiles/boost.dir/src/IRMutator.cc.o.provides: CMakeFiles/boost.dir/src/IRMutator.cc.o.requires
	$(MAKE) -f CMakeFiles/boost.dir/build.make CMakeFiles/boost.dir/src/IRMutator.cc.o.provides.build
.PHONY : CMakeFiles/boost.dir/src/IRMutator.cc.o.provides

CMakeFiles/boost.dir/src/IRMutator.cc.o.provides.build: CMakeFiles/boost.dir/src/IRMutator.cc.o


CMakeFiles/boost.dir/src/IRPrinter.cc.o: CMakeFiles/boost.dir/flags.make
CMakeFiles/boost.dir/src/IRPrinter.cc.o: ../src/IRPrinter.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/boost.dir/src/IRPrinter.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/boost.dir/src/IRPrinter.cc.o -c /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/src/IRPrinter.cc

CMakeFiles/boost.dir/src/IRPrinter.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boost.dir/src/IRPrinter.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/src/IRPrinter.cc > CMakeFiles/boost.dir/src/IRPrinter.cc.i

CMakeFiles/boost.dir/src/IRPrinter.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boost.dir/src/IRPrinter.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/src/IRPrinter.cc -o CMakeFiles/boost.dir/src/IRPrinter.cc.s

CMakeFiles/boost.dir/src/IRPrinter.cc.o.requires:

.PHONY : CMakeFiles/boost.dir/src/IRPrinter.cc.o.requires

CMakeFiles/boost.dir/src/IRPrinter.cc.o.provides: CMakeFiles/boost.dir/src/IRPrinter.cc.o.requires
	$(MAKE) -f CMakeFiles/boost.dir/build.make CMakeFiles/boost.dir/src/IRPrinter.cc.o.provides.build
.PHONY : CMakeFiles/boost.dir/src/IRPrinter.cc.o.provides

CMakeFiles/boost.dir/src/IRPrinter.cc.o.provides.build: CMakeFiles/boost.dir/src/IRPrinter.cc.o


CMakeFiles/boost.dir/src/IRVisitor.cc.o: CMakeFiles/boost.dir/flags.make
CMakeFiles/boost.dir/src/IRVisitor.cc.o: ../src/IRVisitor.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/boost.dir/src/IRVisitor.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/boost.dir/src/IRVisitor.cc.o -c /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/src/IRVisitor.cc

CMakeFiles/boost.dir/src/IRVisitor.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boost.dir/src/IRVisitor.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/src/IRVisitor.cc > CMakeFiles/boost.dir/src/IRVisitor.cc.i

CMakeFiles/boost.dir/src/IRVisitor.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boost.dir/src/IRVisitor.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/src/IRVisitor.cc -o CMakeFiles/boost.dir/src/IRVisitor.cc.s

CMakeFiles/boost.dir/src/IRVisitor.cc.o.requires:

.PHONY : CMakeFiles/boost.dir/src/IRVisitor.cc.o.requires

CMakeFiles/boost.dir/src/IRVisitor.cc.o.provides: CMakeFiles/boost.dir/src/IRVisitor.cc.o.requires
	$(MAKE) -f CMakeFiles/boost.dir/build.make CMakeFiles/boost.dir/src/IRVisitor.cc.o.provides.build
.PHONY : CMakeFiles/boost.dir/src/IRVisitor.cc.o.provides

CMakeFiles/boost.dir/src/IRVisitor.cc.o.provides.build: CMakeFiles/boost.dir/src/IRVisitor.cc.o


# Object files for target boost
boost_OBJECTS = \
"CMakeFiles/boost.dir/src/CCPrinter.cc.o" \
"CMakeFiles/boost.dir/src/IR.cc.o" \
"CMakeFiles/boost.dir/src/IRMutator.cc.o" \
"CMakeFiles/boost.dir/src/IRPrinter.cc.o" \
"CMakeFiles/boost.dir/src/IRVisitor.cc.o"

# External object files for target boost
boost_EXTERNAL_OBJECTS =

libboost.so: CMakeFiles/boost.dir/src/CCPrinter.cc.o
libboost.so: CMakeFiles/boost.dir/src/IR.cc.o
libboost.so: CMakeFiles/boost.dir/src/IRMutator.cc.o
libboost.so: CMakeFiles/boost.dir/src/IRPrinter.cc.o
libboost.so: CMakeFiles/boost.dir/src/IRVisitor.cc.o
libboost.so: CMakeFiles/boost.dir/build.make
libboost.so: CMakeFiles/boost.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX shared library libboost.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/boost.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/boost.dir/build: libboost.so

.PHONY : CMakeFiles/boost.dir/build

CMakeFiles/boost.dir/requires: CMakeFiles/boost.dir/src/CCPrinter.cc.o.requires
CMakeFiles/boost.dir/requires: CMakeFiles/boost.dir/src/IR.cc.o.requires
CMakeFiles/boost.dir/requires: CMakeFiles/boost.dir/src/IRMutator.cc.o.requires
CMakeFiles/boost.dir/requires: CMakeFiles/boost.dir/src/IRPrinter.cc.o.requires
CMakeFiles/boost.dir/requires: CMakeFiles/boost.dir/src/IRVisitor.cc.o.requires

.PHONY : CMakeFiles/boost.dir/requires

CMakeFiles/boost.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/boost.dir/cmake_clean.cmake
.PHONY : CMakeFiles/boost.dir/clean

CMakeFiles/boost.dir/depend:
	cd /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build /mnt/d/codes/CompilerLab/Compiler_Project/CompilerProject-2020Spring/build/CMakeFiles/boost.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/boost.dir/depend

