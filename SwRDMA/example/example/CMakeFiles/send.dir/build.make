# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/amax/hhj/rc4ml_qdma

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/amax/hhj/rc4ml_qdma/example

# Include any dependencies generated for this target.
include example/CMakeFiles/send.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include example/CMakeFiles/send.dir/compiler_depend.make

# Include the progress variables for this target.
include example/CMakeFiles/send.dir/progress.make

# Include the compile flags for this target's objects.
include example/CMakeFiles/send.dir/flags.make

example/CMakeFiles/send.dir/send.cpp.o: example/CMakeFiles/send.dir/flags.make
example/CMakeFiles/send.dir/send.cpp.o: send.cpp
example/CMakeFiles/send.dir/send.cpp.o: example/CMakeFiles/send.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amax/hhj/rc4ml_qdma/example/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object example/CMakeFiles/send.dir/send.cpp.o"
	cd /home/amax/hhj/rc4ml_qdma/example/example && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT example/CMakeFiles/send.dir/send.cpp.o -MF CMakeFiles/send.dir/send.cpp.o.d -o CMakeFiles/send.dir/send.cpp.o -c /home/amax/hhj/rc4ml_qdma/example/send.cpp

example/CMakeFiles/send.dir/send.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/send.dir/send.cpp.i"
	cd /home/amax/hhj/rc4ml_qdma/example/example && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amax/hhj/rc4ml_qdma/example/send.cpp > CMakeFiles/send.dir/send.cpp.i

example/CMakeFiles/send.dir/send.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/send.dir/send.cpp.s"
	cd /home/amax/hhj/rc4ml_qdma/example/example && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amax/hhj/rc4ml_qdma/example/send.cpp -o CMakeFiles/send.dir/send.cpp.s

# Object files for target send
send_OBJECTS = \
"CMakeFiles/send.dir/send.cpp.o"

# External object files for target send
send_EXTERNAL_OBJECTS =

example/send: example/CMakeFiles/send.dir/send.cpp.o
example/send: example/CMakeFiles/send.dir/build.make
example/send: libqdma.so
example/send: example/CMakeFiles/send.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/amax/hhj/rc4ml_qdma/example/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable send"
	cd /home/amax/hhj/rc4ml_qdma/example/example && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/send.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/CMakeFiles/send.dir/build: example/send
.PHONY : example/CMakeFiles/send.dir/build

example/CMakeFiles/send.dir/clean:
	cd /home/amax/hhj/rc4ml_qdma/example/example && $(CMAKE_COMMAND) -P CMakeFiles/send.dir/cmake_clean.cmake
.PHONY : example/CMakeFiles/send.dir/clean

example/CMakeFiles/send.dir/depend:
	cd /home/amax/hhj/rc4ml_qdma/example && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/amax/hhj/rc4ml_qdma /home/amax/hhj/rc4ml_qdma/example /home/amax/hhj/rc4ml_qdma/example /home/amax/hhj/rc4ml_qdma/example/example /home/amax/hhj/rc4ml_qdma/example/example/CMakeFiles/send.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/CMakeFiles/send.dir/depend

