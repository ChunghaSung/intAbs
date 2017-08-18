/**
 * Author: Markus Kusano
 *
 * Implementation of bbops.h
 */

#include "bbops.h"
#include "utils.h"

#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/ConstantFolding.h"

#include "../utils/mk_debug.h"

using namespace llvm;

// Defined in worklistai.cpp, lets us see if weak memory test is
// being used
extern cl::opt<bool> tsoConstrG;
extern cl::opt<bool> psoConstrG;
extern cl::opt<bool> rmoConstrG;
extern cl::opt<bool> assertSliceG;
extern cl::opt<bool> impactG;

std::vector<Instruction*> BBOps::getBody(BasicBlock *b) {
  std::vector<Instruction*> body;
  for (auto bi = b->begin(), be = b->end(); bi != be; ++bi) {
    // Assumption: the iterator for the instructions in the BasicBlock is
    // ordered
    Instruction &i = *bi;
    if (&i == b->getTerminator()) {
      break;
    }
    body.push_back(&i);
  }
  return body;
}

// Update the passed LatticeFact such that `to <- from`
LatticeFact BBOps::performAssign(Value *to
    , Value *from
    , const LatticeFact f
    , const Module *m
    , const std::map<Value*, std::string> v2n) {
	DEBUG_MSG("[DEBUG] performAssign()\n");
  std::string toVar;
  std::string fromVar;

  // You can never assign to a constant
  toVar = Utils::valueToStringUnsafe(v2n, to);

  if (ConstantInt *ci = dyn_cast<ConstantInt>(from)) {
    int val = Utils::getConstantIntUnsafe(ci);
    return f.assign(toVar, val);
  }
  else if (isa<ConstantPointerNull>(from)) {
    return f.assign(toVar, 0);
  }
  else if (ConstantExpr *ce = dyn_cast<ConstantExpr>(from)) {
    Constant *cef = foldConstExpr(ce, m);
    if (ConstantInt *cefi = dyn_cast<ConstantInt>(cef)) {
      int val = Utils::getConstantIntUnsafe(cefi);
      return f.assign(toVar, val);
    }
    else if (isa<ConstantPointerNull>(from)) {
      return f.assign(toVar, 0);
    }
    else {
      errs() << "[ERROR] Unable to fold ConstantExpr: " << *ce << '\n';
      if (cef) {
        errs() << "Folded result: " << *cef << '\n';
      }
      assert(0 && "see above");
      exit(EXIT_FAILURE);
    }
  }
  //else if (GlobalVariable *gv = dyn_cast<GlobalVariable>(from)) {
  //  errs() << "GlobalVariable: " << *gv << '\n';
  //  assert(0 && "unimplemented: assign from GlobalVariable");
  //}
  //else if (Constant *c = dyn_cast<Constant>(from)) {
  //  errs() << "Constant: " << *c << '\n';
  //  assert(0 && "unimplemented: assign from Constant");
  //}

  DEBUG_MSG("[DEBUG] from value: " << *from << '\n');
  fromVar = Utils::valueToStringUnsafe(v2n, from);
  return f.assign(toVar, fromVar);
}


// Perform an atomicrmw instruction. `in` in the input fact to the instruction,
// `interf` the interference; if there is no interference, just set to bottom.
//
// `nonCombinInterf` should be true if combinational interferences are used,
// i.e., each interference combination is tested individually.
LatticeFact BBOps::performAtomicRMW(AtomicRMWInst *rmw, const LatticeFact in
    , const LatticeFact interf, bool nonCombinInterfs
    , std::map<Value *, std::string> v2n) {
  // Regardless of the operation the return value of the atomicrmw is the value
  // at the pointer's memory location.
  //
  // Note: it is assumed that this operation is performed before the actual
  // RMW. See the comments below.
  Value *p = rmw->getPointerOperand();
  LatticeFact inWithRet = performAssignWithInterf(rmw
      , rmw->getParent()->getParent()->getParent(), p, in, interf
      , nonCombinInterfs, v2n);
  
  // The atomicrmw instruction has a "sub field" within the instruction for
  // different operations
  //
  // Note: the atomicrmw also is a synchronization operation depending on its
  // ordering. This isn't handled at the moment.
  AtomicRMWInst::BinOp op = rmw->getOperation();
  if (op == AtomicRMWInst::BinOp::Add) {
    Value *v = rmw->getValOperand();
    // The current value of `p`, including those from the interferences, is
    // stored within the return of the atomicrmw (see the operation above).
    LatticeFact out = performBinOp(BinaryOperator::Add, p, rmw, v
        , inWithRet, v2n);
    return out;
  }
  else {
    errs() << "[ERROR] Unhandled AtomicRMW Operation: " << *rmw << '\n';
    exit(EXIT_FAILURE);
  }
}

// perform an atomic rmw without an interference
//
// From the LLVM language reference the semantics are:
//
// The contents of memory at the location specified by the ‘<pointer>‘ operand
// are atomically read, modified, and written back. The original value at the
// location is returned. The modification is specified by the operation
// argument:
LatticeFact BBOps::performAtomicRMW(AtomicRMWInst *rmw, const LatticeFact in
    , std::map<Value *, std::string> v2n) {

  // The return value of the atomic rmw is the old value of the pointer
  // regardless of the operation type
  Value *ptr = rmw->getPointerOperand();
  LatticeFact inWithRet = performAssign(rmw, ptr, in
      , rmw->getParent()->getParent()->getParent(), v2n);

  AtomicRMWInst::BinOp op = rmw->getOperation();
  if (op == AtomicRMWInst::BinOp::Add) {
    // Modify the memory location: p = p + v
    LatticeFact out = performBinOp(BinaryOperator::Add
        , ptr
        , ptr
        , rmw->getValOperand()
        , inWithRet
        , v2n);
    return out;
  }
  else {
    errs() << "[ERROR] Unhandled AtomicRMW Operation: " << *rmw << '\n';
    exit(EXIT_FAILURE);
  }
}

//LatticeFact BBOps::performAdd(Value *to
//    , Value *op0
//    , Value *op1
//    , LatticeFact f
//    , const ::std::map<Value *, std::string> v2n) {
//  std::string toStr = Utils::valueToStringUnsafe(v2n, to);
//  ConstantInt *ci0 = dyn_cast<ConstantInt>(op0);
//  ConstantInt *ci1 = dyn_cast<ConstantInt>(op1);
//  if (ci0 || ci1) {
//    // either op0 or op1 is a constant integer
//    assert(!(ci0 && ci1) && "adding two constants");
//    if (ci0) {
//      int cVal0 = getConstantIntUnsafe(ci0);
//      std::string op1Str = Utils::valueToStringUnsafe(v2n, op1);
//      DEBUG_MSG("[DEBUG] Performing add to: " << op1Str << '\n');
//      DEBUG_MSG("\tBefore Fact: ");
//      DEBUG(f.fprint(stderr););
//      DEBUG_MSG("\n");
//      LatticeFact ret = f.add(toStr, op1Str, cVal0);
//      DEBUG_MSG("\tAfter Fact: ");
//      DEBUG(ret.fprint(stderr););
//      DEBUG_MSG("\n");
//      return ret;
//    }
//    else {
//      int cVal1 = getConstantIntUnsafe(ci1);
//      std::string op0Str = Utils::valueToStringUnsafe(v2n, op0);
//      DEBUG_MSG("[DEBUG] Performing add to: " << op0Str << '\n');
//      DEBUG_MSG("\tBefore Fact: ");
//      DEBUG(f.fprint(stderr););
//      DEBUG_MSG("\n");
//      LatticeFact ret = f.add(toStr, op0Str, cVal1);
//      DEBUG_MSG("\tAfter Fact: ");
//      DEBUG(ret.fprint(stderr););
//      DEBUG_MSG("\n");
//      return ret;
//    }
//  }
//  else {
//    errs() << "[ERROR] Add operand combination unhandled\n";
//    errs() << *to << '\n';
//    exit(EXIT_FAILURE);
//  }
//}

LatticeFact BBOps::performBinOpMod(Instruction::BinaryOps opTy
    , Value *to
    , Value *op0
    , Value *op1
    , LatticeFact f
    , const ::std::map<Value *, std::string> v2n) {
  // Perform the operation (non modulo)
  LatticeFact tmp = performBinOp(opTy, to, op0, op1, f, v2n);
  // Modulo the result 2^bw 
  unsigned bw = getBitWidth(to);
  long unsigned bw2 = std::pow(2.0, bw);
  // The result of the previous binary operation in tmp is stored in `to`.
  // Next, modulo `to` by `bw2` and store the result in `to`.
  std::string toStr = Utils::valueToStringUnsafe(v2n, to);
  return tmp.mod(toStr, toStr, bw2);
}

LatticeFact BBOps::performBinOp(Instruction::BinaryOps opTy
    , Value *to
    , Value *op0
    , Value *op1
    , const LatticeFact f
    , const ::std::map<Value *, std::string> v2n) {
  std::string toStr = Utils::valueToStringUnsafe(v2n, to);
  ConstantInt *ci0 = dyn_cast<ConstantInt>(op0);
  ConstantInt *ci1 = dyn_cast<ConstantInt>(op1);
  if (ci0 || ci1) {
    // either op0 or op1 is a constant integer
    assert(!(ci0 && ci1) && "binop on two constants");
    if (ci0) { // operand 0 is a constant
      int cVal0 = Utils::getConstantIntUnsafe(ci0);
      std::string op1Str = Utils::valueToStringUnsafe(v2n, op1);
      DEBUG_MSG("[DEBUG] Performing binOp to: " << op1Str << '\n');
      DEBUG_MSG("\tBefore Fact: ");
      DEBUG(f.fprint(stderr););
      DEBUG_MSG("\n");
      LatticeFact ret(f);
      if (opTy == BinaryOperator::Add) {
        ret = f.add(toStr, op1Str, cVal0);
      }
      else if (opTy == BinaryOperator::Mul) {
        ret = f.mul(toStr, op1Str, cVal0);
      }
      // TODO: handle other operations
      else {
        errs() << "[ERROR] BinOp, first op const, unhandled operation\n";
        errs() << *to << '\n';
        assert(0 && "error: see above");
        exit(EXIT_FAILURE);
      }
      DEBUG_MSG("\tAfter Fact: ");
      DEBUG(ret.fprint(stderr););
      DEBUG_MSG("\n");
      return ret;
    }
    else { // operand 1 is a constant
      int cVal1 = Utils::getConstantIntUnsafe(ci1);
      std::string op0Str = Utils::valueToStringUnsafe(v2n, op0);
      DEBUG_MSG("[DEBUG] Performing add to: " << op0Str << '\n');
      DEBUG_MSG("\tBefore Fact: ");
      DEBUG(f.fprint(stderr););
      DEBUG_MSG("\n");
      LatticeFact ret(f);
      if (opTy == BinaryOperator::Add) {
        ret = f.add(toStr, op0Str, cVal1);
      }
      else if (opTy == BinaryOperator::Mul) {
        ret = f.mul(toStr, op0Str, cVal1);
      }
      // TODO: handle other operations
      else {
        errs() << "[ERROR] BinOp: second op const, unhandled operation\n";
        errs() << *to << '\n';
        assert(0 && "error: see above");
        exit(EXIT_FAILURE);
      }
      DEBUG_MSG("\tAfter Fact: ");
      DEBUG(ret.fprint(stderr););
      DEBUG_MSG("\n");
      return ret;
    }
  }
  else {
    if (opTy == BinaryOperator::Add) {
      std::string op0Str = Utils::valueToStringUnsafe(v2n, op0);
      std::string op1Str = Utils::valueToStringUnsafe(v2n, op1);
      return f.add(toStr, op0Str, op1Str);
    }
    else {
      errs() << "[ERROR] BinOp both ops non-const unhandled op type\n";
      errs() << *to << '\n';
      assert(0 && "error: see above");
      exit(EXIT_FAILURE);
    }
  }
}


LatticeFact BBOps::performLogicOp(Instruction::BinaryOps opTy
    , Value *to
    , Value *op0
    , Value *op1
    , LatticeFact f
    , const ::std::map<Value *, std::string> v2n) {
  assert(v2n.find(to) != v2n.end() && "to not in map");
  if (!(op0->getType()->isIntegerTy(1))
      || !(op1->getType()->isIntegerTy(1))) {

    // We can handle some special cases of integer XORs
    if (opTy == Instruction::Xor) {
      if (op0 == op1) {
        // a XOR a = 0
        std::string toVar = Utils::valueToStringUnsafe(v2n, to);
        return f.assign(toVar, 0);
      }
    }

    errs() << "[ERROR] Non-logical (bitsize 1) XOR operation\n"
           << "to: " << *to << '\n'
           << "op0: " << *op0 << '\n'
           << "op1: " << *op1 << '\n';
    assert(0 && "error: see above");
    exit(EXIT_FAILURE);
  }

  // True if the value of op0/op1 is zero or one (both can be true)
  bool op0Zero = false;
  bool op0One = false;
  bool op1Zero = false;
  bool op1One = false;

  // True if the value being assigned to (to) should be true or false (0 or 1)
  // Both of these can be true, e.g., 
  // [0,1] ^ [0,1] = [0,1]
  bool toTrue = false;
  bool toFalse = false;

  // It is possible for isEq to return false for both zero and one if the
  // operation cannot be computed. That is, it is unknown whether the value is
  // zero or one. In this case we overapproximate and set the value to both
  // zero and one
  if (ConstantInt *op0ci = dyn_cast<ConstantInt>(op0)) {
    DEBUG_MSG("\top0 constant int\n");
    int cval = Utils::getConstantIntUnsafe(op0ci);
    if (cval == 0) {
      op0Zero = true;
    }
    else if (cval == 1) {
      op0One = true;
    }
    else {
      assert(0 && "op0 contant != 0 or 1");
    }
  }
  else {
    if (!(f.isEq(Utils::mapAtUnsafe(v2n, op0), 0) 
          || f.isEq(Utils::mapAtUnsafe(v2n, op0), 1))) {
      // Unknown if zero or one
      op0Zero = true;
      op0One = true;
    }
    if (f.isEq(Utils::mapAtUnsafe(v2n, op0), 0)) {
      op0Zero = true;
    }
    if (f.isEq(Utils::mapAtUnsafe(v2n, op0), 1)) {
      op0One = true;
    }
  }
  if (ConstantInt *op1ci = dyn_cast<ConstantInt>(op1)) {
    DEBUG_MSG("\top1 constant int\n");
    // getConstantInt, for a one bit value, returns the signed interpretation
    // (i.e., negative one)
    int cval = Utils::getConstantIntUnsafe(op1ci);
    if (cval == 0) {
      op1Zero = true;
    }
    else if (cval == -1) {
      op1One = true;
    }
    else {
      assert(0 && "op1 constant != 0 or 1");
    }
  }
  else {
    DEBUG_MSG("Op1 not a constant int: " << *op1 << '\n');
    if (!(f.isEq(Utils::mapAtUnsafe(v2n, op1), 0) 
          || f.isEq(Utils::mapAtUnsafe(v2n, op1), 1))) {
      // Unknown if zero or one
      op1Zero = true;
      op1One = true;
    }
    if (f.isEq(Utils::mapAtUnsafe(v2n, op1), 0)) {
      op1Zero = true;
    }
    if (f.isEq(Utils::mapAtUnsafe(v2n, op1), 1)) {
      op1One = true;
    }
  }

  // Given the values of op0 and op1, perform the actual logic operation
  if (opTy == Instruction::Xor) {
    DEBUG_MSG("\tPerforming XOR:"
        << "\n\top0One: " << op0One
        << "\n\top0Zero: " << op0Zero
        << "\n\top1One: " << op1One
        << "\n\top1Zero: " << op1Zero << '\n');
    // 1 ^ 0 or 0 ^ 1 == 1
    if ((op0One && op1Zero)
        || (op0Zero && op1One)) {
      toTrue = true;
    }
    // 0 ^ 0 or 1 ^ 1 = 0
    if ((op0Zero && op1Zero)
        || (op0One && op1One)) {
      toFalse = true;
    }
  }
  else {
    errs() << "[ERROR] unhandeled logic operation: " << opTy << '\n'
           << "to: " << *to << '\n'
           << "op0: " << *op0 << '\n'
           << "op1: " << *op1 << '\n';
    assert(0 && "error: see above");
    exit(EXIT_FAILURE);
  }
  DEBUG_MSG("[DEBUG] logicOp: to: " << *to << '\n' << "op0: " << *op0 << '\n'
            << "op1: " << *op1 << '\n');
  assert((toTrue || toFalse) && "result of logic op neither set nor unset");

  // assign the value to `to`
  return f.assignTrueFalse(Utils::mapAtUnsafe(v2n, to), toTrue, toFalse);
}

// Join all the values in the phi-node and store them in the result
LatticeFact performPhiInst(PHINode *phi, const LatticeFact in
    , std::map<Value *, std::string> v2n) {
  LatticeFact ret(in);
  std::string toStr = Utils::mapAtUnsafe(v2n, (Value *)phi);

  DEBUG_MSG("Phi in fact\n");
  DEBUG(in.fprint(stderr););

  // First, set the result of the phi to bottom. We do this because it is the
  // identify under join.
  ret = ret.setBottom(toStr);

  DEBUG_MSG("Phi in fact with bot ret:\n");
  DEBUG(ret.fprint(stderr););

  // Continuously join ret with new values
  for (unsigned i = 0; i < phi->getNumIncomingValues(); ++i) {
    Value *v = phi->getIncomingValue(i);
    if (ConstantInt *cint = dyn_cast<ConstantInt>(v)) {
      ret.factJoinDestr(ret.assign(toStr, Utils::getConstantIntUnsafe(cint)));
    }
    else if (CmpInst *cmp = dyn_cast<CmpInst>(v)) {
      std::string vstr = Utils::mapAtUnsafe(v2n, (Value *)cmp);
      DEBUG_MSG("Phi operand: " << vstr << '\n');
      ret.factJoinDestr(ret.assign(toStr, vstr));
    }
    else if (BinaryOperator *bo = dyn_cast<BinaryOperator>(v)) {
        Value *op0 = bo->getOperand(0);
        Value *op1 = bo->getOperand(1);
        if ((bo->getOpcode() == Instruction::Add)
                || (bo->getOpcode() == Instruction::Mul) || (bo->getOpcode() == Instruction::Sub)) {
            ret.factJoinDestr(BBOps::performBinOpMod(bo->getOpcode(), v, op0, op1, in, v2n));
        }
        else if (bo->getOpcode() == Instruction::Xor) {
            ret.factJoinDestr(BBOps::performLogicOp(bo->getOpcode(), v, op0, op1, in, v2n));
        }
        else {
            //return in;
            errs() << "[ERROR] BinaryOperator unhandled\n";
            errs() << *bo << '\n';
            assert(0 && "error: see above");
            exit(EXIT_FAILURE);
        }
    }
    else {
        errs() << "[ERROR] Unhandled value in phi node: " << *v << '\n';
        exit(EXIT_FAILURE);
    }
  }
  DEBUG_MSG("Phi ret: " << Utils::mapAtUnsafe(v2n, (Value *) phi) << '\n'
            << "Out fact:\n");
  DEBUG(ret.fprint(stderr););
  return ret;
}

//LatticeFact BBOps::performAssignEq(Value *to
//    , Value *op0
//    , Value *op1
//    , LatticeFact f
//    , const std::map<Value*, std::string> v2n) {
//  // op0 is constant
//  // op1 is varaible
//  if (ConstantInt *ci0 = dyn_cast<ConstantInt>(op0)) {
//    int cVal0 = getConstantIntUnsafe(ci0);
//    std::string toStr = Utils::valueToStringUnsafe(v2n, to);
//    if (!isa<Constant>(op1)) {
//      std::string op1Str = Utils::valueToStringUnsafe(v2n, op1);
//      return f.icmpEq(toStr, cVal0, op1Str);
//    }
//    else {
//      llvm_unreachable("comparing two constants");
//    }
//  }
//  // op0 is variable
//  // op1 is constant
//  else if (ConstantInt *ci1 = dyn_cast<ConstantInt>(op1)) {
//    int cVal1 = getConstantIntUnsafe(ci1);
//    std::string toStr = Utils::valueToStringUnsafe(v2n, to);
//    if (!isa<Constant>(op0)) {
//      std::string op0Str = Utils::valueToStringUnsafe(v2n, op0);
//      return f.icmpEq(toStr, op0Str, cVal1);
//    }
//    else {
//      llvm_unreachable("comparing two constants");
//    }
//  }
//  else {
//    errs() << "[ERROR] unhandled icmp constant/non-constant pair\n"
//           << "  op0: " << *op0 << "\n  " << *op1 << '\n';
//    assert(0 && "see above");
//  }
//  llvm_unreachable("unreachable: performAssignEq");
//}

//LatticeFact BBOps::performAssignNEq(Value *to
//    , Value *op0
//    , Value *op1
//    , LatticeFact f
//    , const std::map<Value*, std::string> v2n) {
//  // op0 is constant
//  // op1 is varaible
//  if (ConstantInt *ci0 = dyn_cast<ConstantInt>(op0)) {
//    int cVal0 = getConstantIntUnsafe(ci0);
//    std::string toStr = Utils::valueToStringUnsafe(v2n, to);
//    if (!isa<Constant>(op1)) {
//      std::string op1Str = Utils::valueToStringUnsafe(v2n, op1);
//      return f.icmpNEq(toStr, cVal0, op1Str);
//    }
//    else {
//      llvm_unreachable("comparing two constants");
//    }
//  }
//  // op0 is variable
//  // op1 is constant
//  else if (ConstantInt *ci1 = dyn_cast<ConstantInt>(op1)) {
//    int cVal1 = getConstantIntUnsafe(ci1);
//    std::string toStr = Utils::valueToStringUnsafe(v2n, to);
//    if (!isa<Constant>(op0)) {
//      std::string op0Str = Utils::valueToStringUnsafe(v2n, op0);
//      return f.icmpNEq(toStr, op0Str, cVal1);
//    }
//    else {
//      llvm_unreachable("comparing two constants");
//    }
//  }
//  else {
//    errs() << "[ERROR] unhandled icmp constant/non-constant pair\n"
//           << "  op0: " << *op0 << "\n  " << *op1 << '\n';
//    assert(0 && "see above");
//  }
//  llvm_unreachable("unreachable: performAssignNEq");
//}

LatticeFact BBOps::performAssignCmp(Value *to
    , Value *op0
    , Value *op1
    , LatticeFact f
    , llvm::CmpInst::Predicate pred
    , const std::map<Value *, std::string> v2n) {
  // op0 is constant
  // op1 is varaible
  if (ConstantInt *ci0 = dyn_cast<ConstantInt>(op0)) {
    int cVal0 = Utils::getConstantIntUnsafe(ci0);
    std::string toStr = Utils::valueToStringUnsafe(v2n, to);
    if (!isa<Constant>(op1)) {
      std::string op1Str = Utils::valueToStringUnsafe(v2n, op1);
      if (pred == llvm::CmpInst::ICMP_NE) {
        return f.icmpNEq(toStr, cVal0, op1Str);
      }
      else if (pred == llvm::CmpInst::ICMP_EQ) {
        return f.icmpEq(toStr, cVal0, op1Str);
      }
      else if (pred == llvm::CmpInst::ICMP_SGT) {
        return f.icmpGt(toStr, cVal0, op1Str);
      }
      else if (pred == llvm::CmpInst::ICMP_SGE) {
        // Greater than or equal
        return f.icmpGeq(toStr, cVal0, op1Str);
      }
      else if (pred == llvm::CmpInst::ICMP_SLT) {
        return f.icmpLt(toStr, cVal0, op1Str);
      } else if (pred == llvm::CmpInst::ICMP_SLE) {
				//assert(0 && "icmp sle unhandled");
        return f.icmpLeq(toStr, cVal0, op1Str);
      } else {
        errs() << "[ERROR] Unhandled icmp predicate:\n"
               << "to: " << *to << '\n';
        assert(0 && "see above");
      }
    }
    else {
     errs() << "[ERROR] Unhandled !!!!!!!!!!!!!!!!!!!!:\n" << "to: " << *to << '\n';
      llvm_unreachable("comparing two constants");
    }
  }
  // op0 is variable
  // op1 is constant
  else if (ConstantInt *ci1 = dyn_cast<ConstantInt>(op1)) {
    int cVal1 = Utils::getConstantIntUnsafe(ci1);
    std::string toStr = Utils::valueToStringUnsafe(v2n, to);
    if (!isa<Constant>(op0)) {
      std::string op0Str = Utils::valueToStringUnsafe(v2n, op0);
      if (pred == llvm::CmpInst::ICMP_NE) {
        return f.icmpNEq(toStr, op0Str, cVal1);
      }
      else if (pred == llvm::CmpInst::ICMP_EQ) {
        return f.icmpEq(toStr, op0Str, cVal1);
      }
      else if (pred == llvm::CmpInst::ICMP_SGT) {
        return f.icmpGt(toStr, op0Str, cVal1);
      }
      else if (pred == llvm::CmpInst::ICMP_SGE) {
        // Greater than or equal
        return f.icmpGeq(toStr, op0Str, cVal1);
      }
			else if (pred == llvm::CmpInst::ICMP_SLT) {
        return f.icmpLt(toStr, op0Str, cVal1);
			}
			else if (pred == llvm::CmpInst::ICMP_SLE) {
        return f.icmpLeq(toStr, op0Str, cVal1);
			}
      else {
        errs() << "[ERROR] Unhandled icmp predicate:\n"
               << "to: " << *to << '\n';
        assert(0 && "see above");
      }
    }
    else {
      llvm_unreachable("comparing two constants");
    }
  }
  else {
    errs() << "[ERROR] unhandled icmp constant/non-constant pair\n"
           << "  op0: " << *op0 << "\n  " << *op1 << '\n';
    assert(0 && "see above");
  }
  llvm_unreachable("unreachable: performAssignICmp");
}

LatticeFact BBOps::performCast(Value *dst
    , Value *val
    , Type *dstTy
    , const LatticeFact f
    , const Module *m
    , const std::map<Value *, std::string> v2n) {
  // An upward cast (i.e., cast to higher bitwidth) can either sign extend or
  // zero extend. All LLVM integers are in twos compliment. So, a zero extend
  // can convert a negative value into a positive one. For example, 0b111
  // (negative one at three bits) zero extended to 5 bits becomes 0b00111 which
  // is positive three. 
  //
  // A sign extend, in twos compliment pads all the higher bits with the sign
  // bit. E.g., 0b111 sign extended to five bits is 0b11111 (negative one).
  // 0b011 (three) sign extended to five bits is 0b00011 (three).
  //
  // Signed/Unsigned values are used in LLVM only at the instruction level:
  // there are no signed/unsigned types. For example, there are signed and
  // unsigned division/remainder functions
  //
  // Overall, we take a simple approach and ignore the bit representation of
  // the values (our numerical domains are on integers not bitvectors). All
  // cast operations simply "do nothing" and ignore overflow/underflow or sign
  // extension.
  //
  // TODO: We need a "bit aware" numerical domain to support casts.
  return performAssign(dst, val, f, m, v2n);
}

LatticeFact BBOps::performAssignWithInterf(LoadInst *to
    , Value *ptr
    , LatticeFact f
    , LatticeFact interf
    , const std::map<Value*, std::string> v2n) {
  Module *m = to->getParent()->getParent()->getParent();
  return performAssignWithInterf(to, m, ptr, f, interf, v2n);
}

LatticeFact BBOps::performAssignWithInterf(Value *to
    , Module *m
    , Value *ptr
    , const LatticeFact f
    , LatticeFact interf
    , const std::map<Value*, std::string> v2n) {
  // Copy f, get the values of globals from interf, then perform the assign
  LatticeFact ret(f);
  DEBUG_MSG("[DEBUG] performAssignWitherInterf: \n");
  DEBUG_MSG("\tInput Fact: ");
  DEBUG(f.fprint(stderr););
  DEBUG_MSG('\n');
  // Assign each global in interf to f
  //for (auto gi = m->global_begin(), ge = m->global_end(); gi != ge; ++ gi) {
  //  GlobalVariable *g = &*gi;
  //  ap_interval_t *gInt = interf.getVarInterval(g, v2n);
  //  // the returned global is NULL if it is not found in v2n. This means it is
  //  // a global we are not tracking
  //  if (gInt == NULL) {
  //    continue;
  //  }
  //  DEBUG_MSG("\tGlobal Interval: " );
  //  DEBUG(ap_interval_fprint(stderr, gInt););
  //  DEBUG_MSG('\n');
  //  ret = ret.assignInterval(g, gInt, v2n);
  //}

  // Get the value of ptr from interf and it it into ret
  ap_interval_t *pInt = interf.getVarInterval(ptr, v2n);
  ret = ret.assignInterval(ptr, pInt, v2n);
  ret = performAssign(to, ptr, ret, m, v2n);
  DEBUG_MSG("\tAfter Assign: ");
  DEBUG(ret.fprint(stderr););
  DEBUG_MSG('\n');
  return ret;
}

// Performs the appropriate assignment depending on if `nonCombinInterf` is
// true
LatticeFact BBOps::performAssignWithInterf(Value *to
    , Module *m
    , Value *ptr
    , LatticeFact f
    , LatticeFact interf
    , const bool nonCombinInterfs
    , const std::map<Value*, std::string> v2n) {
    if (!nonCombinInterfs) {
      return performAssignWithInterf(to, m, ptr, f, interf, v2n);
    }
    else {
      return performNonCombinAssign(to, m, ptr, f, interf, v2n);
    }
}


LatticeFact BBOps::performNonCombinAssign(Instruction *to
    , Value *ptr
    , LatticeFact f
    , LatticeFact interf
    , const std::map<Value*, std::string> v2n) {
  Module *m = to->getParent()->getParent()->getParent();
  return performNonCombinAssign(to, m, ptr, f, interf, v2n);
}

LatticeFact BBOps::performNonCombinAssign(Value *to
    , Module *m
    , Value *ptr
    , LatticeFact f
    , LatticeFact interf
    , const std::map<Value*, std::string> v2n) {
  // Merge the values (interval) of ptr in the current memory environment and
  // in the interference and then perform the assignment
  LatticeFact ret(f);

  // Combine the current state and the interference and then extract the value
  // of ptr.
  LatticeFact comb = LatticeFact::factJoin(f, interf);
  ap_interval_t *ptrCombInt = comb.getVarInterval(ptr, v2n);
  assert(ptrCombInt && "load's ptr value not found in interf. fact");
  // Assign the value of ptr in the combined state in the return state
  ret = ret.assignInterval(ptr, ptrCombInt, v2n);
  // Perform the assignment from ptr to the return of the load
  ret = performAssign(to, ptr, ret, m, v2n);
  return ret;
}

LatticeFact BBOps::transfer_func(LatticeFact in
    , llvm::Instruction *i
    , const std::map<Instruction *, LatticeFact> interf
    //, std::vector<std::pair<Instruction*, LatticeFact>> &rErrors
    , std::set<Instruction*> &rErrors
    , std::map<Instruction *, LatticeFact> &reachStores
    , std::map<CallInst *, LatticeFact> &reachThreadCreate
    , const std::map<Value*, std::string> v2n
    , bool nonCombinInterfs) {
  DEBUG_MSG("[DEBUG] exec transfer: " << *i << '\n');
  const Module *m = i->getParent()->getParent()->getParent();
  if (isa<AllocaInst>(i)) {
    // An alloca instruction does not actually do anything. We allow each
    // alloca to create a single variable name (even if its in a loop). This
    // variable name is manipulated through loads and stores.
    LatticeFact ret = in;
    assert(ret.valid() && "returning invalid fact");
    return ret;
  }
  else if (StoreInst *si = dyn_cast<StoreInst>(i)) {
    // A store instruction simply assigns the value to the pointer. We assume
    // is only one memory location associated with each pointer.
    Value *from = si->getValueOperand();
    Value *to = si->getPointerOperand();
    LatticeFact newF = performAssign(to, from, in, m, v2n);
    reachStores = updateReachStores(reachStores, si, newF);
    return newF;
  }
  else if (LoadInst *li = dyn_cast<LoadInst>(i)) {
    auto it = interf.find(li);
    Value *ptr = li->getPointerOperand();
    if (it == interf.end()) {
      // A load instruction takes a pointer operand and assigns its contents to a
      // variable. The actual variable loaded into is the instruction it self.
      return performAssign(li, ptr, in, m, v2n);
    }
    else {
      LatticeFact interf = it->second;
      return performAssignWithInterf(li
          , li->getParent()->getParent()->getParent(), ptr, in, interf
          , nonCombinInterfs, v2n);
    }
  }
  else if (ICmpInst *ic = dyn_cast<ICmpInst>(i)) {
    Value *op0 = ic->getOperand(0);
    Value *op1 = ic->getOperand(1);
    return performAssignCmp(ic, op0, op1, in, ic->getPredicate(), v2n);
  }
  else if (CallInst *ci = dyn_cast<CallInst>(i)) {
    if (isPthreadCreate(ci)) {
      reachThreadCreate = updateReachThreadCreate(ci, in, reachThreadCreate);
      //updateReachThreadCreate(ci, in, reachThreadCreate);
      //// Save the fact, and potentially join, at any thread creation location
      //auto it = reachThreadCreate.find(ci);
      //if (it == reachThreadCreate.end()) {
      //  reachThreadCreate.emplace(ci, in);
      //}
      //else {
      //  LatticeFact crJoin = LatticeFact::factJoin(in, it->second);
      //  reachThreadCreate.erase(it);
      //  reachThreadCreate.emplace(ci, crJoin);
      //}
      // pthread create does not modify the state of the thread so the input
      // fact can be returned unchanged.
      return in;
    }
    // Some functions, like pthread_create can be skipped because they have no
    // effect on the environment
    else if (skipCall(ci)) {
      // Return the input fact unmodified
      return in;
    }
    else if (isError(ci)) {
      assert(!in.isBottom() && "error reached with bottom");
      //errs() << "**** Found Reachable Error ****\n"
             //<< "Fact: ";
      //in.fprint(stderr);
      //errs() << *ci << '\n';

      // Save the error for the caller
      // TODO: this should be uncommented
      //rErrors.push_back(std::make_pair(ci, in));
      rErrors.insert(ci);
      // The assumption is that an error state is unrecoverable and, thus, has
      // no successors (i.e., it should have a UnreachableInst terminator). So,
      // we can simply return the input fact.
      return in;
    }
    else if (isMFence(ci)) {
      //if (!(tsoConstrG || psoConstrG || rmoConstrG)) {
      //  // Test is not being run under weakmemory, MFENCE is
      //  // undefined.
      //  LatticeFact ret = in.getTop();
      //  return ret;
      //}
      // TODO: Add however you wanna handle an MFENCE here
      return in;
    }
    else {
        return in;
      errs() << "[ERROR] Unhandled function call: " << *ci << '\n';
      exit(EXIT_FAILURE);
    }
  }
  else if (BinaryOperator *bo = dyn_cast<BinaryOperator>(i)) {
    Value *op0 = bo->getOperand(0);
    Value *op1 = bo->getOperand(1);
    if ((bo->getOpcode() == Instruction::Add)
        || (bo->getOpcode() == Instruction::Mul) || (bo->getOpcode() == Instruction::Sub)) {
      return performBinOpMod(bo->getOpcode(), i, op0, op1, in, v2n);
    }
    else if (bo->getOpcode() == Instruction::Xor) {
      return performLogicOp(bo->getOpcode(), i, op0, op1, in, v2n);
    }
    else {
        return in;
      errs() << "[ERROR] BinaryOperator unhandled\n";
      errs() << *bo << '\n';
      assert(0 && "error: see above");
      exit(EXIT_FAILURE);
    }
  }
  else if (CastInst *ci = dyn_cast<CastInst>(i)) {
    Value *op0 = ci->getOperand(0);
    Type *dstTy = ci->getDestTy();
    return performCast(ci, op0, dstTy, in, m, v2n);
  }
  else if (PHINode *phi = dyn_cast<PHINode>(i)) {
    return performPhiInst(phi, in, v2n);
  }
  else if (AtomicRMWInst *rmw = dyn_cast<AtomicRMWInst>(i)) {
    // If there is an interference, use it
    auto it = interf.find(rmw);
    LatticeFact newF = in;
    if (it != interf.end()) {
      newF = performAtomicRMW(rmw, in, it->second, nonCombinInterfs, v2n);
    }
    else {
      newF = performAtomicRMW(rmw, in, v2n);
    }
    // a antomicrmw is both a store and a load
    reachStores = updateReachStores(reachStores, rmw, newF);
    return newF;
  }
  else {
    errs() << "[ERROR] unhandled instruction in transfer function: " 
           << *i << '\n';
    exit(EXIT_FAILURE);
  }
}

LatticeFact BBOps::identity_transfer_func(LatticeFact in
    , llvm::Instruction *i
    //, std::vector<std::pair<Instruction*, LatticeFact>> &rErrors
    , std::set<Instruction*> &rErrors
    , std::map<Instruction *, LatticeFact> &reachStores
    , std::map<CallInst *, LatticeFact> &reachThreadCreate) {
  if (CallInst *ci = dyn_cast<CallInst>(i)) {
    if (isPthreadCreate(ci)) {
      reachThreadCreate = updateReachThreadCreate(ci, in, reachThreadCreate);
      //updateReachThreadCreate(ci, in, reachThreadCreate);
      return in;
    }
    else if (isError(ci)) {
      // XXX:
      return in;
      assert(!in.isBottom() && "error reached with bottom");
      errs() << "**** Found Reachable Error ****\n"
             << "Fact: ";
      in.fprint(stderr);
      errs() << *ci << '\n';
      // Save the error for the caller
      //rErrors.push_back(std::make_pair(ci, in));
      rErrors.insert(ci);
      // The assumption is that an error state is unrecoverable and, thus, has
      // no successors (i.e., it should have a UnreachableInst terminator). So,
      // we can simply return the input fact.
      return in;
    }
    else {
      // No other call instructions update the bookkeeping data structures
      return in;
    }
  }
  else if (StoreInst *si = dyn_cast<StoreInst>(i)) {
    reachStores = updateReachStores(reachStores, si, in);
    return in;
  }
  else {
    // No other instruction updates the book keeping data structures
    return in;
  }
}

std::map<CallInst *, LatticeFact> BBOps::updateReachThreadCreate(CallInst *ci
    , const LatticeFact f
    , const std::map<CallInst *, LatticeFact> reachThreadCreate) {
  auto ret = reachThreadCreate;
  // Save the fact, and potentially join, at any thread creation location
  auto it = ret.find(ci);
  if (it == ret.end()) {
    ret.emplace(ci, f);
  }
  else {
    // TODO: widen here
    LatticeFact crJoin = LatticeFact::factJoin(f, it->second);
    ret.erase(it);
    ret.emplace(ci, crJoin);
  }
  return ret;
}

// Assuming `f` is the fact after executing the store instruction `i` update
// the map `rs` to hold the fact `f`. 
//
// If the pair (i, f) already exists in the map this will join the result.
std::map<Instruction*, LatticeFact> BBOps::updateReachStores(
    const std::map<Instruction*, LatticeFact> rs
    , Instruction *i
    , const LatticeFact f) {
  auto newRs = rs;
  auto it = newRs.find(i);
  if (it == newRs.end()) {
    newRs.emplace(i, f);
  }
  else {
    // Join the new fact with the old fact at the store. We join the fact
    // with the one already present in the map. But we do *not* use the
    // combined fact to continue the analysis. This would introduce an
    // unecessary over approximation (combining the memory state at this
    // store with a prior execution).
    // TODO: Widen here
    LatticeFact stJoin = LatticeFact::factJoin(f, it->second);
    newRs.erase(it);
    newRs.emplace(i, stJoin);
  }
  return newRs;
}

bool BBOps::skipCall(const CallInst * const ci) {
  Function *f = ci->getCalledFunction();
  if (f->getName() == "pthread_exit") {
    return true;
  }
  else if (f->getName() == "pthread_join") {
    return true;
  }
  return false;
}

bool BBOps::isError(const CallInst * const ci) {
  Function *f = ci->getCalledFunction();
  if (f->getName() == "__assert_fail") {
    return true;
  }
  return false;
}

bool BBOps::isMFence(const CallInst * const ci) {
  Function *f = ci->getCalledFunction();
  if (f->getName() == "MFENCE"
      || f->getName() == "MSYNC"
      || f->getName() == "MLWSYNC") {
    return true;
  }
  return false;
}

LatticeFact BBOps::getFactInBBAfter(BasicBlock *b
    , const LatticeFact &inFact
    , Instruction *after
    , const std::map<Instruction *, LatticeFact> interf
    //, std::vector<std::pair<Instruction*, LatticeFact>> &rErrors
    , std::set<Instruction*> &rErrors
    , std::map<Instruction *, LatticeFact> &reachStores
    , std::map<CallInst *, LatticeFact> &reachThreadCreate
		, const std::set<Instruction *> assertSlice
		, const std::set<Instruction *> impacted
		, const std::set<Instruction *> mayImpact
    , const std::map<Value*, std::string> v2n
    , bool nonCombinInterfs) {
  // Get the fact before the instruction we are looking for then execute its
  // transfer function
  LatticeFact ret = getFactInBBBefore(b, inFact, after, interf, rErrors
      , reachStores, reachThreadCreate, assertSlice, impacted, mayImpact, v2n
      , nonCombinInterfs);
  //if (assertSliceG) {
	//  if (!assertSlice.count(after)) {
  //  //if (after->getMetadata("AssertSlice")) {
  //    ret = transfer_func(ret, after, interf, rErrors, reachStores
  //        , reachThreadCreate, v2n, nonCombinInterfs);
  //  }
  //  else {
  //    ret = identity_transfer_func(ret, after, rErrors, reachStores
  //        , reachThreadCreate);
  //  }
  //}
  if (assertSliceG && assertSlice.count(after)) {
    // assertSlice is enabled and after is not on the slice
    ret = identity_transfer_func(ret, after, rErrors, reachStores
        , reachThreadCreate);
  }
  else if (impactG && !impacted.count(after) && !mayImpact.count(after)) {
    // imapct is enabled and statement is neither impacted nor may impact
    ret = identity_transfer_func(ret, after, rErrors, reachStores
        , reachThreadCreate);
  }
  else {
    ret = transfer_func(ret, after, interf, rErrors, reachStores
        , reachThreadCreate, v2n, nonCombinInterfs);
  }
  return ret;
}

LatticeFact BBOps::getFactInBBBefore(BasicBlock *b
    , const LatticeFact &inFact
    , Instruction *before
    , const std::map<Instruction *, LatticeFact> interf
    //, std::vector<std::pair<Instruction*, LatticeFact>> &rErrors
    , std::set<Instruction*> &rErrors
    , std::map<Instruction *, LatticeFact> &reachStores
    , std::map<CallInst *, LatticeFact> &reachThreadCreate
		, const std::set<Instruction *> assertSlice
		, const std::set<Instruction *> impacted
		, const std::set<Instruction *> mayImpact
    , const std::map<Value*, std::string> v2n
    , bool nonCombinInterfs) {
  LatticeFact curFact = inFact;
  for (auto it = b->begin(), ie = b->end(); it != ie; ++it) {
    Instruction *curInst = &*it;
    if (curInst == before) {
      return curFact;
    }
    if (assertSliceG) {
      if (!assertSlice.count(curInst)) {
      //if (curInst->getMetadata("AssertSlice")) {
        curFact = transfer_func(curFact, curInst, interf, rErrors, reachStores
            , reachThreadCreate, v2n, nonCombinInterfs);
      }
      else {
        curFact = identity_transfer_func(curFact, curInst, rErrors, reachStores
            , reachThreadCreate);
      }
    }
    else {
      curFact = transfer_func(curFact, curInst, interf, rErrors, reachStores
          , reachThreadCreate, v2n, nonCombinInterfs);
    }
    assert(curFact.valid() && "getFactInBBBefore: invalid fact");
    DEBUG_MSG("  New Fact: ");
    DEBUG(curFact.fprint(stderr););
    DEBUG_MSG("\n");
  }
  errs() << "[ERROR] getFactInBBBefore(): passed instruction not in block\n";
  exit(EXIT_FAILURE);
}

std::vector<std::pair<BasicBlock*, LatticeFact> > 
BBOps::run_transfer_funcs(BasicBlock *b
    , const LatticeFact &inFact
    , const std::map<Instruction *, LatticeFact> interf
    //, std::vector<std::pair<Instruction*, LatticeFact>> &rErrors
    , std::set<Instruction *> &rErrors
    , std::map<Instruction *, LatticeFact> &reachStores
    , std::map<CallInst *, LatticeFact> &reachThreadCreate
		, const std::set<Instruction *> assertSlice
		, const std::set<Instruction *> impacted
		, const std::set<Instruction *> mayImpact
    , const std::map<Value*, std::string> v2n
    , bool nonCombinInterfs) {
  assert(inFact.valid() && "run_transfer_funcs(): invalid input fact");
  LatticeFact endFact = getFactInBBBefore(b, inFact, b->getTerminator()
      , interf, rErrors, reachStores, reachThreadCreate, assertSlice, impacted
      , mayImpact, v2n, nonCombinInterfs);

  // Setup the outgoing facts
  std::vector<std::pair<BasicBlock*, LatticeFact>> ret;
  TerminatorInst *ti = b->getTerminator();
  DEBUG_MSG("  Terminator: " << *ti << '\n');
  if (BranchInst *bi = dyn_cast<BranchInst>(ti)) {
    if (bi->isUnconditional()) {
      // For an unconditional branch, just pass the fact unchanged
      BasicBlock *succ = bi->getSuccessor(0);
      std::pair<BasicBlock*, LatticeFact> p(succ, endFact);
      ret.push_back(p);
      assert(ret.size() == 1 && "uncondbr: ret size != 1");
      return ret;
    }
    else {
      assert(bi->getNumSuccessors() == 2 && "BranchInst without two succs");
      BasicBlock *trueSucc = bi->getSuccessor(0);
      BasicBlock *falseSucc = bi->getSuccessor(1);
      Value *cond = bi->getCondition();
      // If the condition is UNSAT then the abstract value within the
      // LatticeFact will be bottom
      LatticeFact trueF = endFact.constrainTF(cond, true, v2n);
      LatticeFact falseF = endFact.constrainTF(cond, false, v2n);

      DEBUG_MSG("[DEBUG] true branch facts: " );
      DEBUG(trueF.fprint(stderr););
      DEBUG_MSG('\n');

      DEBUG_MSG("[DEBUG] false branch facts: " );
      DEBUG(falseF.fprint(stderr););
      DEBUG_MSG('\n');
      std::pair<BasicBlock*, LatticeFact> tp(trueSucc, trueF);
      std::pair<BasicBlock*, LatticeFact> fp(falseSucc, falseF);
      assert(trueSucc != falseSucc && "condbr with same successors");
      ret.push_back(tp);
      ret.push_back(fp);
      assert(ret.size() == 2 && "condbr: ret size not 2");
      return ret;
    }
  }
  else if (isa<ReturnInst>(ti)) {
    // Return instructions do not pass anything.
    // TODO: Perhaps we should have some way to get the facts at the return
    // instruction
    assert(ret.size() == 0 && "ReturnInst: returing facts");
    return ret; // uninitialized
  }
  else if (isa<UnreachableInst>(ti)) {
    // Unreachable instructions do not do anything
    assert(ret.size() == 0 && "UnreachableInst: returing facts");
    return ret; // uninitialized
  }
  else {
    errs() << "[ERROR] Unhandled terminator: " << *ti << '\n';
    assert(0 && "see above");
  }
  assert(0 && "unreachable");
}

unsigned BBOps::getBitWidth(Value *v) {
  Type *ty = v->getType();
  if (IntegerType *intTy = dyn_cast<IntegerType>(ty)) {
    return intTy->getBitWidth();
  }
  else {
    errs() << "[ERROR] getBitWidth(): unhandled type: " << *ty << '\n';
    assert(0 && "see above");
    exit(EXIT_FAILURE);
  }
}

Constant *BBOps::foldConstExpr(ConstantExpr *ce, const Module *m) {
  // First, attempt to fold the expression
  // Attempt to fold the ConstantExpr into a single value
  Constant *cf = ConstantFoldConstantExpression(ce, m->getDataLayout());
  // If it is a simple constant expression, evaluate it and return the result
  if (ConstantExpr *ceSimpl = dyn_cast<ConstantExpr>(cf)) {
    if (ceSimpl->isCast()) {
      Constant *src = ceSimpl->getOperand(0);
      assert(src != NULL);
      if (src->getType()->isIntegerTy()
          && ceSimpl->getType()->isPointerTy()) {
        // An integer to pointer cast is "transparent": it does not require any
        // computation (as opposed to a real to integer)
        return src;
      }
      else if (src->getType()->isPointerTy()
          && ceSimpl->getType()->isIntegerTy()) {
        // Similarly, a pointer to integer cast is also transparent
        return src;
      }
      // TODO: Handle other cast operations
      else {
        return ceSimpl;
      }
    }
    // TODO: Handle other ConstantExpr types. We may be able to simplify more
    // than just returning the folded result
    else {
      return ceSimpl;
    }
  }
  else if (ConstantInt *ci = dyn_cast<ConstantInt>(cf)) {
    return ci;
  }
  // TODO: Handle other possible stuff. We may be able to simplify the result
  // of the ConstantExpr fold more
  else {
    return ceSimpl;
  }
}

bool BBOps::isPthreadCreate(CallInst *ci) {
  Function *f = ci->getCalledFunction();
  if (f->getName() == "pthread_create") {
    return true;
  }
  return false;
}
