## README

This repository is about thread modular abstract interpretation (Using the APRON library) 
for interrupt-driven programs by adopting priority information.

The source code is a modified version of Markus Kusano's source code.

Right now, all files are not cleaned up including test files.

However, subset of result tables can be printed since some output files are uploaded.


# Building

The program is an LLVM opt pass. It is built using CMake. 
Since we are using CMake, this likely requires LLVM version greater than or equal to 3.7 (tested on 3.7).

Modify the CMakeLists file variable APRON_PREFIX to be the location where Apron is installed

Assuming your LLVM library files are in a standard location (more on this below), simply:

    mkdir build
    cd build
    cmake ../
    make

The result of the build process is an .so file, libworklistAI.so

If you need to tell CMake where LLVM lives, you need to add the option -DLLVM_DIR, e.g.,

cmake -DLLVM_DIR=/home/chungha/src/share/llvm/cmake ../

The directory passed to LLVM_DIR should be the location where the LLVM CMake files are (e.g., LLVM-Config.cmake).

You also may need to set -DZ3_INC and -DZ3_LIB to the include directory, and library file for Z3, respectively.

# Test Path

You need to modify LLVM path and build path in src/test/export.sh file to test program such as LLVM_PREFIX, WORKLIST_SO, Z3_BIN.


# ASSERT Usage

If you want to check the program's invariants using assert, you have to use in this way.

Instead of using ``assert(a==1)``, you have to write down
``if (a != 1) { assert(0); }``.

Because of post dominator relation, I need to implement my tool to support only this case (assert makes the program unreachable so some post dominator relationship is lost).


# Directory description

- icbmc: This is the directory where I tested icbmc binary file from DATE15 paper with my benchmarks.

- src: It contains main implementation (worlist-ai, utils, CMakeLists.txt) and a test directory (test)

- src/test: It contains script files to run the test and benchmark programs. Also, the result files are included in each directory.

