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
include CMakeFiles/net_address_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/net_address_test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/net_address_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/net_address_test.dir/flags.make

CMakeFiles/net_address_test.dir/test/core/net_address_test.cc.o: CMakeFiles/net_address_test.dir/flags.make
CMakeFiles/net_address_test.dir/test/core/net_address_test.cc.o: ../test/core/net_address_test.cc
CMakeFiles/net_address_test.dir/test/core/net_address_test.cc.o: CMakeFiles/net_address_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/shaoguixin/Icee/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/net_address_test.dir/test/core/net_address_test.cc.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/net_address_test.dir/test/core/net_address_test.cc.o -MF CMakeFiles/net_address_test.dir/test/core/net_address_test.cc.o.d -o CMakeFiles/net_address_test.dir/test/core/net_address_test.cc.o -c /Users/shaoguixin/Icee/test/core/net_address_test.cc

CMakeFiles/net_address_test.dir/test/core/net_address_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/net_address_test.dir/test/core/net_address_test.cc.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/shaoguixin/Icee/test/core/net_address_test.cc > CMakeFiles/net_address_test.dir/test/core/net_address_test.cc.i

CMakeFiles/net_address_test.dir/test/core/net_address_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/net_address_test.dir/test/core/net_address_test.cc.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/shaoguixin/Icee/test/core/net_address_test.cc -o CMakeFiles/net_address_test.dir/test/core/net_address_test.cc.s

# Object files for target net_address_test
net_address_test_OBJECTS = \
"CMakeFiles/net_address_test.dir/test/core/net_address_test.cc.o"

# External object files for target net_address_test
net_address_test_EXTERNAL_OBJECTS =

net_address_test: CMakeFiles/net_address_test.dir/test/core/net_address_test.cc.o
net_address_test: CMakeFiles/net_address_test.dir/build.make
net_address_test: _deps/doctest-build/libdoctest_with_main.a
net_address_test: libicee_core.a
net_address_test: libicee_log.a
net_address_test: CMakeFiles/net_address_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/shaoguixin/Icee/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable net_address_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/net_address_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/net_address_test.dir/build: net_address_test
.PHONY : CMakeFiles/net_address_test.dir/build

CMakeFiles/net_address_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/net_address_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/net_address_test.dir/clean

CMakeFiles/net_address_test.dir/depend:
	cd /Users/shaoguixin/Icee/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/shaoguixin/Icee /Users/shaoguixin/Icee /Users/shaoguixin/Icee/build /Users/shaoguixin/Icee/build /Users/shaoguixin/Icee/build/CMakeFiles/net_address_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/net_address_test.dir/depend

