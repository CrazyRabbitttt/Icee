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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.22.2/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.22.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/shaoguixin/Icee

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/shaoguixin/Icee/build

# Include any dependencies generated for this target.
include CMakeFiles/buffer_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/buffer_test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/buffer_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/buffer_test.dir/flags.make

CMakeFiles/buffer_test.dir/test/core/buffer_test.cc.o: CMakeFiles/buffer_test.dir/flags.make
CMakeFiles/buffer_test.dir/test/core/buffer_test.cc.o: ../test/core/buffer_test.cc
CMakeFiles/buffer_test.dir/test/core/buffer_test.cc.o: CMakeFiles/buffer_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/shaoguixin/Icee/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/buffer_test.dir/test/core/buffer_test.cc.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/buffer_test.dir/test/core/buffer_test.cc.o -MF CMakeFiles/buffer_test.dir/test/core/buffer_test.cc.o.d -o CMakeFiles/buffer_test.dir/test/core/buffer_test.cc.o -c /Users/shaoguixin/Icee/test/core/buffer_test.cc

CMakeFiles/buffer_test.dir/test/core/buffer_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/buffer_test.dir/test/core/buffer_test.cc.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/shaoguixin/Icee/test/core/buffer_test.cc > CMakeFiles/buffer_test.dir/test/core/buffer_test.cc.i

CMakeFiles/buffer_test.dir/test/core/buffer_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/buffer_test.dir/test/core/buffer_test.cc.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/shaoguixin/Icee/test/core/buffer_test.cc -o CMakeFiles/buffer_test.dir/test/core/buffer_test.cc.s

# Object files for target buffer_test
buffer_test_OBJECTS = \
"CMakeFiles/buffer_test.dir/test/core/buffer_test.cc.o"

# External object files for target buffer_test
buffer_test_EXTERNAL_OBJECTS =

buffer_test: CMakeFiles/buffer_test.dir/test/core/buffer_test.cc.o
buffer_test: CMakeFiles/buffer_test.dir/build.make
buffer_test: _deps/doctest-build/libdoctest_with_main.a
buffer_test: libicee_core.a
buffer_test: libicee_log.a
buffer_test: CMakeFiles/buffer_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/shaoguixin/Icee/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable buffer_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/buffer_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/buffer_test.dir/build: buffer_test
.PHONY : CMakeFiles/buffer_test.dir/build

CMakeFiles/buffer_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/buffer_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/buffer_test.dir/clean

CMakeFiles/buffer_test.dir/depend:
	cd /Users/shaoguixin/Icee/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/shaoguixin/Icee /Users/shaoguixin/Icee /Users/shaoguixin/Icee/build /Users/shaoguixin/Icee/build /Users/shaoguixin/Icee/build/CMakeFiles/buffer_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/buffer_test.dir/depend

