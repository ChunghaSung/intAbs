// Author: Markus Kusano
//
// Some general helpers to add relations/rules to a Z3_fixedpoint
#pragma once

#include "llvm/IR/Instruction.h"
#include <z3.h>
#include <z3++.h>
#include <z3_api.h>
#include <z3_fixedpoint.h>
#include <map>

namespace z3_fp_helpers {


  // Dump the contents of the value cache as metadata attached to each value
  //
  // bvSize should be the max bitvector size used when creating value IDs in
  // bits.
  void valueCacheToMetadata(llvm::Module &M, unsigned bvSize);

  // Convert an unsigned into a constant bitvector string used by Z3 (e.g.,
  // #x0001). `sz` is the number of bits in the bitvector string.
  //
  // There should be enough bytes in `sz` to represent.
  std::string to_const_bitvec(unsigned i, unsigned sz);

  // Given the name of a relation with arguments (bitvector, bitvector) of the
  // passed size return its function declaration.
  z3::func_decl getBVBVFuncDecl(z3::context &ctx, const char *name
      , const unsigned bvSize);

  // Given the name of a relation with arguments (bitvector) of the
  // passed size return its function declaration.
  z3::func_decl getBVFuncDecl(z3::context &ctx, const char *name
      , const unsigned bvSize);

  // Given a predicate with two arguments, create a new fact that fd(v1, v2) =
  // true (i.e., v1 and v2 are in fd).
  void addFact2(z3::context &ctx
              , Z3_fixedpoint &zfp
              , z3::func_decl fd
              , z3::expr v1
              , z3::expr v2);

  // Given a predicate with one arguments, create a new fact that fd(v) =
  // true (i.e., v is in fd).
  void addFact1(z3::context &ctx
              , Z3_fixedpoint &zfp
              , z3::func_decl fd
              , z3::expr v);

  // Same as addFact2Sz but for a func_decl with only argument.
  void addFact1Sz(z3::context &ctx
              , Z3_fixedpoint &zfp
              , z3::func_decl fd
              , llvm::Value *v
              , unsigned bvSize);

  // Add fact that the bitvector IDs of v1 and v2 are in relation fd.
  // i.e., fd(v1, v2) = true
  // Assumes all relations are on bitvectors of the passed size
  void addFact2Sz(z3::context &ctx
              , Z3_fixedpoint &zfp
              , z3::func_decl fd
              , llvm::Value *v1
              , llvm::Value *v2
              , unsigned bvSize);


  // Return a const bitvector ID for the passed instruction.
  //
  // This uses the address of the instruction; LLVM guarantees this is unique.
  //
  // The size of the bitvector sort is bvSize
  z3::expr getValueBVIDSz(z3::context &ctx, llvm::Value *v, unsigned bvSize);

  // Add a relation between two bitvectors of the passed size to the context
  // and solver
  void addBVBVRelation(z3::context &ctx, Z3_fixedpoint &zfp
      , const char * const name, const unsigned bvSize);

  // Add rules related with priorities
  void addPriRule(z3::context &ctx, Z3_fixedpoint &zfp
    , z3::func_decl fd, unsigned bvSize);

  // Add rules related with higher
  void addHighRule(z3::context &ctx, Z3_fixedpoint &zfp
    , z3::func_decl priDecl, z3::func_decl highDecl, unsigned bvSize);

  // Add rules related with compLd and nonLastSt
  void addCombRule(z3::context &ctx
          , Z3_fixedpoint &zfp
          , z3::func_decl ldDecl
          , z3::func_decl stDecl
          , z3::func_decl domDecl
          , z3::func_decl postDomDecl
          , z3::func_decl compLdDecl
          , z3::func_decl nonLastDecl
          , unsigned bvSize);

      // Add rules related with linehigh
  void addLineHighRule(z3::context &ctx, Z3_fixedpoint &zfp
    , z3::func_decl ldDecl, z3::func_decl stDecl, z3::func_decl highDecl
    , z3::func_decl linePriDecl, z3::func_decl lineHighDecl, unsigned bvSize);

  // Add facts for linepri
  void addLinePriFactSz(z3::context &ctx, Z3_fixedpoint &zfp
    , z3::func_decl fd, llvm::Value *v1
    , unsigned priority,  unsigned bvSize);

  // For debuggin purpose
  unsigned getId(llvm::Value *v, unsigned bvSize);

  // Add a rule that the passed func_decl is transitive. Assumes that the
  // passed func_decl is a (bitvector,bitvector) relation of size bvSize
  void addTransRuleBVBV(z3::context &ctx, Z3_fixedpoint &zfp
    , z3::func_decl fd, unsigned bvSize);

  // Add a rule that fd1 => fd2, that is, membership in fd1 implies membership
  // in fd2. 
  // Assumes that the passed func_decl is a (bitvector,bitvector) relation of
  // size bvSize
  void addRelImplicationRule(z3::context &ctx
      , Z3_fixedpoint &zfp, z3::func_decl fd1, z3::func_decl fd2
      , unsigned bvSize);

} // namespace z3_fp_helpers
