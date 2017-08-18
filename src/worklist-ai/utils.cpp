/** 
 * Author: Markus Kusano
 *
 * Implementation of utils.h
 */
#include "utils.h"

#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/CommandLine.h"

#include "box.h"
#include "oct.h"
#include "pk.h"
#include "pkeq.h"

#include <sstream>
#include <iomanip>
#include <cassert>

using namespace llvm;

// Initialized in worklistai.cpp
extern cl::opt<bool> useBoxG;
extern cl::opt<bool> useOctG;
extern cl::opt<bool> usePolyG;
extern cl::opt<bool> useLinEqG;

// Depending on the command line flags, return the appropriate manager.
ap_manager_t *Utils::getManager() {
  // TODO: Probablly wanna set the options here (widening?)
  if (useBoxG) {
    return box_manager_alloc();
  }
  else if (useOctG) {
    return oct_manager_alloc();
  }
  else if (usePolyG) {
    // The boolean arg eneables strict mode (e.g., x > 0)
    return pk_manager_alloc(true);
  }
  else {
    return pkeq_manager_alloc();
  }
}

std::string Utils::valueToStringUnsafe(
    const std::map<llvm::Value*, std::string> v2n
    , llvm::Value * v) {
  return mapAtUnsafe(v2n, v);
//  std::string ret;
//  try {
//    ret = v2n.at(v);
//  }
//  catch (const std::out_of_range &oor) {
//    errs() << "[ERROR] valueToStringUnsafe: value not in var map\n";
//    assert(0 && "see above");
//    exit(EXIT_FAILURE);
//  }
//  return ret;
}

bool Utils::isIntegerGlobal(GlobalVariable *gv) {
  PointerType *pt = gv->getType();
  // The type the pointer is pointing to
  Type *ty = pt->getElementType();
  if (ty->isIntegerTy()) {
    return true;
  }
  return false;
}

int Utils::getConstantIntUnsafe(const ConstantInt * const ci) {
  APInt ap = ci->getValue();
  // LLVM sizes are in bits
  assert(ap.isSignedIntN(sizeof(int) * 8) && "constant too large");
  int64_t sextVal;
  if (ap.isNegative()) {
    sextVal = ci->getSExtValue();
  }
  else {
    sextVal = ci->getZExtValue();
  }
  assert(sextVal <= INT_MAX && "constant too large");
  return (int) sextVal;

  //int val = ci->getLimitedValue(INT_MAX);
  //if (val == INT_MAX) {
  //  errs() << "[ERROR] constant int out-of-range\n";
  //  errs() << "  " << *ci << '\n';
  //  assert(0 && "see above");
  //  exit(EXIT_FAILURE);
  //}
  //return val;
}


bool Utils::isFloatGlobal(GlobalVariable *gv) {
  PointerType *pt = gv->getType();
  // The type the pointer is pointing to
  Type *ty = pt->getElementType();
  if (ty->isFloatingPointTy()) {
    return true;
  }
  return false;
}

// Return true if the passed instruction is a memory load. This includes
// LoadInst and atomic RMW operations
bool Utils::isSomeLoad(Instruction *i) {
  if (isa<LoadInst>(i)) {
    return true;
  }
  else if (isa<AtomicRMWInst>(i)) {
    return true;
  }
  else if (isa<AtomicCmpXchgInst>(i)) {
    return true;
  }
  else {
    return false;
  }
}

// Same as isSomeLoad but for a store
bool Utils::isSomeStore(Instruction *i) {
  if (isa<StoreInst>(i)) {
    return true;
  }
  else if (isa<AtomicRMWInst>(i)) {
    return true;
  }
  else if (isa<AtomicCmpXchgInst>(i)) {
    return true;
  }
  else {
    return false;
  }
}

// Given some store instruction (StoreInst, AtomicRMW) return its pointer operand
// Return NULL if `i` is not a store
Value *Utils::getStorePtr(Instruction *i) {
  // Get the pointer operand of any store
  // TODO: is this a complete list of stores?
  if (StoreInst *st = dyn_cast<StoreInst>(i)) {
    return st->getPointerOperand();
  }
  else if (AtomicRMWInst *ari = dyn_cast<AtomicRMWInst>(i)) {
    return ari->getPointerOperand();
  }
  else if (AtomicCmpXchgInst *acmpx = dyn_cast<AtomicCmpXchgInst>(i)) {
    return acmpx->getPointerOperand();
  }
  return NULL;
}
