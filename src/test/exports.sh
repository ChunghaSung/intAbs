#!/bin/sh

# modify according to your install directory for LLVM (this chould contain the
# bin, lib, include, ...  directories
#LLVM_PREFIX=/usr
LLVM_PREFIX=/home/chungha/base_llvm/llvm/build

# Helps Duet find its way

# Location of WorklistAI .so file (debug)
#WORKLIST_SO=/home/markus/src/LLVMApron/debug/worklist-ai/libworklistAI.so
WORKLIST_SO=/home/chungha/work/interruptabstract/src/build/worklist-ai/libworklistAI.so

# Location of optimized SO
#WORKLIST_OPT_SO=/home/markus/src/LLVMApron/release/worklist-ai/libworklistAI.so


# Location of Z3 binary
Z3_BIN=/usr/bin/z3

# Old version of Z3 supporting old datalog query syntax
# see: https://github.com/Z3Prover/z3/issues/428
Z3_OLD_BIN=/home/markus/src/z3_old/install/bin/z3

##### Everything below here should not need to be modified
OPT=${LLVM_PREFIX}/bin/opt
CLANG=${LLVM_PREFIX}/bin/clang
CLANGPP=${LLVM_PREFIX}/bin/clang++
LLVM_DIS=${LLVM_PREFIX}/bin/llvm-dis
LLVM_AS=${LLVM_PREFIX}/bin/llvm-as

# Helper variable holding the bulk of the opt command to run the pass. Just
# pass the additional options like -constraints, -oct, -box, ..., along with
# the .ll/.bc file
LLAPRON="$OPT -load $WORKLIST_SO -worklist-ai -z3 $Z3_BIN"

# Helper variable to run the program dependence pass just put the .ll/bc file
# at the end
#LLPDG="$OPT -load $DYN_PDG_SO -contextinsen-dynpdg -mdassert -assert -z3 $Z3_BIN"
