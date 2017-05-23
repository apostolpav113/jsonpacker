# JSON Packer
JSON Packer is the utility for converting file containig JSON records separated by line into TLV format and vise versa

# Installation
You need the following to build the executable
- Boost libraries
- pthreads (for Google Test)
- GNU make, gcc 
- in order to generate a Makefile for your platform, you need cmake
- in order to generate documentation, you need doxygen

Compilation is done by performing the following steps:
1. Download source archive or clone GIT repository
2. Create a build directory (for instance inside the source tree)
```  
cd jsonpacker
mkdir build
cd build
```
3. Run cmake with the makefile generator
```
cmake ..
```
4. Compile the program by running make:
```
make
```
The program should compile and the binaries (json_packer and json_packer_tests) should be available in the bin directory within the build directory.

5. Optional. Generate documentation.
```
cmake -DBUILD_DOCUMENTATION=ON ..
make doc
```
In current version documentation is generated in the doc/doc directory within source directory (i.e. jsonpacker/doc/doc)
