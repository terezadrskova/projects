# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /homes/td613/Documents/graphics/cwk_5/cgcoursework

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /homes/td613/Documents/graphics/cwk_5/cgcoursework

# Include any dependencies generated for this target.
include CMakeFiles/cgExercise04b.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/cgExercise04b.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cgExercise04b.dir/flags.make

CMakeFiles/cgExercise04b.dir/main04b.o: CMakeFiles/cgExercise04b.dir/flags.make
CMakeFiles/cgExercise04b.dir/main04b.o: main04b.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /homes/td613/Documents/graphics/cwk_5/cgcoursework/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/cgExercise04b.dir/main04b.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/cgExercise04b.dir/main04b.o -c /homes/td613/Documents/graphics/cwk_5/cgcoursework/main04b.cpp

CMakeFiles/cgExercise04b.dir/main04b.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cgExercise04b.dir/main04b.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /homes/td613/Documents/graphics/cwk_5/cgcoursework/main04b.cpp > CMakeFiles/cgExercise04b.dir/main04b.i

CMakeFiles/cgExercise04b.dir/main04b.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cgExercise04b.dir/main04b.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /homes/td613/Documents/graphics/cwk_5/cgcoursework/main04b.cpp -o CMakeFiles/cgExercise04b.dir/main04b.s

CMakeFiles/cgExercise04b.dir/main04b.o.requires:
.PHONY : CMakeFiles/cgExercise04b.dir/main04b.o.requires

CMakeFiles/cgExercise04b.dir/main04b.o.provides: CMakeFiles/cgExercise04b.dir/main04b.o.requires
	$(MAKE) -f CMakeFiles/cgExercise04b.dir/build.make CMakeFiles/cgExercise04b.dir/main04b.o.provides.build
.PHONY : CMakeFiles/cgExercise04b.dir/main04b.o.provides

CMakeFiles/cgExercise04b.dir/main04b.o.provides.build: CMakeFiles/cgExercise04b.dir/main04b.o

# Object files for target cgExercise04b
cgExercise04b_OBJECTS = \
"CMakeFiles/cgExercise04b.dir/main04b.o"

# External object files for target cgExercise04b
cgExercise04b_EXTERNAL_OBJECTS =

cgExercise04b: CMakeFiles/cgExercise04b.dir/main04b.o
cgExercise04b: CMakeFiles/cgExercise04b.dir/build.make
cgExercise04b: /usr/lib/x86_64-linux-gnu/libGL.so
cgExercise04b: /usr/lib/x86_64-linux-gnu/libGLU.so
cgExercise04b: /usr/lib/x86_64-linux-gnu/libGLEW.so
cgExercise04b: /usr/lib/x86_64-linux-gnu/libglut.so
cgExercise04b: CMakeFiles/cgExercise04b.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable cgExercise04b"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cgExercise04b.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cgExercise04b.dir/build: cgExercise04b
.PHONY : CMakeFiles/cgExercise04b.dir/build

CMakeFiles/cgExercise04b.dir/requires: CMakeFiles/cgExercise04b.dir/main04b.o.requires
.PHONY : CMakeFiles/cgExercise04b.dir/requires

CMakeFiles/cgExercise04b.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cgExercise04b.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cgExercise04b.dir/clean

CMakeFiles/cgExercise04b.dir/depend:
	cd /homes/td613/Documents/graphics/cwk_5/cgcoursework && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /homes/td613/Documents/graphics/cwk_5/cgcoursework /homes/td613/Documents/graphics/cwk_5/cgcoursework /homes/td613/Documents/graphics/cwk_5/cgcoursework /homes/td613/Documents/graphics/cwk_5/cgcoursework /homes/td613/Documents/graphics/cwk_5/cgcoursework/CMakeFiles/cgExercise04b.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cgExercise04b.dir/depend

