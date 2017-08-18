/**
 * Author: Markus Kusano
 *
 * Static functions updating facts over basicblocks
 */
#pragma once
#include "latticefact.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Constants.h"
#include <vector>
#include <map>
#include <set>

namespace BBOps {
  // Given a new fact incoming to this node, run the transfer functions if
  // the newFact contains more information than was previously stored within
  // this BBNode.
  //
  // Returns a map from a BasicBlock to new facts. This indicates the new
  // facts to be passed to the successors of this block.
  //
  // Each BasicBlock in the passed vector should only occur once.
  //
  // The passed map should contain a map from variables in the program to
  // their string representation in Apron.
  //
  // Any instruction in the passed interference map will load its value (and,
  // at the time of execution, all the global variables) inside the
  // corresponding LatticeFact.
  //
  // To disable interferences, just pass an empty map.
  //
  // Reachable error states will be stored in the passed vector (rErrors).
  //
  // reachThreadCreate will contain the fact encountered at any thread creation
  // call. If a specific call instruction is not found in reachThreadCreate
  // then it was not reached.
	//
	// Any instruction in assertSlice will be treated as a noop.
  //
  // If nonCombinInterfs is true, then any interference will be merged with the
  // current thread state. In other words, a load of `x` can load either the
  // value from within the thread or the value from the interference.
  std::vector<std::pair<llvm::BasicBlock*, LatticeFact> > 
    run_transfer_funcs(llvm::BasicBlock *b
        , const LatticeFact &inFact
        , const std::map<llvm::Instruction *, LatticeFact> interf
        //, std::vector<std::pair<llvm::Instruction*, LatticeFact>> &rErrors
        , std::set<llvm::Instruction*> &rErrors
        , std::map<llvm::Instruction *, LatticeFact> &reachStores
        , std::map<llvm::CallInst *, LatticeFact> &reachThreadCreate
				, const std::set<llvm::Instruction *> assertSlice
				, const std::set<llvm::Instruction *> impacted
				, const std::set<llvm::Instruction *> mayImpact
        , const std::map<llvm::Value*, std::string> v2n
        , bool nonCombinInterfs);

  // Return the body (non-terminator) instructions of the passed BasicBlock
  std::vector<llvm::Instruction *> getBody(llvm::BasicBlock *b);

  // Execute the passed instruction using the passed facts. Return the result.
  //
  // If a load instruction is encountered (i.e., `i` is a LoadInst) which is
  // contained in the passed interference map then all the globals inside the
  // corresponding LatticeFact will be loaded instead of the value in the current
  // input lattice fact (`in`).
  //
  // To disable interferences, simply pass an empty map
  //
  // Reachable error statements will be saved in the passed vector (rErrors)
  //
  // The facts reachable to any store instruction will be saved in reachStores.
  // If some store already has a fact then it will be joined ("unioned") with any
  // new facts.
  //
  // See run_transfer_funcs for a description of nonCombinInterfs
  LatticeFact transfer_func(LatticeFact in
            , llvm::Instruction *i
            , const std::map<llvm::Instruction *, LatticeFact> interf
            //, std::vector<std::pair<llvm::Instruction*, LatticeFact>> &rErrors
            , std::set<llvm::Instruction*> &rErrors
            , std::map<llvm::Instruction *, LatticeFact> &reachStores
            , std::map<llvm::CallInst *, LatticeFact> &reachThreadCreate
            , const std::map<llvm::Value*, std::string> v2n
            , bool nonCombinInterfs);

  // Update the passed LatticeFact such that `to <- from`
  //
  // The module is used to resolve constant expressions (requires datalayout)
  LatticeFact performAssign(llvm::Value *to
      , llvm::Value *from
      , const LatticeFact f
      , const llvm::Module *m
      , std::map<llvm::Value*, std::string> v2n);

  // Update the passed LatticeFact such that `to <- op0 + op1`
  //LatticeFact performAdd(llvm::Value *to
  //    , llvm::Value *op0
  //    , llvm::Value *op1
  //    , LatticeFact f
  //    , std::map<llvm::Value*, std::string> v2n);

  // Perform the passed binary operation (non-logical, e.g., add, sub, mul,
  // div; see performLogicOp) 
  LatticeFact performBinOp(llvm::Instruction::BinaryOps opTy
      , llvm::Value *to
      , llvm::Value *op0
      , llvm::Value *op1
      , LatticeFact f
      , const ::std::map<llvm::Value *, std::string> v2n);


  // Same as performBinOp but performs the operation modulo the bitwidth of to
  LatticeFact performBinOpMod(llvm::Instruction::BinaryOps opTy
      , llvm::Value *to
      , llvm::Value *op0
      , llvm::Value *op1
      , LatticeFact f
      , const ::std::map<llvm::Value *, std::string> v2n);



  // Update the passed LatticeFact such that `to <- op0 opTy op1`.
  // Note: This only support logic (i.e., bitsize 1) XOR; it will crash
  // otherwise.
  //
  // Aside: The reason for this is that Apron supports numerical operations.
  // Perhaps this could be overcome by extracting the values of op0 and op1 as
  // intervals. However, this would need to, perhaps, concretize the interval
  // (or, atleast inteligently search for the new max/min values). The reason
  // for this is that the min/max values of a operand cannot simply be used for
  // the bitwise operation
  LatticeFact performLogicOp(llvm::Instruction::BinaryOps opTy
      , llvm::Value *to
      , llvm::Value *op0
      , llvm::Value *op1
      , LatticeFact f
      , const std::map<llvm::Value *, std::string> v2n);


  // Perform the following LLVM instruction:
  //
  // to = icmp pred op0 op1
  LatticeFact performAssignCmp(llvm::Value *to
    , llvm::Value *op0
    , llvm::Value *op1
    , LatticeFact f
    , llvm::CmpInst::Predicate pred
    , const std::map<llvm::Value *, std::string> v2n);

  // Cast `val` to type `dstTy` and store the result in `dst`
  LatticeFact performCast(llvm::Value *dst
      , llvm::Value *val
      , llvm::Type *dstTy
      , const LatticeFact f
      , const llvm::Module *m
      , const std::map<llvm::Value *, std::string> v2n);

  // Return true if the passed call instruction is to a function which we can
  // safely skip.
  bool skipCall(const llvm::CallInst * const ci);

  // Return true if the passed call is to an error function (e.g.,
  // __assert_fail())
  bool isError(const llvm::CallInst * const ci);

  // Return true if the passed call is to a fence function (e.g., MFENCE())
  bool isMFence(const llvm::CallInst * const ci);

  // Return the bitwidth of the passed value.
  // TODO: This currently crashes for floating point values
  unsigned getBitWidth(llvm::Value *v);

  // Return the fact after the executing the passed instruction.
  //
  // This will execute the transfer functions up to and including the passed
  // instruction within the passed basicblock.
  //
  // This will crash if the passed instruction is not present in the passed
  // basicblock.
	//
	// Any instruction in `assertSlice` encountered in the BasicBlock will be
	// treated as a noop.
  //
  // See run_transfer_funcs()/transfer_func() for description of `interf`, 
  // `rErrors` and `nonCombinInterfs`.
  LatticeFact getFactInBBAfter(llvm::BasicBlock *b
      , const LatticeFact &inFact
      , llvm::Instruction *after
      , const std::map<llvm::Instruction *, LatticeFact> interf
      //, std::vector<std::pair<llvm::Instruction*, LatticeFact>> &rErrors
      , std::set<llvm::Instruction*> &rErrors
      , std::map<llvm::Instruction *, LatticeFact> &reachStores
      , std::map<llvm::CallInst *, LatticeFact> &reachThreadCreate
			, const std::set<llvm::Instruction *> assertSlice
			, const std::set<llvm::Instruction *> impacted
			, const std::set<llvm::Instruction *> mayImpact
      , const std::map<llvm::Value*, std::string> v2n
      , bool nonCombinInterfs);

  // Same as getFactInBBBefore but does not execute the transfer function of
  // `after`
  LatticeFact getFactInBBBefore(llvm::BasicBlock *b
      , const LatticeFact &inFact
      , llvm::Instruction *before
      , const std::map<llvm::Instruction *, LatticeFact> interf
      //, std::vector<std::pair<llvm::Instruction*, LatticeFact>> &rErrors
      , std::set<llvm::Instruction*> &rErrors
      , std::map<llvm::Instruction *, LatticeFact> &reachStores
      , std::map<llvm::CallInst *, LatticeFact> &reachThreadCreate
			, const std::set<llvm::Instruction *> assertSlice
			, const std::set<llvm::Instruction *> impacted
			, const std::set<llvm::Instruction *> mayImpact
      , const std::map<llvm::Value*, std::string> v2n
      , bool nonCombinInterfs);

  // Return the value of `ptr` assigned to `to` in the presense of
  // interferences `interf`.
  // This will take the value of each global variable in the program from
  // interf and assign it to f. Potentially, this could overwrite the value of
  // `ptr`. Then, it will set the value of `to` to the value of `ptr`.
  //
  // The first argument is a loadinst only so the Module of the program
  LatticeFact performAssignWithInterf(llvm::LoadInst *to
      , llvm::Value *ptr
      , LatticeFact f
      , LatticeFact interf
      , const std::map<llvm::Value*, std::string> v2n);

  // Same as the definition with a LoadInst as the first argument but
  // explicitly passes the module of the program and does not require the first
  // argument to be a LoadInst (i.e., an instruction)
  LatticeFact performAssignWithInterf(llvm::Value *to
      , llvm::Module *m
      , llvm::Value *ptr
      , LatticeFact f
      , LatticeFact interf
      , const std::map<llvm::Value*, std::string> v2n);

  // Same as the other two version but performs a non-combinational assign
  // depending on the value of nonCombinInterf (see performNonCombinAssign())
  LatticeFact performAssignWithInterf(llvm::Value *to
      , llvm::Module *m
      , llvm::Value *ptr
      , LatticeFact f
      , LatticeFact interf
      , const bool nonCombinInterfs
      , const std::map<llvm::Value*, std::string> v2n);


  // Perform a non-combinational assignment in `to <- ptr` in the presense of
  // interference. This will load the possible values of `ptr` from `f` (the
  // state within the thread) and the interfering state `interf`.
  LatticeFact performNonCombinAssign(llvm::Value *to
      , llvm::Module *m
      , llvm::Value *ptr
      , LatticeFact f
      , LatticeFact interf
      , const std::map<llvm::Value*, std::string> v2n);
  
  // Helper function extracting the module from the passed load instruction
  LatticeFact performNonCombinAssign(llvm::Instruction *to
      , llvm::Value *ptr
      , LatticeFact f
      , LatticeFact interf
      , const std::map<llvm::Value*, std::string> v2n);

  // Perform an atomic RMW write operations. This overloaded version performs
  // the operation in the presense of the passed interference.
  //
  // The flag nonCombinInterfs should be true/false based on if the abstract
  // interpreter is combinationally exploring the set of interferences.
  //
  // Returns the memory state after the instruction is executed.
  LatticeFact performAtomicRMW(llvm::AtomicRMWInst *rmw
      , const LatticeFact in
      , const LatticeFact interf
      , bool nonCombinInterfs
      , std::map<llvm::Value *, std::string> v2n);

  // Perform an atomic rmw without any interferences.
  LatticeFact performAtomicRMW(llvm::AtomicRMWInst *rmw, const LatticeFact in
      , std::map<llvm::Value *, std::string> v2n);

  // Given a ConstantExpr, attempt to return the simplest form
  llvm::Constant *foldConstExpr(llvm::ConstantExpr *ce, const llvm::Module *m);

  // Return true if the passed CallInst is pthread_create()
  bool isPthreadCreate(llvm::CallInst *ci);

  std::map<llvm::Instruction*, LatticeFact> updateReachStores(
      const std::map<llvm::Instruction*, LatticeFact> rs
      , llvm::Instruction *i
      , const LatticeFact f);

  // Update the passed map assuming the passed call instruction is a thread
  // creation with fact at the time of the thread-creation `f`.
  std::map<llvm::CallInst *, LatticeFact> updateReachThreadCreate(llvm::CallInst *ci
      , const LatticeFact f
      , const std::map<llvm::CallInst *, LatticeFact> reachThreadCreate);

  // same as transfer_func() but the returned Fact is the same as the input.
  //
  // This simply updates the book keeping datastructures (reachable stores,
  // reachable errors, reachable thread creation).
  LatticeFact identity_transfer_func(LatticeFact in
      , llvm::Instruction *i
      //, std::vector<std::pair<llvm::Instruction*, LatticeFact>> &rErrors
      , std::set<llvm::Instruction*> &rErrors
      , std::map<llvm::Instruction *, LatticeFact> &reachStores
      , std::map<llvm::CallInst *, LatticeFact> &reachThreadCreate);
};
