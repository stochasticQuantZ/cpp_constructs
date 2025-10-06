# Steps
- mkdir build 
- cd build 
- cmake .. (to generate build files)
- cmake --build . (compile)
- ./executable

# Also
- mkdir build && cd build 
- cmake -S < path of source , typically root directoryb of the project> -B < path to build >
- cmake is a generator 
- cmake is not a build system
- tool to use some generator
- generator (build file based on generator)
- makefile, Vs code , Ninja etc
- we need not care what are the build systems
- cmake --help 
- The following generators are available on this platform (* marks default):
*** Unix Makefiles               = Generates standard UNIX makefiles.
  Ninja                        = Generates build.ninja files.
  Ninja Multi-Config           = Generates build-<Config>.ninja files.
  Watcom WMake                 = Generates Watcom WMake makefiles.
  Xcode                        = Generate Xcode project files.
  CodeBlocks - Ninja           = Generates CodeBlocks project files
                                 (deprecated).
  CodeBlocks - Unix Makefiles  = Generates CodeBlocks project files
                                 (deprecated).
  CodeLite - Ninja             = Generates CodeLite project files
                                 (deprecated).
  CodeLite - Unix Makefiles    = Generates CodeLite project files
                                 (deprecated).**

# Cmake
- add_library( Library  STATIC source.cc)
- add_execuatbale( Executable main.cc)
- target_link_libraries( Executable  PUBLIC Library)

- cmake --build . --target < Library name>      [ Now only library compiles ]


# Project structure
- src : carries the library
- app/test: carries the driver or test code e.g main.cpp
- If we are using header files in a source file and that is NOT the same in the source file, then we get the error about headers
- It is good practice to create separate CMake where sourec and headers are there

# Variable: 
- CMAKE_CXX_STANDARD
- CMAKE_CXX_STANDARD_REQUIRED
- 