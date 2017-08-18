#include "varvisit.h"
#include "llvm/Support/raw_ostream.h"
#include "../utils/mk_debug.h"

using namespace llvm;

bool isFloatingPoint(Instruction &I) {
  Type *t = I.getType();
  return t->isFloatingPointTy();
}

bool isVector(Instruction &I) {
  Type *t = I.getType();
  return t->isVectorTy();
}

bool isPointer(Instruction &I) {
  Type *t = I.getType();
  return t->isPointerTy();
}

bool isInteger(Instruction &I) {
  Type *t = I.getType();
  return t->isIntegerTy();
}

bool isStructure(Instruction &I) {
  Type *t = I.getType();
  return t->isStructTy();
}

bool isArray(Instruction &I) {
  Type *t = I.getType();
  return t->isArrayTy();
}

VarVisitor::Vars VarVisitor::mergeVars(Vars v1, Vars v2) {
  Vars ret;

  // Preallocate space
  ret.ints.reserve(v1.ints.size() + v2.ints.size());
  ret.floats.reserve(v1.floats.size() + v2.floats.size());

  // Insert all the integers
  ret.ints.insert(ret.ints.end(), v1.ints.begin(), v1.ints.end());
  ret.ints.insert(ret.ints.end(), v2.ints.begin(), v2.ints.end());

  // Insert all the floats
  ret.floats.insert(ret.floats.end(), v1.floats.begin(), v1.floats.end());
  ret.floats.insert(ret.floats.end(), v2.floats.begin(), v2.floats.end());

  return ret;
}


// Checks the return type of the instruction and modifies the internal vars
// field accordingly
void VarVisitor::handleInstructionWithRet(Instruction &I) {
  if (isFloatingPoint(I)) {
    vars.floats.push_back(&I);
  }
  else if (isInteger(I) || isPointer(I)) {
    vars.ints.push_back(&I);
  }
  else if (isVector(I)) {
    assert(0 && "vectors are unhandled");
  }
  else if (isStructure(I)) {
    assert(0 && "structures are unhandled");
  }
  else if (isArray(I)) {
    assert(0 && "arrays are unhandled");
  }
  else {
    errs() << "[ERROR]: Unahandled type: " << I << '\n';
    assert(0 && "unhhandled  type; see above");
  }
}

void VarVisitor::visitPHINode(PHINode &phi) {
  handleInstructionWithRet(phi);

  //DEBUG_MSG("visiting phi node\n");
  //for (size_t i = 0; i < phi.getNumIncomingValues(); ++i) {
  //  Value *v = phi.getIncomingValue(i);
  //  if (Instruction * I = dyn_cast<Instruction>(v)) {
  //    if (isFloatingPoint(*I)) {
  //      DEBUG_MSG("\tfloat in phi\n");
  //      vars.floats.push_back(I);
  //    }
  //    else if (isInteger(*I) || isPointer(*I)) {
  //      DEBUG_MSG("\tinteger/pointer in phi");
  //      vars.ints.push_back(I);
  //    }
  //    else {
  //      errs() << "[ERROR] var visit: unhandled instruction in phi: " 
  //             << *I << '\n';
  //      exit(EXIT_FAILURE);
  //    }
  //  } // if (Instruction * I ...)
  //  // Constants are not variables
  //  else if (isa<ConstantInt>(v)) {
  //    DEBUG_MSG("\tconstant int in phi\n");
  //    continue;
  //  }
  //  else {
  //    errs() << "[ERROR] varvisit: unhandled value in phi node: " << *v 
  //           << '\n';
  //    exit(EXIT_FAILURE);
  //  }
  //}
}

void VarVisitor::visitBinaryOperator(BinaryOperator &I) {
  handleInstructionWithRet(I);
}

void VarVisitor::visitUnaryInstruction(UnaryInstruction &I) {
  handleInstructionWithRet(I);
}

void VarVisitor::visitCmpInst(CmpInst &I) {
  handleInstructionWithRet(I);
}

void VarVisitor::visitCastInst(CastInst &I) {
  handleInstructionWithRet(I);
}

void VarVisitor::visitAtomicRMWInst(AtomicRMWInst &I) {
  handleInstructionWithRet(I);
}
