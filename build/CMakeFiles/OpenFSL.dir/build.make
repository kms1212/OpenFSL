# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /mnt/f/내문서/GitHub/OpenFSL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/f/내문서/GitHub/OpenFSL/build

# Include any dependencies generated for this target.
include CMakeFiles/openfsl.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/openfsl.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/openfsl.dir/flags.make

CMakeFiles/openfsl.dir/src/device.cpp.o: CMakeFiles/openfsl.dir/flags.make
CMakeFiles/openfsl.dir/src/device.cpp.o: ../src/device.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/f/내문서/GitHub/OpenFSL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/openfsl.dir/src/device.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/openfsl.dir/src/device.cpp.o -c /mnt/f/내문서/GitHub/OpenFSL/src/device.cpp

CMakeFiles/openfsl.dir/src/device.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/openfsl.dir/src/device.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/f/내문서/GitHub/OpenFSL/src/device.cpp > CMakeFiles/openfsl.dir/src/device.cpp.i

CMakeFiles/openfsl.dir/src/device.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/openfsl.dir/src/device.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/f/내문서/GitHub/OpenFSL/src/device.cpp -o CMakeFiles/openfsl.dir/src/device.cpp.s

CMakeFiles/openfsl.dir/src/diskdevice.cpp.o: CMakeFiles/openfsl.dir/flags.make
CMakeFiles/openfsl.dir/src/diskdevice.cpp.o: ../src/diskdevice.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/f/내문서/GitHub/OpenFSL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/openfsl.dir/src/diskdevice.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/openfsl.dir/src/diskdevice.cpp.o -c /mnt/f/내문서/GitHub/OpenFSL/src/diskdevice.cpp

CMakeFiles/openfsl.dir/src/diskdevice.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/openfsl.dir/src/diskdevice.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/f/내문서/GitHub/OpenFSL/src/diskdevice.cpp > CMakeFiles/openfsl.dir/src/diskdevice.cpp.i

CMakeFiles/openfsl.dir/src/diskdevice.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/openfsl.dir/src/diskdevice.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/f/내문서/GitHub/OpenFSL/src/diskdevice.cpp -o CMakeFiles/openfsl.dir/src/diskdevice.cpp.s

CMakeFiles/openfsl.dir/src/logdevice.cpp.o: CMakeFiles/openfsl.dir/flags.make
CMakeFiles/openfsl.dir/src/logdevice.cpp.o: ../src/logdevice.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/f/내문서/GitHub/OpenFSL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/openfsl.dir/src/logdevice.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/openfsl.dir/src/logdevice.cpp.o -c /mnt/f/내문서/GitHub/OpenFSL/src/logdevice.cpp

CMakeFiles/openfsl.dir/src/logdevice.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/openfsl.dir/src/logdevice.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/f/내문서/GitHub/OpenFSL/src/logdevice.cpp > CMakeFiles/openfsl.dir/src/logdevice.cpp.i

CMakeFiles/openfsl.dir/src/logdevice.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/openfsl.dir/src/logdevice.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/f/내문서/GitHub/OpenFSL/src/logdevice.cpp -o CMakeFiles/openfsl.dir/src/logdevice.cpp.s

CMakeFiles/openfsl.dir/src/sector.cpp.o: CMakeFiles/openfsl.dir/flags.make
CMakeFiles/openfsl.dir/src/sector.cpp.o: ../src/sector.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/f/내문서/GitHub/OpenFSL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/openfsl.dir/src/sector.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/openfsl.dir/src/sector.cpp.o -c /mnt/f/내문서/GitHub/OpenFSL/src/sector.cpp

CMakeFiles/openfsl.dir/src/sector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/openfsl.dir/src/sector.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/f/내문서/GitHub/OpenFSL/src/sector.cpp > CMakeFiles/openfsl.dir/src/sector.cpp.i

CMakeFiles/openfsl.dir/src/sector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/openfsl.dir/src/sector.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/f/내문서/GitHub/OpenFSL/src/sector.cpp -o CMakeFiles/openfsl.dir/src/sector.cpp.s

CMakeFiles/openfsl.dir/src/serviceswrapper.cpp.o: CMakeFiles/openfsl.dir/flags.make
CMakeFiles/openfsl.dir/src/serviceswrapper.cpp.o: ../src/serviceswrapper.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/f/내문서/GitHub/OpenFSL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/openfsl.dir/src/serviceswrapper.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/openfsl.dir/src/serviceswrapper.cpp.o -c /mnt/f/내문서/GitHub/OpenFSL/src/serviceswrapper.cpp

CMakeFiles/openfsl.dir/src/serviceswrapper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/openfsl.dir/src/serviceswrapper.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/f/내문서/GitHub/OpenFSL/src/serviceswrapper.cpp > CMakeFiles/openfsl.dir/src/serviceswrapper.cpp.i

CMakeFiles/openfsl.dir/src/serviceswrapper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/openfsl.dir/src/serviceswrapper.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/f/내문서/GitHub/OpenFSL/src/serviceswrapper.cpp -o CMakeFiles/openfsl.dir/src/serviceswrapper.cpp.s

# Object files for target openfsl
openfsl_OBJECTS = \
"CMakeFiles/openfsl.dir/src/device.cpp.o" \
"CMakeFiles/openfsl.dir/src/diskdevice.cpp.o" \
"CMakeFiles/openfsl.dir/src/logdevice.cpp.o" \
"CMakeFiles/openfsl.dir/src/sector.cpp.o" \
"CMakeFiles/openfsl.dir/src/serviceswrapper.cpp.o"

# External object files for target openfsl
openfsl_EXTERNAL_OBJECTS =

libopenfsl.a: CMakeFiles/openfsl.dir/src/device.cpp.o
libopenfsl.a: CMakeFiles/openfsl.dir/src/diskdevice.cpp.o
libopenfsl.a: CMakeFiles/openfsl.dir/src/logdevice.cpp.o
libopenfsl.a: CMakeFiles/openfsl.dir/src/sector.cpp.o
libopenfsl.a: CMakeFiles/openfsl.dir/src/serviceswrapper.cpp.o
libopenfsl.a: CMakeFiles/openfsl.dir/build.make
libopenfsl.a: CMakeFiles/openfsl.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/f/내문서/GitHub/OpenFSL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX static library libopenfsl.a"
	$(CMAKE_COMMAND) -P CMakeFiles/openfsl.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/openfsl.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/openfsl.dir/build: libopenfsl.a

.PHONY : CMakeFiles/openfsl.dir/build

CMakeFiles/openfsl.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/openfsl.dir/cmake_clean.cmake
.PHONY : CMakeFiles/openfsl.dir/clean

CMakeFiles/openfsl.dir/depend:
	cd /mnt/f/내문서/GitHub/OpenFSL/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/f/내문서/GitHub/OpenFSL /mnt/f/내문서/GitHub/OpenFSL /mnt/f/내문서/GitHub/OpenFSL/build /mnt/f/내문서/GitHub/OpenFSL/build /mnt/f/내문서/GitHub/OpenFSL/build/CMakeFiles/openfsl.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/openfsl.dir/depend

