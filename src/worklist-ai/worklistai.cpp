/**
 * Author: Chungha Sung 
 * This is modified version of Markus' source code of his FSE 2017 submission.
 *
 * Original Author: Markus Kusano
 *
 * LLVM pass to perform abstract interpretation (using apron)
 */
#include "llvm/Pass.h"
#include "latticefact.h"
#include "varvisit.h"
#include "bbops.h"
#include "utils.h"
#include "cartesianprod.h"

#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/DominanceFrontier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/InstIterator.h"

#include "../utils/mk_debug.h"
#include "../utils/z3_fp_helpers.h"

#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cstdint>

#include "box.h"
#include "oct.h"
#include "pk.h"
#include "pkeq.h"
//#include "ap_abstract1.h"

#include <z3.h>
#include <z3++.h>
#include <z3_api.h>
#include <z3_fixedpoint.h>

using namespace llvm;
using namespace z3_fp_helpers;

ap_manager_t *manG;
int totalPairs = 0;
int filteredPairs = 0;

// Size of Z3 bitvector sort used in fixedpoint solver
//static const unsigned Z3_BV_SIZE = sizeof(uintptr_t) * 8;

// This size needs to be able to represent all the values used in the analysis.
// Increase it as necessary. If it gets too big, things seem to get pretty
// slow.
static const unsigned Z3_BV_SIZE = 16;

// Command line flags. These are used in other files!
// priority information flag
cl::opt<bool> 
  usePriG("priority"
      , cl::desc("Use priority information")
      , cl::init(false));

cl::opt<bool> 
  useBoxG("box"
      , cl::desc("Use box abstract domain")
      , cl::init(false));

cl::opt<bool> 
  useOctG("oct"
      , cl::desc("Use octagon abstract domain")
      , cl::init(false));

cl::opt<bool> 
  usePolyG("pkpoly"
      , cl::desc("Use (strict) convex polyhedral abstract domain")
      , cl::init(false));

cl::opt<bool> 
  useLinEqG("pklineq"
      , cl::desc("Use linear inequality abstract domain")
      , cl::init(false));

cl::opt<bool>
  noCombinsG("nocombs"
      , cl::desc("Combine all interferences into a single state (no "
                 "combinational exploration")
      , cl::init(false));

cl::opt<bool>
  useConstraintsG("constraints"
      , cl::desc("Use constraint solver to prune infreasible interferences")
      , cl::init(false));

cl::opt<bool>
	assertSliceG("aslice"
			, cl::desc("Slice on assertion(s) (requires PDG pass)")
			, cl::init(false));

cl::opt<bool>
  impactG("impact"
      , cl::desc("Use change-impact analysis (equires change-impact pass).")
      , cl::init(false));

// When true, the state at the time of a thread's creation is used instead of
// the initial state of the entire program.
cl::opt<bool>
  dynInitG("dyninit"
      , cl::desc("Calculate the initial state of a thread dynamically")
      , cl::init(false));

cl::opt<bool>
  filterMHB("filter-mhb"
      , cl::desc("Filter interferences based on global program must-happen before")
      , cl::init(false));

cl::opt<bool>
  tsoConstrG("tso"
      , cl::desc("Use TSO interference constraints")
      , cl::init(false));

cl::opt<bool>
  psoConstrG("pso"
      , cl::desc("Use PSO interference constraints")
      , cl::init(false));

cl::opt<bool>
  rmoConstrG("rmo"
      , cl::desc("Use RMO interference constraints")
      , cl::init(false));

// The default behavior is to attempt to constrain branches on ICMP
// instructions. For example,
//
// br %1 label %2 label %3
//
// If %1 is an icmp instruction, e.g., icmp ne %5 %6 then the branch being
// true (resp. false) not only means %1 should be 1 (resp 0) but also that %5
// != %6 (resp. %5 == %6).
//
// So, when generating a fact to pass to either the true of false branch, using
// the icmp instruction allows for more acurrate facts being passed. Obviously,
// this comes with some extra overhead.
cl::opt<bool> constrICmp("-no-icmp-constr"
      , cl::desc("Do not constrain branches using ICMP predicates")
      , cl::init(true));

cl::opt<std::string>
  z3BinLocG("z3"
      , cl::desc("Location of Z3 binary")
      , cl::init(""));
    

// Disable debugging output in debug build
#ifdef MK_DEBUG
cl::opt<bool> 
    nodebugG("nodebug"
        , cl::desc("Disable debug output for debug build")
	      , cl::init(false));
#endif

// The maximum number of combinational interference permutations
static unsigned maxCombPermsG = 0;

struct WorklistAI : public ModulePass {
  static char ID;

  // The type of the worklist
  typedef std::map<BasicBlock*, LatticeFact> worklist_t;

  static constexpr const char * const constraintFile = "poconstr.smt2";

  virtual void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.setPreservesAll();
    if (useConstraintsG || filterMHB) {
      AU.addRequired<DominatorTreeWrapperPass>();
      AU.addRequired<PostDominatorTree>();
    }
  }

  // Print the passed map to stderr
  void printFactMapStderr(std::map<BasicBlock*, LatticeFact> m) const {
    for (auto i = m.begin(), e = m.end(); i != e; ++i) {
      BasicBlock *b = i->first;
      LatticeFact f = i->second;
      errs() << "BasicBlock: " << *b ;
      errs() << "Facts: ";
      f.fprint(stderr);
      errs() << '\n';
    }
  }

  void printValToNameStderr(const std::map<Value*, std::string> m) const {
    for (auto i = m.begin(), e = m.end(); i != e; ++i) {
      Value *v = i->first;
      std::string s = i->second;
      errs() << s << ": " << *v << '\n';
    }
  }

  WorklistAI() : ModulePass(ID) { }

  // Given a lattice fact, return a map with each BasicBlock in the module
  // mapped to the passed fact. 
  std::map<BasicBlock*, LatticeFact> initBBs(const LatticeFact &init
          , Module &M) const {
      std::map<BasicBlock *, LatticeFact> ret;
      for (auto mi = M.begin(), me = M.end(); mi != me; ++mi) {
          for (auto fi = mi->begin(), fe = mi->end(); fi != fe; ++fi) {
              BasicBlock *bb = &*fi;
              ret.emplace(bb, init);
          }
      }
      return ret;
  }

  // Given a lattice fact, return a map with each BasicBlock in the function
  // mapped to the passed fact
  std::map<BasicBlock*, LatticeFact> initBBs(const LatticeFact &init
      , Function *F) const {
    std::map<BasicBlock *, LatticeFact> ret;
    for (auto fi = F->begin(), fe = F->end(); fi != fe; ++fi) {
      BasicBlock *bb = &*fi;
      ret.emplace(bb, init);
    }
    return ret;
  }

  // Merge the passed vector of new facts into the passed worklist
  void merge_worklist(
          std::map<BasicBlock *, LatticeFact> *wl
          , std::vector<std::pair<BasicBlock*, LatticeFact>> ps) const {
      //errs() << "Size of ps: " << ps.size() << "\n";
      for (auto p = ps.begin(), pe = ps.end(); p != pe; ++p) {
          // Check if any of the basicblock are already in worklist.
          // Merge if they are.
          BasicBlock *newB = p->first;
          DEBUG_MSG("Merging worklist: BB: " << *newB << '\n');
          auto f = wl->find(newB);
          if (f == wl->end()) {
              DEBUG_MSG("Fact not in list: ");
              DEBUG(p->second.fprint(stderr););
              DEBUG_MSG("\n");
              wl->emplace(newB, p->second);
          }
          else {
              // TODO: Widen here
              LatticeFact mergeFact = LatticeFact::factJoin(f->second, p->second);
              //LatticeFact mergeFact = LatticeFact::factJoinWiden(f->second, p->second);
              DEBUG_MSG("Fact in list, merged: ");
              DEBUG(mergeFact.fprint(stderr););
              DEBUG_MSG("\n");
              wl->erase(f);
              wl->emplace(newB, mergeFact);
          }
      }
      return;
  }

  // Given a value, return its string name.
  //
  // This will always return a string. If the value has never been encountered
  // before, it will be stored in the valToName map with a fresh name.
  //
  // This also updates nameToVal so a reverse lookup can be done (see
  // getValue())
  std::string getName(std::map<Value *, std::string> &valToName
      , std::map<std::string, Value *> &nameToVal, Value *v) const {
    auto f = valToName.find(v);
    if (f == valToName.end()) {
      size_t newSz = valToName.size();
      std::string newName = "v" + std::to_string(newSz);
      auto res = valToName.insert(std::make_pair(v, newName));
      if (!res.second) {
        assert(0 && "item already exists in valToName map");
      }

      nameToVal.emplace(newName, v);

      return (res.first)->second;
    }
    return f->second;
  }

  // Given a string name of a value return the actual value.
  //
  // Note: this will crash if the value has never been saved in the map before.
  Value *getValue(const std::map<std::string, Value*> nameToVal
      , std::string s) const {
    auto f = nameToVal.find(s);
    if (f == nameToVal.end()) {
      assert(0 && "item not found in nameToVal");
    }
    return f->second;
  }

  // Return the name of all the variables used in the program
  VarVisitor::Vars getAllVarNames(Module &M) const {
    VarVisitor vv;
    vv.visit(M);

    // Also add in the float/int globals variables
    for (auto gi = M.global_begin(), ge = M.global_end(); gi != ge; ++gi) {
      GlobalVariable &g = *gi;
      if (Utils::isIntegerGlobal(&g)) {
        vv.vars.ints.push_back(&g);
      }
      else if (Utils::isFloatGlobal(&g)) {
        vv.vars.floats.push_back(&g);
      }
      else {
        errs() << "[WARNING] Global which is neither Integer nor Float\n";
        errs() << g << '\n';
        errs() << "It is not being monitored (there may be errors en route)\n";
      }
    }
    
    // TODO: Need to handle arrays here
    
    return vv.vars;
  }

  // Return the function named "main" or NULL if the module does not have a
  // "main" function
  Function *getMainFuncOrNULL(Module &M) const {
    for (auto mi = M.begin(), me = M.end(); mi != me; ++mi) {
      Function &f = *mi;
      if (f.getName() == "main") {
        return &f;
      }
    }
    return NULL;
  }

  // Return any function used in a pthread_create() call. 
  // This will crash if any indirect functions are found
  std::set<Function *> getThreads(Module &M) const {
    std::set<Function *> ret;
    for (auto mi = M.begin(), me = M.end(); mi != me; ++mi) {
      for (auto fi = mi->begin(), fe = mi->end(); fi != fe; ++fi) {
        for (auto bi = fi->begin(), be = fi->end(); bi != be; ++bi) {
          Instruction *I = &*bi;
          if (CallInst* ci = dyn_cast<CallInst>(I)) {
            Function *called = ci->getCalledFunction();
            if (called->getName() == "pthread_create") {
              // Found pthread_create. The 2nd argument (zero indexed) is the
              // function being called
              Value *v = ci->getArgOperand(2);
              if (Function *tFunc = dyn_cast<Function>(v)) {
                ret.insert(tFunc);
              }
              else {
                errs() << "[ERROR] pthread_create with non function 2nd arg\n";
                errs() << *v << '\n';
                exit(EXIT_FAILURE);
              }
            }
          } 
        } // for (bi ... )/
      } // for (fi ... )
    } // for (mi ... )
    return ret;
  }

  std::vector<CallInst *> getThreadCreate(Module &M) const {
    std::vector<CallInst *> ret;
    for (auto mi = M.begin(), me = M.end(); mi != me; ++mi) {
      Function *cur = &*mi;
      Utils::addVector(ret, getThreadCreate(cur));
    }
    return ret;
  }

  std::vector<CallInst *> getThreadCreate(Function *F) const {
    std::vector<CallInst *> ret;
    for (auto bi = F->begin(), be = F->end(); bi != be; ++bi) {
      for (auto ii = bi->begin(), ie = bi->end(); ii != ie; ++ii) {
        Instruction *I = &*ii;
        if (CallInst* ci = dyn_cast<CallInst>(I)) {
          Function *called = ci->getCalledFunction();
          if (called->getName() == "pthread_create") {
            ret.push_back(ci);
          }
        } 
      } // for (ii ...)
    } // for (bi ... )/
    return ret;
  }

  // Collect the Integer/Floating point function parameters
  VarVisitor::Vars getFuncParamNames(Module &M) const {
    VarVisitor::Vars vars;
    for (auto mi = M.begin(), me = M.end(); mi != me; ++mi) {
      Function &f = *mi;
      Function::ArgumentListType &al = f.getArgumentList();
      for (auto ai = al.begin(), ae = al.end(); ai != ae; ++ai) {
        Argument &a = *ai;
        Type *at = a.getType();
        if (at->isPointerTy() || at->isIntegerTy()) {
          vars.ints.push_back(&a);
        }
        else if (at->isFloatingPointTy()) {
          vars.floats.push_back(&a);
        }
        else {
          errs() << "[ERROR] unhandled argument type: " << a << '\n';
          assert(0 && "see above");
        }
      }
    }
    return vars;
  }

  // Given the list of integer and floating point values return an environment
  // containing them all.
  //
  // Returns NULL if the environment could not be created
  //
  // Note: The user must manage the memory of the returned environment.
  ap_environment_t *createEnvironment(std::map<Value *, std::string> &valToName
      , std::map<std::string, Value *> &nameToVal, Module &M) const {
    // TODO: Global variables should also be assigned to variable names?
    VarVisitor::Vars vs = getAllVarNames(M);
    vs = VarVisitor::mergeVars(vs, getFuncParamNames(M));
    DEBUG_MSG("[DEBUG] Num Int vars: " << vs.ints.size() << '\n');
    DEBUG_MSG("[DEBUG] Num Float vars: " << vs.floats.size() << '\n');

    // Use sets to ensure no duplicates are added to the environment
    std::set<const char *> intStrs;
    std::set<const char *> floatStrs;

    // This will create names for the all the int and float variables in the
    // program and store them in the internal maps of this class (getName())

    // Put all the ints in a set
    for (size_t i = 0; i < vs.ints.size(); ++i) {
      Value *v = vs.ints[i];
      DEBUG_MSG("[DEBUG] getting int value: " << *v << '\n');
      std::string vs = getName(valToName, nameToVal, v);
      DEBUG_MSG("\tname: " << vs << '\n');
      intStrs.insert(vs.c_str());
    }
    ap_var_t intVs[intStrs.size()];
    size_t pos = 0;
    for (auto it = intStrs.begin(), e = intStrs.end(); it != e; ++it) {
      intVs[pos++] = (ap_var_t)(*it);
    }

    // Put all the floats in a set
    for (size_t i = 0; i < vs.floats.size(); ++i) {
      Value *v = vs.floats[i];
      DEBUG_MSG("[DEBUG] getting float value: " << *v << '\n');
      std::string vs = getName(valToName, nameToVal, v);
      DEBUG_MSG("\tname: " << vs << '\n');
      floatStrs.insert(vs.c_str());
    }
    ap_var_t floatVs[floatStrs.size()];
    pos = 0;
    for (auto it = floatStrs.begin(), e = floatStrs.end(); it != e; ++it) {
      floatVs[pos++] = (ap_var_t)(*it);
    }

    ap_environment_t *env = ap_environment_alloc(intVs
                               , intStrs.size()
                               , floatVs
                               , floatStrs.size());
    assert(env && "error creating environment");
    return env;
  }

  // Return a vector of functions such that the parent of any thread always
  // ocurrs at a lower index than the child.
  std::vector<Function *> sortByCreation(Function *main
      , const std::map<Function *, std::vector<Function*>> parent2child) {
    std::vector<Function *> ret;
    std::deque<Function *> toProcess;
    // Main is the parent of everyone
    toProcess.push_back(main);
    while (toProcess.size()) {
      Function *curParent = toProcess.front();
      toProcess.pop_front();
      // Add the parent
      ret.push_back(curParent);
      // Add all its children to be processed
      auto iter = parent2child.find(curParent);
      if (iter != parent2child.end()) {
        std::vector<Function *> children = iter->second;
        // The children of the children also need to be added to `ret`
        for (Function *c : children) {
          toProcess.push_back(c);
        }
      }
    }
    assert(ret.size() && "returning size zero vector");
    return ret;
  }

  // Given the passed lattice fact with all variables at whatever initial value
  // you want (e.g., undefined (top)) this will update the passed fact
  // (initFact) such that all the globals are initialized to their starting
  // value (from the source code).
  // This assumes that all LLVM Values are mapped to name for apron to use
  // (v2v).
  LatticeFact getInitState(Module *M
      , const LatticeFact initFact
      , const std::map<Value *, std::string> v2v) const {
    LatticeFact ret = initFact;
    for (auto gi = M->global_begin(), ge = M->global_end(); gi != ge; ++gi) {
      // Get the initializer for the global. Lookup the name, and set the value
      // of the name to the initializer
      GlobalVariable *g = &*gi;
      if (!g->hasInitializer()) {
        errs() << "[ERROR] Global without initializer (possibly external)\n"
               << *g << '\n';
        assert(0 && "see above");
      }
      const Constant *c = g->getInitializer();
      if (const ConstantInt *ci = dyn_cast<ConstantInt>(c)) {
        std::string gName = Utils::valueToStringUnsafe(v2v, g);
        int val = Utils::getConstantIntUnsafe(ci);
        DEBUG_MSG("[DEBUG] Initializing Global: ");
        DEBUG_MSG("\tGlobal Name: " << gName << '\n');
        DEBUG_MSG("\tInit Val: " << val << '\n');
        ret = ret.assign(gName, val);
        DEBUG(ret.fprint(stderr););
      }
      // TODO: also need to handle floats (ConstantFloat::getValueAPF())
      //if (isFloatGlobal(g)) {
      // ...
      //}
      else {
        errs() << "[WARNING] Initializing unhandled global type: " << *g 
               << "\nSetting to default initial value\n";
      }
    } // for
    return ret;
  }

  // Return all the stores in the passed function
  std::vector<StoreInst *> getStores(Function *f) const {
    std::vector<StoreInst *> ret;
    for (auto fi = f->begin(), fe = f->end(); fi != fe; ++fi) {
      BasicBlock &b = *fi;
      for (auto bi = b.begin(), be = b.end(); bi != be; ++bi) {
        Instruction *i = &*bi;
        if (StoreInst *si = dyn_cast<StoreInst>(i)) {
          ret.push_back(si);
        }
      }
    }
    return ret;
  }

  // Return all the stores in the passed BasicBlock
  std::vector<StoreInst *> getStores(BasicBlock *b) {
    std::vector<StoreInst *> ret;
    for (auto bi = b->begin(), be = b->end(); bi != be; ++bi) {
      Instruction *i = &*bi;
      if (StoreInst *si = dyn_cast<StoreInst>(i)) {
        ret.push_back(si);
      }
    }
    return ret;
  }

  // Return all the loads in the passed function
  std::vector<Instruction *> getLoads(Function *f) const {
    std::vector<Instruction *> ret;
    for (auto fi = f->begin(), fe = f->end(); fi != fe; ++fi) {
      BasicBlock &b = *fi;
      for (auto bi = b.begin(), be = b.end(); bi != be; ++bi) {
        Instruction *i = &*bi;
        //if (LoadInst *si = dyn_cast<LoadInst>(i)) {
        if (Utils::isSomeLoad(i)) {
          ret.push_back(i);
        }
      }
    }
    return ret;
  }

  bool loadStoreSameVal(Instruction *si, Instruction *li) const {
    if (StoreInst *s = dyn_cast<StoreInst>(si)) {
      if (LoadInst *l = dyn_cast<LoadInst>(li)) {
        return loadStoreSameValSL(s, l);
      }
      else if (AtomicRMWInst *rmw = dyn_cast<AtomicRMWInst>(li)) {
        return loadStoreSameValSRMW(s, rmw);
      }
      else {
        assert(0 && "loadStoreSameVal: unhandled load type sub StoreInst");
      }
    }
    else if (AtomicRMWInst *rmw = dyn_cast<AtomicRMWInst>(si)) {
      if (LoadInst *l = dyn_cast<LoadInst>(li)) {
        return loadStoreSameValRMWL(rmw, l);
      }
      else if (AtomicRMWInst *rmwl = dyn_cast<AtomicRMWInst>(li)) {
        return loadStoreSameValRMWRMW(rmw, rmwl);
      }
      else {
        assert(0 && "loadStoreSameVal: unhandled load type sub atomicrmw");
      }
    }
    else {
      assert(0 && "loadStoreSameVal: unhandled store type");
    }
  }

  bool loadStoreSameValRMWL(AtomicRMWInst *rmw, LoadInst *l) const {
    Value *stPtr = rmw->getPointerOperand();
    Value *lPtr = l->getPointerOperand();
    return stPtr == lPtr;
  }

  bool loadStoreSameValRMWRMW(AtomicRMWInst *rmws, AtomicRMWInst *rmwl) const {
    Value *stPtr = rmws->getPointerOperand();
    Value *lPtr = rmwl->getPointerOperand();
    return stPtr == lPtr;
  }

  bool loadStoreSameValSRMW(StoreInst *s, AtomicRMWInst *rmw) const {
    Value *stPtr = s->getPointerOperand();
    Value *lPtr = rmw->getPointerOperand();
    return stPtr == lPtr;
  }

  // Return true if the passed store stores into the memory location loaded by
  // the load
  bool loadStoreSameValSL(StoreInst *s, LoadInst *l) const {
    Value *stPtr = s->getPointerOperand();
    Value *lPtr = l->getPointerOperand();
    return stPtr == lPtr;
  }

  // Given the set of possible interferences for each load in the passed
  // thread, reanalyze the thread in the presense of the interferences.
  //
  // The fact at the start of each basicblock after the analysis
  // stabalizes is returned.
  //
  // This will do a combinational exploration of the possible interferences for
  // each load, e,g, if l1 has two interferences i1 and i2, and l2 has two
  // interferences i3 and i4, we will explore all the combinations of reading
  // ((i1,i2), (i3,i4)) as well as reading from the thread's own memory state
  // (i.e., at most 9 combinations).
  //
  // The passed vector, rErrors, will contain any reachable error statements as
  // well as the reachable fact at the statement.
  std::map<BasicBlock *, LatticeFact> analyzeThreadInterf(
          ap_manager_t *man
          , ap_environment_t *env
          , Function *f
          , CallInst *createSite
          , const std::map<Instruction *, std::vector<Instruction *>> interfs
          , const std::map<Instruction *, LatticeFact> stFacts
          //, std::vector<std::pair<Instruction *, LatticeFact>> &rErrors
          , std::set<Instruction *> &rErrors
          , std::map<Instruction *, LatticeFact> &reachableStores
          , std::map<CallInst *, LatticeFact> &reachThreadCreate
          , LatticeFact entryFact
          , const std::set<Instruction *> assertSlice
          , const std::set<Instruction *> impacted
          , const std::set<Instruction *> mayImpact
          , const std::map<Value *, std::string> v2n
          , z3::context &ctx
          , Z3_fixedpoint &zfp
          ) const {
      DEBUG_MSG("[DEBUG] analyzeThreadIntefer(): number of interferences: " 
              << interfs.size() << '\n');
      //std::cout << "Interf size: " << interfs.size() << "\n";
      if (!interfs.size()) {
          // When there are no interferences, we do not need to do any
          // combinational exploration
          return analyzeFuncNoInterf(man, env, rErrors, reachableStores
                  , reachThreadCreate, entryFact, assertSlice, impacted, mayImpact, v2n, f);
      }

      if (noCombinsG) {
          // If we are not doing a combinatorial exploration, join all the
          // interferences for each load into a single fact
          std::map<Instruction *, LatticeFact> allInterfs 
              = joinInterfs(interfs, stFacts);
          // Then, perform the analysis as usual but inform the transfer functions
          // to consider both the interference and the state of the thread
          // simultaneously
          return analyzeFunc(man, env, allInterfs, rErrors, reachableStores
                  , reachThreadCreate, assertSlice, impacted, mayImpact, v2n, entryFact, f, true);
      }
      // Calculate the cartesian product of all possible facts from which a load
      // can read from including its own memory state.
      // To do this, consider that 0 indicates a thread should read from its own
      // memory state at the time of some load while a value higher than 0 is an
      // index into the vector of StoreInsts in intefers (note: it will be off by
      // one (indexed starting from one))
      std::vector<int> choicesPerLoad;
      // Each location in choicesPerLoad corresponds to a load instruction. The
      // ordering of load instructions is based on the iterator order of interfs.
      for (auto i = interfs.begin(), ie = interfs.end(); i != ie; ++i) {
          // Incluce one extra choice, the thread reading the value from its own
          // memory environment
          int v = i->second.size() + 1;
          assert(v > 0 && "making zero choices");
          choicesPerLoad.push_back(v);
      }

      // The results of each permutation will be joined into this map
      std::map<BasicBlock *, LatticeFact> res;
      CartesianProduct cp(choicesPerLoad);
      // Test each possible combination 
      unsigned count = 1;
      for (; !cp.AtEnd(); cp.Increment()) {
          std::vector<int> curPerm = cp.permutation();
          bool ret = testCurPerm(man
                  , env
                  , f
                  , createSite
                  , curPerm
                  , interfs
                  , stFacts
                  , rErrors
                  , reachableStores
                  , reachThreadCreate
                  , entryFact
                  , assertSlice
                  , impacted
                  , mayImpact
                  , v2n
                  , ctx
                  , zfp
                  , res);
          if (ret) {
              // Only increment the number of permutations if the permutation is
              // tested.
              maxCombPermsG = std::max(maxCombPermsG, count);
              count++;
          }
      } // for (; !cp.AtEnd(); cp.Increment())
      return res;
  }

  // Test the passed permutation.
  //
  // Returns true if the abstract interperter was run (i.e., the solver could
  // not prove the permutation was redundant)
  //
  // any facts in res will be merged with the facts produced by the analysis
  bool testCurPerm(ap_manager_t *man
                   , ap_environment_t *env
                   , Function *f
                   , CallInst *createSite
                   , const std::vector<int> perm
                   , const std::map<Instruction *, std::vector<Instruction *>> interfs
                   , const std::map<Instruction *, LatticeFact> stFacts 
                   //, std::vector<std::pair<Instruction *, LatticeFact>> &rErrors
                   , std::set<Instruction *> &rErrors
                   , std::map<Instruction *, LatticeFact> &reachableStores
                   , std::map<CallInst *, LatticeFact> &reachThreadCreate
                   , LatticeFact entryFact
			             , const std::set<Instruction *> assertSlice
			             , const std::set<Instruction *> impacted
			             , const std::set<Instruction *> mayImpact
                   , const std::map<Value *, std::string> v2n
                   , z3::context &ctx
                   , Z3_fixedpoint &zfp
                   , std::map<BasicBlock *, LatticeFact> &res ) const {
    assert(perm.size() == interfs.size() && "choice number mismatch");

    // Note: since the store and load instructions are all orded (they are in a
    // map) the results for the same map of store instructions will lead to the
    // same order in the permutation vector. Additionally, the interferences
    // can only grow and not decrease (everything is monotonic). So, if we see
    // the same _exact_ permutation again we can use a cached result.
    //
    // This works because the constraints are based on the entire program
    // (i.e., the constraints will always be the same for each call).
    //
    // This is a map from the permutation vector to the cached result
    static std::map<std::vector<int>, bool> permCache;

    DEBUG_MSG("Permutation: ");
    DEBUG(for (int i : perm) DEBUG_MSG(i << " "););
    DEBUG_MSG('\n');
    std::map<Instruction *, LatticeFact> curInterfMap;
    // Same as curInterfMap but maps a Load to the store it is reading from
    std::map<Instruction *, Instruction *> curInterfStoreMap;
    // Map from an instruction reading from the thread-local to the
    // corresponding store its reading from.
    std::map<Instruction *, Instruction *> curSelfStoreMap;
    int pos = 0;

    // Given the indices in the permutation, gather up the corresponding facts
    // at the interfering stores.
    for (auto i = interfs.begin(), ie = interfs.end(); i != ie; ++i, ++pos) {
      int curChoice = perm[pos];
      Instruction *l = i->first;
      DEBUG_MSG("[DEBUG] Adding load--fact interference\n");
      DEBUG_MSG("\tload: " << *l << '\n');
      if (curChoice == 0) {
        // Not including the load in the map indicates that it should read
        // from its own memory environment
        DEBUG_MSG("\tstore: self\n");
        std::vector<Instruction *> ss = getPreviousStoresWithCreate(l, createSite);
        if (ss.size() == 1) {
          DEBUG_MSG("\tfound predecessor store:" << *(ss[0]) << '\n');
          curSelfStoreMap.emplace(l, ss[0]);
        }
        else {
          // TODO: what constraints can we deduce when there is more than one
          // preceeding store?
        }
        continue;
      }
      // Indecies in the permutation are one indexed (since zero is used in
      // the previous `if`)
      size_t idx = curChoice - 1;
      std::vector<Instruction*> sts = i->second;
      assert(idx < sts.size() && "out-of-bounds choice");
      Instruction *st = sts[idx];
      assert(curInterfMap.find(l) == curInterfMap.end() && "duplicate load");
      assert(stFacts.find(st) != stFacts.end() 
          && "fact not found for store");
      DEBUG_MSG("\tstore: " << *st << '\n');
      curInterfMap.emplace(l, Utils::mapAtUnsafe(stFacts, st));
      if (useConstraintsG) {
        curInterfStoreMap.emplace(l, st);
      }
    } // for (auto i = interfs.begin(), ie = interfs.end(); ...)

    // Check if this interference is feasible
    if (useConstraintsG) {
      auto it = permCache.find(perm);
      DEBUG_MSG("Checking perm cache\n");
      if (it != permCache.end()) {
        DEBUG_MSG("[DEBUG] perm cache hit!\n");
        if (it->second) {
          DEBUG_MSG("\tviolates program order\n");
          // return without testing, can safely skip.
          return false;
        }
        // falling through this if brach (if (it != permCache.end())) leads
        // to the permutation being tested but violatesProgOrder() will not
        // be called thus saving the solver call time
      }
      else {
        // Since we have never seen this permutation before, test if it
        // violates the program order
        if (violatesProgOrder(curInterfStoreMap, curSelfStoreMap, ctx, zfp)) {
          DEBUG_MSG("\tpermutation violates program order\n");
          DEBUG_MSG("\t*Permutation: ");
          DEBUG(for (int i : perm) DEBUG_MSG(i << " "););
          DEBUG_MSG('\n');
          permCache[perm] = true;
          // Test can be safely skipped
          return false;
        }
        else {
          DEBUG_MSG("\tpermutation does not violate prog. order\n");
          DEBUG_MSG("\t*Permutation: ");
          DEBUG(for (int i : perm) DEBUG_MSG(i << " "););
          DEBUG_MSG('\n');
          permCache[perm] = false;
        }
      }
    }
    DEBUG_MSG("running abstract interpretation\n");
    // Test the thread in the presense of this round of interferences
    std::map<BasicBlock *, LatticeFact> newRes 
      = analyzeFunc(man, env, curInterfMap, rErrors, reachableStores
          , reachThreadCreate, assertSlice, impacted, mayImpact, v2n, entryFact, f, false);
    DEBUG_MSG("merging AI results\n");
    // Merge the new results for this combination with the old
    res = mergeFactMaps(res, newRes);
    return true;
  }

  // Return true if the load instructions reading from the passed store
  // instructions violates the program order.
  //
  // This uses the baseConstrs and issues a solver call to muZ.
  //
  // rfs are extra (i1, i2) pairs where reads-from(i1, i2) will be added but
  // not checked if its feasible
  bool violatesProgOrder(const std::map<Instruction *, Instruction *> interfs
                         , const std::map<Instruction *, Instruction *> rfs
      , z3::context &ctx, Z3_fixedpoint &zfp) const {
    DEBUG_MSG("Checking prog order\n");
    if (interfs.size() == 0) {
      // Interferences can be of size zero when the thread is reading from its
      // own memory environment. This will never violate the program order
      DEBUG_MSG("\tinterf.size() is zero, skipping prog. order check\n");
      return false;
    }

    // If a loads and stores is in this map then we already have the results
    // (the associated value). The solver does not need to be called
    std::map<std::set<Instruction*>, bool> readFromCache;
    std::set<Instruction*> curInsts;

    // The reads-from constraints are only valid for the current interation;
    // so, save a backtracking location in the solver.
    Z3_fixedpoint_push(ctx, zfp);
    bool violates = false;

    // Add the read-from constraints from the passed map
    for (auto i = interfs.begin(), ie = interfs.end(); i != ie; ++i) {
      Instruction *l = i->first;
      Instruction *s = i->second;
      curInsts.insert(l);
      curInsts.insert(s);
      addReadsFromFact(ctx, zfp, l ,s);
    }

    for (auto i = rfs.begin(), ie = rfs.end(); i != ie; ++i) {
      Instruction *l = i->first;
      Instruction *s = i->second;
      addReadsFromFact(ctx, zfp, l ,s);
    }

    // Check for any infeasible reads-from
    // Note: this needs to be run after the previous for-loop runs to
    // completion so that all the reads-from constraints are added
    for (auto i = interfs.begin(), ie = interfs.end(); i != ie; ++i) {
      Instruction *l = i->first;
      Instruction *s = i->second;
      Z3_bool res = queryNotReads(ctx, zfp, l, s);
      if (res == Z3_L_TRUE) {
        violates = true;
        // Finding one which violates is sufficient to stop
        break;
      }
      else if (res == Z3_L_FALSE) {
        violates = false;
        DEBUG_MSG("No program order violation, moving to next reads-from\n");
      }
      else if (res == Z3_L_UNDEF) {
        // Assume that it does not violate if it is unknown
        errs() << "[WARNING] undef result from muZ\n";
        violates = false;
      }
      else {
        assert(0 && "unreachable");
      }
    }
    // Restore the previous state of the solver. This removes all the
    // reads-from constraints added
    Z3_fixedpoint_pop(ctx, zfp);
    // Update the cache
    readFromCache[curInsts] = violates;
    DEBUG_MSG("Program order violation? " << violates << '\n');
    return violates;
  }

  // Return the results of a query on (not-rf l st)
  Z3_bool queryNotReads(z3::context &ctx, Z3_fixedpoint &zfp, Instruction *l
                       , Instruction *st) const {
    assert(Utils::isSomeLoad(l) && "reading from non-load");
    z3::expr args[2] = {getValueBVID(ctx, l), getValueBVID(ctx, st)};
    z3::expr nrfApp= getNotReadsFromFuncDecl(ctx)(2, args);
    Z3_bool ret = Z3_fixedpoint_query(ctx, zfp, nrfApp);

#if 0
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    z3::sort boolSort = ctx.bool_sort();
    //Z3_fixedpoint_register_relation(ctx, zfp
                          //, Z3_get_app_decl(ctx
                                            //, Z3_to_app(ctx, qAst)));
    //Z3_ast ldID = getValueBVID(ctx, l);
    //Z3_ast stID = getValueBVID(ctx, st);
    z3::expr args[2] = {getValueBVID(ctx, l), getValueBVID(ctx, st)};
    z3::expr nrfApp = getNotReadsFromFuncDecl(ctx)(2, args);

    // Create a relation which is true if the query is true.
    // The truth of the query is based off a rule
    z3::expr qAst = z3::expr(ctx, Z3_mk_fresh_const(ctx, "query", boolSort));
    Z3_fixedpoint_add_rule(ctx, zfp
                          , Z3_mk_implies(ctx, nrfApp, qAst), NULL);
    Z3_bool ret = Z3_fixedpoint_query(ctx, zfp, qAst);
#endif

#ifdef MK_DEBUG
    DEBUG_MSG("NRF Query Stats\n");
    z3::stats sts = z3::stats(ctx, Z3_fixedpoint_get_statistics(ctx, zfp));
    std::cerr << sts << '\n';
    std::string rules = Z3_fixedpoint_to_string(ctx, zfp, 0, NULL);
    DEBUG_MSG("Rules: " << rules << '\n';);
    DEBUG_MSG("Query: " << Z3_ast_to_string(ctx, nrfApp) << '\n');
#endif
    return ret;
  }

  // Return the results of a query on (linehigh l st)
  // Result of query: 1:true, -1:false, 0:undefined
  bool queryPri(z3::context &ctx, Z3_fixedpoint &zfp, Instruction *l
          , Instruction *st) const {
      // check nonLastSt first
      z3::expr args[1] = {getValueBVID(ctx, st)};
      z3::expr nonLastStApp = getNonLastStFuncDecl(ctx)(1, args);
      Z3_bool nonLastRet = Z3_fixedpoint_query(ctx, zfp, nonLastStApp);

      errs() << "======================================\n";
      errs() << "ID of Load: " << getId(l, Z3_BV_SIZE) << "\n";
      errs() << *l << "\n";
      errs() << "ID of Store: " << getId(st, Z3_BV_SIZE) << "\n";
      errs() << *st << "\n";
      if (nonLastRet == 1) {
          errs() << "St is nonLast\n";
      } else  {
          errs() << "St is NOT nonLast\n";
      }
      // check complete ld
      z3::expr args3[1] = {getValueBVID(ctx, l)};
      z3::expr compLdApp = getCompLdFuncDecl(ctx)(1, args3);
      Z3_bool compLdRet = Z3_fixedpoint_query(ctx, zfp, compLdApp);
      if (compLdRet == 1) {
          errs() << "Load is completeLoad\n";
      } else {
          errs() << "Load is not completeLoad\n";
      }
      if (compLdRet == 1 && nonLastRet == 1) {
          errs() << "======================================\n";
          return false;
      }
      if (compLdRet != 1 && nonLastRet != 1) {
          errs() << "======================================\n";
          return true;
      }

      // check ld-st higher
      z3::expr args1[2] = {getValueBVID(ctx, l), getValueBVID(ctx, st)};
      z3::expr lineHighApp = getLineHighFuncDecl(ctx)(2, args1);
      Z3_bool ldStRet = Z3_fixedpoint_query(ctx, zfp, lineHighApp);

      // check st-ld higher
      z3::expr args2[2] = {getValueBVID(ctx, st), getValueBVID(ctx, l)};
      z3::expr lineHighApp2 = getLineHighFuncDecl(ctx)(2, args2);
      Z3_bool stLdRet = Z3_fixedpoint_query(ctx, zfp, lineHighApp2);

      if (compLdRet != 1 && nonLastRet == 1 && ldStRet == 1) {
          errs() << "ld is higher than st\n";
          errs() << "======================================\n";
          return true;
      } else if (compLdRet !=1 && nonLastRet == 1 && ldStRet != 1) {
          errs() << "ld is not higher than st\n";
          errs() << "======================================\n";
          return false;
      } else if (compLdRet == 1 && nonLastRet != 1 && stLdRet == 1) {
          errs() << "st is higher than ld\n";
          errs() << "======================================\n";
          return true;
      } else if (compLdRet == 1 && nonLastRet != 1 && stLdRet != 1) {
          errs() << "st is not higher than ld\n";
          errs() << "======================================\n";
          return false;
      } else {
          assert(0 && "There is no case for here");
      }

      /*
         std::cout << "Priority Query Stats";
         z3::stats sts = z3::stats(ctx, Z3_fixedpoint_get_statistics(ctx, zfp));
         std::cerr << sts << "\n";
       */

//#ifdef MK_DEBUG
      //DEBUG_MSG("Priority Query Stats\n");
      //z3::stats sts = z3::stats(ctx, Z3_fixedpoint_get_statistics(ctx, zfp));
      //std::cerr << sts << '\n';
//#endif
  }

  // Return the results of a query on (MHB l st)
  Z3_bool queryMHB(z3::context &ctx, Z3_fixedpoint &zfp, Instruction *l
                       , Instruction *st) const {
    z3::expr args[2] = {getValueBVID(ctx, l), getValueBVID(ctx, st)};
    z3::expr mhbApp = getMHBFuncDecl(ctx)(2, args);
    Z3_bool ret = Z3_fixedpoint_query(ctx, zfp, mhbApp);
#ifdef MK_DEBUG
    DEBUG_MSG("MHB Query Stats\n");
    z3::stats sts = z3::stats(ctx, Z3_fixedpoint_get_statistics(ctx, zfp));
    std::cerr << sts << '\n';
#endif
    return ret;
  }

  // Given an instruction and Value found the closest preceding store within
  // the same function as the instruction to the passed Value.
  //
  // If there are none, returns an empty vector.
  static std::vector<Instruction *> getPreviousStores(Instruction *i, Value *v) {
    assert(i != NULL && "getPreviousStores(i,v): NULL instruction");
    assert(v != NULL && "getPreviousStores(i,v): NULL value");
    DEBUG_MSG("[DEBUG] getPreviousStores(): " << *i << ", v: " << *v << '\n');
    Function *f = i->getParent()->getParent();
    BasicBlock *b = i->getParent();

    // Check if there is simply a store in this basicblock
    Instruction *prevStore = NULL;
    for (Instruction &bbi : *b) {
      if (&bbi == i) {
        break;
      }
      Value *strPtr = Utils::getStorePtr(&bbi);
      if (strPtr == v) {
        prevStore = &bbi;
      }
    }
    if (prevStore != NULL) {
      std::vector<Instruction *> ret;
      ret.push_back(prevStore);
      return ret;
    }

    // We know there is no store before `i` to `v` in the BasicBlock. If there
    // is one it must come from some other BasicBlock.
    std::map<BasicBlock *, std::set<Instruction*>> reach = getReachingStores(f);
    std::set<Instruction*> bReachIn = Utils::mapAtUnsafe(reach, b);
    DEBUG_MSG("  numReachingStores:" << bReachIn.size() << '\n');

    // Find all the incoming stores to `v`
    std::vector<Instruction*> storesToV;
    for (auto it = bReachIn.begin(), et = bReachIn.end(); it != et; ++it) {
      Instruction *st = *it;
      assert(st != NULL && "getPreviousStores: NULL in reachable map");
      Value *stPtr = Utils::getStorePtr(st);
      assert(stPtr != NULL
             && "getPreviousStores: reachable store with NULL ptr operand");
      if (stPtr == v) {
        storesToV.push_back(st);
      }
    }

    // storesToV contains zero or more preceeding stores
    return storesToV;

    //if (storesToV.size() == 0) {
    //  // There are no stores before `i` to `v` in the BasicBlock and there are no 
    //  return storesToV;
    //}
    //else if (storesToV.size() == 1) {
    //  Instruction *storeIn = storesToV[0];
    //  assert(storeIn != NULL && "getPreviousStore: NULL reachable store");
    //  assert(Utils::getStorePtr(storeIn) != NULL 
    //         && "getPreviousStore: storeIn with NULL ptr operand");
    //  return storeIn;
    //}
    //else {
    //  DEBUG_MSG("num reaching stores:" << storesToV.size() << '\n');
    //  for (size_t i = 0; i < storesToV.size(); ++i) {
    //    DEBUG_MSG("  " << *(storesToV[i]) << '\n');
    //  }
    //  assert(0 && "getPreviousStore: more than one reaching store");
    //}
  }

  // CallInst is the thread_create function of `i`. Attempt to look backwards
  // from i to find a previous store including looking before the thread
  // creation site.
  static std::vector<Instruction *> getPreviousStoresWithCreate(Instruction *i
      , CallInst *cre) {
    if (LoadInst *l = dyn_cast<LoadInst>(i)) {
      Value *v = l->getPointerOperand();
      std::vector<Instruction *> ret = getPreviousStores(i, v);
      if (ret.size() > 0) {
        // Found some previous store
        return ret;
      }
      else {
        // Try to look back from the creation site. 
        // TODO: this could continue up the thread creation chain but it doesn't
        DEBUG_MSG("[DEBUG] Search searching back from creation stite for store\n");
        if (cre == NULL) {
          DEBUG_MSG("  NULL creation site, not searching\n");
          return ret; // ret.size <= 0
        }
        ret = getPreviousStores(cre, v);
        return ret;
      }
    }
    else {
      errs() << "[ERROR] getPreviousStoreWithCreate: unimplemented instruction type: "
             << * i << '\n';
      assert(0 && "unreachable");
    }
  }

  // Given an instruction performing a load find the closest precding store
  // within the same Function. If no store is found, return NULL.
  static std::vector<Instruction *> getPreviousStore(Instruction *i) {
    if (LoadInst *l = dyn_cast<LoadInst>(i)) {
      Value *v = l->getPointerOperand();
      return getPreviousStores(i, v);
    }
    else {
      errs() << "[ERROR] getPreviousStores: unimplemented instruction type: "
             << * i << '\n';
      assert(0 && "unreachable");
    }
    //DEBUG_MSG("[DEBUG] getPreviousStore(): " << *l << '\n');
    //Function *f = l->getParent()->getParent();
    //BasicBlock *b = l->getParent();
    //std::map<BasicBlock *, std::set<Instruction*>> reach = getReachingStores(f);
    //std::set<Instruction*> bReachIn = Utils::mapAtUnsafe(reach, b);
    //DEBUG_MSG("  numReachingStores:" << bReachIn.size() << '\n');
    //if (bReachIn.size() == 0) {
    //  return NULL;
    //}
    //else if (bReachIn.size() == 1) {
    //  Instruction *storeIn = *(bReachIn.begin());
    //  assert(storeIn != NULL && "getPreviousStore: NULL reachable store");
    //  assert(Utils::getStorePtr(storeIn) != NULL 
    //         && "getPreviousStore: storeIn with NULL ptr operand");
    //  // Check for any killing stores before l
    //  for (Instruction &bbi : *(l->getParent())) {
    //    if (&bbi == l) {
    //      // Reached the target instruction
    //      break;
    //    }
    //    Value *strPtr = Utils::getStorePtr(&bbi);
    //    if (strPtr != NULL && strPtr == Utils::getStorePtr(storeIn)) {
    //      // strPtr kills in incoming store
    //      storeIn = &bbi;
    //    }
    //  }
    //  return storeIn;
    //}
    //else {
    //  assert(0 && "getPreviousStore: more than one reaching store");
    //}
  }

  // Return all the store instructions which reach the entry of each basicblock
  // in F.
  //
  // Memoizes the results based on the functon address. So, this assumes the
  // function is never modified.
  static std::map<BasicBlock *, std::set<Instruction *>> getReachingStores(Function *f) {
    static std::map<Function *, std::map<BasicBlock *, std::set<Instruction *>>> memoizer;
    DEBUG_MSG("Get reaching stores:" << f->getName() << '\n');
    auto memIt = memoizer.find(f);
    if (memIt != memoizer.end()) {
      return memIt->second;
    }
    // Defininitions reaching the end of a BB
    std::map <BasicBlock *, std::set<Instruction *>> reachOut;
    std::map <BasicBlock *, std::set<Instruction *>> reachIn;
    // Definitions generated by a basicblock
    std::map <BasicBlock *, std::set<Instruction *>> bb2gen;
    // Variables overwritten by a basicblock
    std::map <BasicBlock *, std::set<Value *>> bb2rem;

    // Initialze all the maps
    for (BasicBlock &B : *f) {
      bb2gen[&B] = getGen(&B);
      bb2rem[&B] = getRemVars(&B);
      reachOut[&B] = std::set<Instruction*>();
    }

    bool updated = true;
    while (updated) {
      updated = false;
      for (BasicBlock &B : *f) {
        std::set<Instruction *> curGen = Utils::mapAtUnsafe(bb2gen, &B);
        std::set<Value *> curRem = Utils::mapAtUnsafe(bb2rem, &B);
        // The definitions reaching the input to this block is the union of the
        // output of the predecessors. Iterate over all the preds and find this
        // union.
        std::set<Instruction*> curReach;
        for (auto it = pred_begin(&B), et = pred_end(&B); it != et; ++it) {
          BasicBlock *pred = *it;
          std::set<Instruction *> predReachOut = Utils::mapAtUnsafe(reachOut, pred);
          curReach.insert(predReachOut.begin(), predReachOut.end());
        }
        reachIn[&B] = curReach;

        // Remove all those things which were killed
        for (Value *v : curRem) {
          for (auto it = curReach.begin(), et = curReach.end(); it != et; ++it) {
            Instruction *curSt = *it;
            Value *stPtr = Utils::getStorePtr(curSt);
            assert(stPtr != NULL && "reaching store with NULL pointer arg");
            if (v == stPtr) {
              curReach.erase(it);
            }
          }
        }
        // Add all those things generated
        curReach.insert(curGen.begin(), curGen.end());
        std::set<Instruction*> oldReach = Utils::mapAtUnsafe(reachOut, &B);
        if (curReach.size() > oldReach.size()) {
          updated = true;
          reachOut[&B] = curReach;
        }
      }
    } // while (updated)
    memoizer[f] = reachIn;
    return reachIn;
  }

  // Get those stores created in the B
  static std::set<Instruction *> getGen(BasicBlock *B) {
    // A map keeping track of which values already have stores associated with
    // them.
    std::map<Value *, Instruction *> valToSt;
    // Assumption: the for loop below goes in order
    for (Instruction &i : *B) {
      Value *stPtr = Utils::getStorePtr(&i);
      if (stPtr != NULL) {
        // Only maintain the most recent store to each value
        valToSt[stPtr] = &i;
      }
    }
    // Flatten the map to a set
    std::set<Instruction*> ret;
    for (auto it = valToSt.begin(), et = valToSt.end(); it != et; ++it) {
      ret.insert(it->second);
    }
    return ret;
  }

  static std::set<Value *> getRemVars(BasicBlock *B) {
    std::set<Value *> ret;
    for (Instruction &i : *B) {
      if (Utils::isSomeStore(&i)) {
        Value *p = Utils::getStorePtr(&i);
        assert(p != NULL && "getKilledVars: store w/o ptr operand");
        ret.insert(p);
      }
    }
    return ret;
  }


  // Return a read-from rule using the two passed strings. Also returns a query
  // if the read is satisfiable (check if not-rf is SAT)
  static std::string getReadFromRuleAndQuery(const std::string loader
      , const std::string storer) {
    std::string ret = "";
    ret += "(rule (rf " + loader + " " + storer + "))\n";
    ret += "(query (not-rf " + loader + " " + storer + "))";
    return ret;
  }

  // Merge the contents of m1 with m2. If they both have Facts for the same
  // LatticeFact, the facts will be joined ("unioned") together
  std::map<BasicBlock *, LatticeFact> mergeFactMaps(
      const std::map<BasicBlock *, LatticeFact> m1 
      , const std::map<BasicBlock *, LatticeFact> m2) const {
    // First, make return contain everything in m1
    std::map<BasicBlock *, LatticeFact> ret = m1;
    // Then, merge everything in m2 with the return
    for (auto i = m2.begin(), ie = m2.end(); i != ie; ++i) {
      BasicBlock *b = i->first;
      LatticeFact f = i->second;
      auto it = ret.find(b);
      if (it == ret.end()) {
        // Not found in ret , no need to join
        ret.emplace(b, f);
      }
      else {
        // Join the two facts
        LatticeFact otherF = it->second;
        LatticeFact joinedF = LatticeFact::factJoin(f, otherF);
        ret.erase(b);
        ret.emplace(b, joinedF);
      }
    }
    return ret;
  }

  // Helper function for analyzeThreadInterf().
  // Collapses the map from functions to maps of storeinsts to facts to a map
  // from storeinsts to lattice facts. Then calls the function performing the
  // analysis.
  // This merge can be done because each store instruction is unique.
  std::map<BasicBlock *, LatticeFact> analyzeThreadInterf(
      ap_manager_t *man
      , ap_environment_t *env
      , Function *f
      , CallInst *createSite
      , const std::map<Instruction *, std::vector<Instruction *>> interfs
      , const std::map<Function *, std::map<Instruction *, LatticeFact>> fInterfs
      //, std::vector<std::pair<Instruction *, LatticeFact>> &rErrors
      , std::set<Instruction *> &rErrors
      , std::map<Instruction *, LatticeFact> &reachableStores
      , std::map<CallInst *, LatticeFact> &reachThreadCreate
      , LatticeFact entryFact
			, const std::set<Instruction *> assertSlice
			, const std::set<Instruction *> impacted
			, const std::set<Instruction *> mayImpact
      , const std::map<Value *, std::string> v2n
      , z3::context &ctx
      , Z3_fixedpoint &zfp
      ) {
    DEBUG_MSG("[DEBUG] Printing interference map\n");
    DEBUG(printFuncInterfMap(fInterfs););
    std::map<Instruction *, LatticeFact> storeToInterf = flattenReachStores(fInterfs);
    return analyzeThreadInterf(man, env, f, createSite, interfs, storeToInterf, rErrors
        , reachableStores, reachThreadCreate, entryFact, assertSlice, impacted, mayImpact
				, v2n, ctx, zfp);
  }

  // Given a set of interferences from all threads, i.e., that generated on the
  // current iteration of the analysis, return a map from each load instruction
  // in the passed function to a vector of StoreInsts it could read from
  //
  // In other words, map each load instruction within the passed function
  // (thread) to every store instruction to the same variable from other
  // threads
  //
  // Note: The LatticeFacts in the inner map of interfs is not actually used
  // (i.e., interfs could be a map from functions to lists of storeinsts).
  //
  // TODO: This cannot handle the same function interfering with itself (i.e.,
  // multiple threads executing the same function)
  std::map<Instruction *, std::vector<Instruction*>> getInterferingStores(
      Function *f
      , const std::map<Function *, std::map<Instruction *, LatticeFact>> rStores
      , const std::map<Value *, std::string> v2n
    ) const {
    DEBUG_MSG("[DEBUG] Getting interfering stores for function: " 
        << f->getName() << '\n');
    std::map<Instruction *, std::vector<Instruction*>> ret;
    for (auto i = rStores.begin(), ie = rStores.end(); i != ie; ++i) {
      Function *otherF = i->first;
      if (otherF == f) {
        // A function cannot interfere with itself (the only case where this
        // can happen is for unbounded thread instances)
        continue;
      }
      std::map<Instruction *, LatticeFact> currStores = i->second;
      std::vector<Instruction *> fLoads = getLoads(f);
      for (Instruction *l : fLoads) {
        // For each load, find all the stores in the current thread and get
        // the interfering fact
        for (auto j = currStores.begin(), je = currStores.end()
            ; j != je; ++j) {
          Instruction *s = j->first;
          if (loadStoreSameVal(s, l)) {
            DEBUG_MSG("Found intefering store: " << *s << "\nt" << *l << '\n');
            Utils::mapInsertToVector(ret, l, s);
            //LatticeFact sFact = j->second;
            //mapInsertToVector(ret, l, sFact);
          }
        }
      }
    } // for (auto ii = rStores.begin() ... )
    return ret;
  }

  // Given a map of loads to their corresponding interfering stores, remove
  // those stores which have lower priority than loads
  std::map<Instruction*, std::vector<Instruction *>> filterPriInterfs(
      z3::context &ctx
      , Z3_fixedpoint &zfp
      , std::map<Instruction *, std::vector<Instruction *>> interfs) const {
    static std::map<std::pair<Instruction*, Instruction*>, Z3_bool> cache;
    DEBUG_MSG("Filtering Priority Interferences\n");
    // Map from a Load to the vector of interfering stores
    std::map<Instruction *, std::vector<Instruction *>> ret;
#ifdef MK_DEBUG
    if (interfs.size() == 0) {
      DEBUG_MSG("No interferences to filter\n");
    }
#endif
    for (auto it = interfs.begin(); it != interfs.end(); ++it) {
      Instruction *curL = it->first;
      std::vector<Instruction *> stores = it->second;
      // Pass only stores which have higher priority
      std::vector<Instruction *> lInterfs;
      // Issue a query for every load--store pair
      for (Instruction *st : stores) {
        // Use cached query result if possible
        bool queryResult;
        auto cacheIt = cache.find(std::make_pair(curL, st));
        if (cacheIt == cache.end()) {
          DEBUG_MSG("Priority filter cache miss\n");
          //errs() << "Priority filter cache miss\n";
          // never seen this pair before
          queryResult = queryPri(ctx, zfp, curL, st);
          cache[std::make_pair(curL, st)] = queryResult;
        } else {
          DEBUG_MSG("Priority filter cache hit\n");
          //errs() << "Priority filter cache hit\n";
          queryResult = cacheIt->second;
        }
        totalPairs ++;
        if (queryResult) {
            // if query is true than it is valid pair
            lInterfs.push_back(st);
        } else {
            filteredPairs ++;
            /*
          errs() << "Infeasible load--store from MHB\n";
          errs() << "Load: " << *curL << '\n';
          errs() << "Load Function: " << curL->getParent()->getParent()->getName() << '\n';
          errs() << "Store: " << *st << '\n';
          errs() << "Store Function: " << st->getParent()->getParent()->getName() << '\n';
          */
          DEBUG_MSG("Infeasible load--store from Prioirity\n");
          DEBUG_MSG("Load: " << *curL << '\n');
          DEBUG_MSG("Load Function: " << curL->getParent()->getParent()->getName() << '\n');
          DEBUG_MSG("Store: " << *st << '\n');
          DEBUG_MSG("Store Function: " << st->getParent()->getParent()->getName() << '\n');
        }
      }
      assert(lInterfs.size() <= stores.size());
#ifdef MK_DEBUG
      if (lInterfs.size() < stores.size()) {
        DEBUG_MSG("Pruned interferences via priority: " 
                  << stores.size() - lInterfs.size() << '\n');
      } else {
        DEBUG_MSG("No interferences pruned via priority\n");
      }
#endif
      ret[curL] = lInterfs;
    }
    return ret;
  }

  // Given a map of loads to their corresponding interfering stores, remove
  // those stores which must-happen-after the load. These can be removed since
  // the effect of the store cannot travel backwards in time. These removals
  // are valid over all interference permutations
  std::map<Instruction *, std::vector<Instruction *>> filterMHBInterfs(
      z3::context &ctx
      , Z3_fixedpoint &zfp
      , std::map<Instruction *, std::vector<Instruction *>> interfs) const {
    // Only need to ever check MHB once per load-store pair
    static std::map<std::pair<Instruction*, Instruction*>, Z3_bool> cache;
    DEBUG_MSG("Filtering MHB Interferences\n");
    // Map from a Load to the vector of interfering stores
    std::map<Instruction *, std::vector<Instruction *>> ret;
#ifdef MK_DEBUG
    if (interfs.size() == 0) {
      DEBUG_MSG("No interferences to filter\n");
    }
#endif
    for (auto it = interfs.begin(); it != interfs.end(); ++it) {
      Instruction *curL = it->first;
      std::vector<Instruction *> stores = it->second;
      // Interfering stores which do not violate MHB. Will be populated in the
      // for-loop below
      std::vector<Instruction *> lInterfs;
      // Issue a query for every load--store pair
      for (Instruction *st : stores) {
        // Use cached query result if possible
        Z3_bool queryResult;
        auto cacheIt = cache.find(std::make_pair(curL, st));
        if (cacheIt == cache.end()) {
          DEBUG_MSG("MHB filter cache miss\n");
          // never seen this pair before
          queryResult = queryMHB(ctx, zfp, curL, st);
          cache[std::make_pair(curL, st)] = queryResult;
        }
        else {
          DEBUG_MSG("MHB filter cache hit\n");
          queryResult = cacheIt->second;
        }
        if (queryResult == Z3_L_FALSE || queryResult == Z3_L_UNDEF) {
          // either l may-not-happen before st, or unknown. In both cases,
          // conservatively assume that the load can read from the store
          lInterfs.push_back(st);
        }
        else {
          DEBUG_MSG("Infeasible load--store from MHB\n");
          DEBUG_MSG("Load: " << *curL << '\n');
          DEBUG_MSG("Load Function: " << curL->getParent()->getParent()->getName() << '\n');
          DEBUG_MSG("Store: " << *st << '\n');
          DEBUG_MSG("Store Function: " << st->getParent()->getParent()->getName() << '\n');
        }
      }
      assert(lInterfs.size() <= stores.size());
#ifdef MK_DEBUG
      if (lInterfs.size() < stores.size()) {
        DEBUG_MSG("Pruned interferences via MHB: " 
                  << stores.size() - lInterfs.size() << '\n');
      }
      else {
        DEBUG_MSG("No interferences pruned via MHB\n");
      }
#endif
      ret[curL] = lInterfs;
    }
    return ret;
  }

  // Combine the inner map of the passed across all functions.
  std::map<Instruction *, LatticeFact> flattenReachStores(
      const std::map<Function *, std::map<Instruction *, LatticeFact>> reachStrs
      ) {
    std::map<Instruction *, LatticeFact> facts;
    for (auto i = reachStrs.begin(), ie = reachStrs.end(); i != ie; ++i) {
      std::map<Instruction *, LatticeFact> cur = i->second;
      for (auto j = cur.begin(), je = cur.end(); j != je; ++j) {
        Instruction *s = j->first;
        // Assumption: Store instructions are unique
        assert(facts.find(s) == facts.end() && "duplicate store");
        facts.emplace(s, j->second);
      }
    }
    return facts;
  }

  // Return true if every fact for every storeinst in new is contained in old
  bool stableStores(
          const std::map<Function *, std::map<Instruction *, LatticeFact>> oldI
          , std::map<Function *, std::map<Instruction *, LatticeFact>> newI) {
      // Since the store instructions are unique (not shared across threads).
      // we can flatten the results and just work on the combined inner maps
      std::map<Instruction *, LatticeFact> flatOld = flattenReachStores(oldI);
      std::map<Instruction *, LatticeFact> flatNew = flattenReachStores(newI);
      // Check if every fact in new is in old
      for (auto ni = flatNew.begin(), ne = flatNew.end(); ni != ne; ++ni) {
          Instruction *s = ni->first;
          LatticeFact newF = ni->second;
          auto oi = flatOld.find(s);
          if (oi == flatOld.end()) {
              // This means the store was not even reachable in the prior execution
              // (but, it was reachable in this execution). So, we can stop now since
              // we have a completely new fact created in new
              assert(!newF.isBottom() && "reachable bottom fact");
              DEBUG_MSG("stableStores(): new fact not in old, returning false\n");
              return false;
          }
          // Compare the old and new facts
          else {
              LatticeFact oldF = oi->second;
              // If the new fact is definitely larger (strictly greater than) than
              // the old one, we can stop
              if (!LatticeFact::areValsEq(newF, oldF)
                      && LatticeFact::areValsGEq(newF, oldF)) {
                  return false;
              }
          }
      }
      // At this point, we know none of the new facts are *definitively* larger
      // than the old facts. areValsGEq will return false either in the case of
      // "no" or "don't know"
      // TODO: What to do in this case? return true?
      return true;
  }

  // Convience wrapper to analyze a function in the presense of no inteferences
  std::map<BasicBlock *, LatticeFact> analyzeFuncNoInterf(ap_manager_t *man
          , ap_environment_t *env
          //, std::vector<std::pair<Instruction *, LatticeFact>> &rErrors
          , std::set<Instruction *> &rErrors
          , std::map<Instruction *, LatticeFact> &reachableStores
          , std::map<CallInst *, LatticeFact> &reachThreadCreate
          , LatticeFact entryFact
          , const std::set<Instruction *> assertSlice
          , const std::set<Instruction *> impacted
          , const std::set<Instruction *> mayImpact
          , const std::map<Value *, std::string> &valToName
          , Function *f) const {
      std::map<Instruction *, LatticeFact> empty;
      return analyzeFunc(man, env, empty, rErrors, reachableStores
              , reachThreadCreate, assertSlice, impacted, mayImpact
              , valToName, entryFact, f, false);
  }

  // Analyze the passed function. Returns a map from each basicblock in the
  // function to the stabalized facts at the begining of the block.
  //
  // The passed manager and environment are assumed to be initialized. The
  // environment should at least include every variable in the passed map from
  // LLVM Values to Apron names.
  //
  // The passed vector, rErrors, will be updated with any reachable errors
  // states and the fact reaching the statement.
  //
  // If mergeInterfs is true, then the analysis will simultaneously consider
  // both any interfering memory state and the state within the thread. This
  // means, on any load of `x` both the value within the thread for `x` and the
  // value of `x` in the interference are loaded.
  std::map<BasicBlock *, LatticeFact> analyzeFunc(ap_manager_t *man
          , ap_environment_t *env
          , const std::map<Instruction *, LatticeFact> interf
          //, std::vector<std::pair<Instruction*, LatticeFact>> &rErrors
          , std::set<Instruction*> &rErrors
          , std::map<Instruction *, LatticeFact> &reachableStores
          , std::map<CallInst *, LatticeFact> &reachThreadCreate
          , const std::set<Instruction *> assertSlice
          , const std::set<Instruction *> impacted
          , const std::set<Instruction *> mayImpact
          , const std::map<Value *, std::string> &valToName
          , LatticeFact entryFact
          , Function *f
          , bool mergeInterfs) const {
      assert(f && "analyzeFunc: null passed");

      if (f->begin() == f->end()) {
          errs() << "[ERROR] function has no body\n";
          exit(EXIT_FAILURE);
      }

      // The worklist. It is a map from a basicblock to the new facts reaching
      // the block. A map is used so that if multiple predecessors of a
      // basicblock add items to the worklist they can be met. In this way, the
      // predecessors of a BasicBlock do not need to be explicitly found.
      std::map<BasicBlock *, LatticeFact> worklist;

      // Bottom and top facts used in initialization
      ap_abstract1_t factBot = ap_abstract1_bottom(man, env);
      ap_abstract1_t factTop = ap_abstract1_top(man, env);

      LatticeFact latBot = LatticeFact(&factBot);
      LatticeFact latTop = LatticeFact(&factTop);

      // The current state of the analysis. 
      // This is the facts at the begining of each basicblock.
      // If a key is not in the map it is assumed to be an error.
      // The initial state is passed by the user
      std::map<BasicBlock *, LatticeFact> analState = initBBs(latBot, f);


      //Module *M = f->getParent();
      //LatticeFact initFact = getInitState(M, latTop, valToName);
      // get the first basic block
      BasicBlock &firstBb = *(f->begin());
      worklist.emplace(&firstBb, entryFact);

      while (worklist.size()) {
          try {
              DEBUG_MSG("New worklist iteration, size: " 
                      << worklist.size() << '\n');
              // Iterator to first item in the list
              auto it = worklist.begin();
              auto cur = *it;
              worklist.erase(it);

              BasicBlock *curBb = cur.first;
              LatticeFact newFact = cur.second;
              DEBUG_MSG("[DEBUG] analyzing block: " << *curBb << '\n');

              LatticeFact oldFact = analState.at(curBb);

              DEBUG_MSG("  Incomming Fact: ");
              DEBUG(newFact.fprint(stderr););
              DEBUG_MSG("  Old Fact: ");
              DEBUG(oldFact.fprint(stderr););
              DEBUG_MSG("  new leq old? " << newFact.leq(oldFact) << '\n');

              // If the new fact is the same size or smaller than the old one 
              // then we can stop. 
              // It should never really be smaller (assuming the transfer
              // functions are monotonicly increasing).
              if (newFact.leq(oldFact)) {
                  continue;
              }
              // If the new fact is bigger, save it in the analysis state 
              // Note: the end fact of each block is not saved 
              // (but, it can be recovered by just running the 
              // transfer function over the input fact)
              analState.erase(curBb);
              analState.emplace(curBb, newFact);

              std::vector<std::pair<BasicBlock*, LatticeFact> > outFacts
                  = BBOps::run_transfer_funcs(curBb, newFact, interf, rErrors
                          , reachableStores, reachThreadCreate, assertSlice
                          , impacted, mayImpact, valToName, noCombinsG);
              // Combine the outfacts of the just analyzed block with any in the
              // worklist
              //worklist = merge_worklist(worklist, outFacts);
              merge_worklist(&worklist, outFacts);
          }
          catch (const std::out_of_range &oor) {
              errs() << "[ERROR] item not found in analysis state\n";
              assert(0 && "see above");
              exit(EXIT_FAILURE);
          }
      }

      // Cleanup the initial abstract value
      ap_abstract1_clear(man, &factBot);
      ap_abstract1_clear(man, &factTop);

      return analState;
  }

  // Add the "dom" relation and it's transitive property.
  void addDomRel(z3::context &ctx, Z3_fixedpoint &zfp) {
    // Create dominance relation
    z3::func_decl domDecl = getDomFuncDecl(ctx);
    Z3_fixedpoint_register_relation(ctx, zfp, domDecl);

    // Create post-dominance relation
    z3::func_decl postDomDecl = getPostDomFuncDecl(ctx);
    Z3_fixedpoint_register_relation(ctx, zfp, postDomDecl);


    addTransRuleBVBV(ctx, zfp, domDecl, Z3_BV_SIZE);
    addTransRuleBVBV(ctx, zfp, postDomDecl, Z3_BV_SIZE);
  }

  // And relations indicating a certain instruction is either a load or store
  void addLoadStoreRels(z3::context &ctx, Z3_fixedpoint &zfp) {
    // Store relation: (st s v): store s loads value v
    z3::func_decl stDecl = getStFuncDecl(ctx);
    Z3_fixedpoint_register_relation(ctx, zfp, stDecl);

    // Load relation
    z3::func_decl ldDecl = getLdFuncDecl(ctx);
    Z3_fixedpoint_register_relation(ctx, zfp, ldDecl);
  }

  // Add MHB relation
  void addMHBRel(z3::context &ctx, Z3_fixedpoint &zfp) {
    z3::func_decl mhbDecl = getMHBFuncDecl(ctx);
    Z3_fixedpoint_register_relation(ctx, zfp, mhbDecl);
  }

  // Add fence relation
  void addFenceRel(z3::context &ctx, Z3_fixedpoint &zfp) {
    z3::func_decl fenceDecl = getFenceFuncDecl(ctx);
    Z3_fixedpoint_register_relation(ctx, zfp, fenceDecl);
  }
  
  void addLwSyncRel(z3::context &ctx, Z3_fixedpoint &zfp) {
    z3::func_decl fenceDecl = getFenceFuncDecl(ctx);
    Z3_fixedpoint_register_relation(ctx, zfp, fenceDecl);
  }

  // Add not-reach relation
  void addNotReachRel(z3::context &ctx, Z3_fixedpoint &zfp) {
    z3::func_decl nreachRel = getNotReachFuncDecl(ctx);
    Z3_fixedpoint_register_relation(ctx, zfp, nreachRel);
  }

  void addReadsFromRel(z3::context &ctx, Z3_fixedpoint &zfp) {
    z3::func_decl rf = getReadsFromFuncDecl(ctx);
    Z3_fixedpoint_register_relation(ctx, zfp, rf);
  }

  void addNoReorderRel(z3::context &ctx, Z3_fixedpoint &zfp) {
    z3::func_decl no = getNoReorderFuncDecl(ctx);
    Z3_fixedpoint_register_relation(ctx, zfp, no);
  }

  void addNotReadsFromRel(z3::context &ctx, Z3_fixedpoint &zfp) {
    z3::func_decl nrf = getNotReadsFromFuncDecl(ctx);
    Z3_fixedpoint_register_relation(ctx, zfp, nrf);
  }


  // Add MHB edges from creation site to child
  //
  // Currently, this requires that all the pthread_create sites have explicit
  // function arguments.
  void addCreateFacts(Module &M, z3::context &ctx, Z3_fixedpoint &zfp) {
    for (Function &F : M) {
      for (BasicBlock &B : F) {
        for (Instruction &I : B) {
          CallInst *ci = dyn_cast<CallInst>(&I);
          if (ci == NULL) {
            continue;
          }
          Function *called = ci->getCalledFunction();
          if (called->getName() != "pthread_create") {
            continue;
          }
          // Found pthread_create. The 2nd argument (zero indexed) is the
          // function being called
          Value *v = ci->getArgOperand(2);
          if (Function *tFunc = dyn_cast<Function>(v)) {
            // Make sure the function has a body
            if (tFunc->empty()) {
              errs() << "[WARNING] thread function without body\n";
              continue;
            }
            // The pthread_create site must happen before the first instruction
            // in the function
            //addMHBFact(ctx, zfp, ci, &tFunc->front().front());
            addMHBFact(ctx, zfp, ci
                , getFirstLoadStoreCallTerm(&tFunc->front()));
            addCreateStoreLoadFacts(ctx, zfp, ci, tFunc);
            addThreadLocalNotRead(ctx, zfp, ci, tFunc);
          }
          else {
            errs() << "[ERROR] pthread_create with non function 2nd arg\n";
            errs() << *v << '\n';
            exit(EXIT_FAILURE);
          }
        } // for (Instruction &I : B)
      } // for (BasicBlock &B : F)
    } // for (Function &F : M)

  }


  // Any store instruction which must ocurr before a thread's creation cannot
  // be read by this thread.
  //
  // If the store before the thread's creation can propagate to the thread,
  // then its value will be in the initial environment of the thread
  //
  // ci is assumed to be the pthread_create call creating tFunc
  void addCreateStoreLoadFacts(z3::context &ctx, Z3_fixedpoint &zfp
      , CallInst *ci, Function *tFunc) {
    std::vector<Instruction*> lds = getLoads(tFunc);
    for (Instruction *l : lds) {
      addStoreCreateLoadFact(ctx, zfp, ci, l);
    }
  }

  // Add: (=> (and (is-store s _) (mhb s ci)) (nrf s l))
  void addStoreCreateLoadFact(z3::context &ctx, Z3_fixedpoint &zfp
      , CallInst *ci, Instruction *l) {
    assert(Utils::isSomeLoad(l));
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    z3::expr s = z3::expr(ctx, Z3_mk_bound(ctx, 0, bvSort));
    z3::expr v = z3::expr(ctx, Z3_mk_bound(ctx, 1, bvSort));
    z3::expr ciExp = getValueBVID(ctx, ci);
    z3::expr lExp = getValueBVID(ctx, l);

    z3::expr args[2] = {s, v};
    z3::expr isStoreS = getStFuncDecl(ctx)(2, args);

    args[0] = s;
    args[1] = ciExp;
    z3::expr mhbSCi = getMHBFuncDecl(ctx)(2, args);

    args[0] = s;
    args[1] = lExp;
    z3::expr nrfSL = getNotReadsFromFuncDecl(ctx)(2, args);

    // (and (is-store s v) (mhb s ci))
    z3::expr andSMHB = isStoreS && mhbSCi;

    Z3_fixedpoint_add_rule(ctx, zfp, z3::implies(andSMHB, nrfSL), NULL);
  }

  // Add the "pri" and "higher" relation and it's transitive property
  void addPriHighRel(z3::context &ctx, Z3_fixedpoint &zfp) {

    // Create (function-decl pri (number))
    z3::func_decl priDecl = getPriFuncDecl(ctx);
    Z3_fixedpoint_register_relation(ctx, zfp, priDecl);
    // add priority related rules
    addPriRule(ctx, zfp, priDecl, Z3_BV_SIZE);

    // Create (function-decl higher (number number))
    z3::func_decl highDecl = getHighFuncDecl(ctx);
    Z3_fixedpoint_register_relation(ctx, zfp, highDecl);
    // add higher related rules
    addHighRule(ctx, zfp, priDecl, highDecl, Z3_BV_SIZE);
    // add higher transitive rules
    addTransRuleBVBV(ctx, zfp, highDecl, Z3_BV_SIZE);

    // Create (function-decl lineHigh (lineNum lineNum))
    z3::func_decl lineHighDecl = getLineHighFuncDecl(ctx);
    Z3_fixedpoint_register_relation(ctx, zfp, lineHighDecl);
    // Create (function-decl linePri (lineNum Priority))
    z3::func_decl linePriDecl = getLinePriFuncDecl(ctx);
    Z3_fixedpoint_register_relation(ctx, zfp, linePriDecl);
    // Build rules for linehigh inference
    z3::func_decl ldDecl = getLdFuncDecl(ctx);
    z3::func_decl stDecl = getStFuncDecl(ctx);
    addLineHighRule(ctx, zfp, ldDecl, stDecl, highDecl, linePriDecl, lineHighDecl, Z3_BV_SIZE);

    // Create (function-decl compLd (lineNum))
    z3::func_decl compLdDecl = getCompLdFuncDecl(ctx);
    Z3_fixedpoint_register_relation(ctx, zfp, compLdDecl);
    // Create (function-decl nonLastSt (lineNum))
    z3::func_decl nonLastStDecl = getNonLastStFuncDecl(ctx);
    Z3_fixedpoint_register_relation(ctx, zfp, nonLastStDecl);
    // Build rules for compLd and nonLastSt inferences
    z3::func_decl domDecl = getDomFuncDecl(ctx);
    z3::func_decl postDomDecl = getPostDomFuncDecl(ctx);
    addCombRule(ctx
            , zfp
            , ldDecl
            , stDecl
            , domDecl
            , postDomDecl
            , compLdDecl
            , nonLastStDecl
            , Z3_BV_SIZE);
  }

  // Update the context and fixedpoint to contain dominance relation, rules,
  // and facts, and also relations and facts about instructions being
  // loads/stores
  void addDomLoadStoreFacts(std::set<Function*> threadFuncs
                            , z3::context &ctx
                            , Z3_fixedpoint &zfp) {

    addDomRel(ctx, zfp);
    addLoadStoreRels(ctx, zfp);

    if (usePriG) {
        addPriHighRel(ctx, zfp);
    }

    // read priority info file and save priority information
    std::map<std::string, std::string> priorityInfo;
    if (usePriG) {
        std::ifstream file("priority.info");
        std::string line;
        while(std::getline(file, line)) {
            std::stringstream linestream(line);
            std::string data;
            std::string val1;
            // read up-to the first tab (discard tab).
            std::getline(linestream, data, ':');  
            // Read the integers using the operator >>
            linestream >> val1;
            //errs() << data << ":::" << val1 << "\n";
            priorityInfo.insert(
                    std::pair<std::string, std::string> (data, val1));
        }
    }

    for (Function *f : threadFuncs) {
        unsigned priority;
        if (usePriG) {
            // get priority information
            std::string name = f->getName();
            //errs() << "function: " << name << "\n";
            if (name.compare("main") == 0) {
                // for the main function priority is just 1
                priority = 1;
            } else {
                priority = 
                    std::stoi(priorityInfo.find(f->getName().str())->second);
            }
        }

        //DominatorTree &dt = getAnalysis<DominatorTreeWrapperPass>(*f).getDomTree();
        DominatorTree dt = DominatorTree(*f);
        PostDominatorTree pdt;
        pdt.runOnFunction(*f);

        // The first instruction of the thread may have a happens-before edge
        // from the parent-thread. So, connect this first instruction (which may
        // not be a load/store) to a load/store/terminator so that transitive
        // orderings can be calculated
        /*
        Instruction *first = &f->front().front();
        Instruction *lst = getFirstLoadStoreCallTerm(&f->front());
        if (first != lst) {
            addDomFact(ctx, zfp, first, lst);
            addPostDomFact(ctx, zfp, lst, first);
        }
        */

        for (BasicBlock &b : *f) {
            // add internal dom facts of read/write/call 
            // inside of each basic block
            addInternalDomAndPostDomFacts(&b, ctx, zfp);
            if (usePriG) {
                addLoadStoreFacts(&b, ctx, zfp, priority);
            } else {
                addLoadStoreFacts(&b, ctx, zfp);
            }

            // for dominator tree
            DomTreeNodeBase<BasicBlock> *bbNode = dt.getNode(&b);
            if (bbNode != NULL) {
                // For the dom children of `b`, 
                // connect the terminator of `b` to the
                // first relevant (load, store, terminator) instruction 
                // in the child
                Instruction *from = b.getTerminator();
                const std::vector<DomTreeNodeBase<BasicBlock> *> &children 
                    = bbNode->getChildren();
                for (size_t i = 0; i < children.size(); ++i) {
                    BasicBlock *c = children[i]->getBlock();
                    // check assertion
                    Instruction *t = &(c->front());
                    if (CallInst *ci = dyn_cast<CallInst>(t)) {
                        // Chungha: This is a modification to support
                        // post-dominance relation for assert.
                        // Since assert makes branches so that the instruction
                        // after the branch can't post-dominate the instruction
                        // before the branch. Therefore, I forcely insert it.
                        if (ci->getCalledFunction()->getName() 
                                == "__assert_fail") {
                            BasicBlock *c2 = children[i+1]->getBlock();
                            Instruction *postFrom 
                                = getFirstLoadStoreCallTerm(c2);
                            /*
                            errs() << "PostFrom!!!\n";
                            errs() << *postFrom;
                            errs() << "PostTo!!!\n";
                            errs() << *from;
                            errs() << "\n";
                            */
                            addPostDomFact(ctx, zfp, postFrom, from);

                        }
                    }

                    Instruction *to = getFirstLoadStoreCallTerm(c);
                    addDomFact(ctx, zfp, from, to);
                }
            }
            // for post dominator tree
            DomTreeNodeBase<BasicBlock> *btNode = pdt.getNode(&b);
            if (btNode != NULL) {
                // For the post dom children of `b`
                // connect the first call/read/write of `b` to the 
                // last instruction in the child
                Instruction *from = getFirstLoadStoreCallTerm(&b);
                const std::vector<DomTreeNodeBase<BasicBlock> *> &children 
                    = btNode->getChildren();
                for (size_t i = 0; i < children.size(); ++i) {
                    BasicBlock *c = children[i]->getBlock();
                    //Instruction *to = getFirstLoadStoreCallTerm(c);
                    Instruction *to = c->getTerminator();
                    addPostDomFact(ctx, zfp, from, to);
                }
            }
        } // for (BasicBlock &b : *f)
    } // for (Function f : threadFuncs)
  }

  // Validate the set parameters. This will crash the program on errors
  void validateParams(z3::context &ctx, Z3_fixedpoint &zfp, z3::params &params) {
    DEBUG_MSG("checking parameters\n");
    Z3_param_descrs p_desc = Z3_fixedpoint_get_param_descrs(ctx, zfp);
    Z3_param_descrs_inc_ref(ctx, p_desc);
    Z3_params_validate(ctx, params, p_desc);
    try {
      ctx.check_error();
    }
    catch (z3::exception ex) {
      errs() << "[ERROR] Parameter error: " << ex.msg() << '\n';
      Z3_param_descrs_dec_ref(ctx, p_desc);
      exit(EXIT_FAILURE);
    }
    Z3_param_descrs_dec_ref(ctx, p_desc);
  }

  // Add facts about fence instructions in the program. This also will add a
  // definition of the fence relation (see addFenceRel()).
  void addFenceFacts(std::set<Function*> threadFuncs
                            , z3::context &ctx
                            , Z3_fixedpoint &zfp) {
    addFenceRel(ctx, zfp);
    addLwSyncRel(ctx, zfp);
    for (Function *f : threadFuncs) {
      for (BasicBlock &b : *f) {
        for (Instruction &i : b) {
          if (isFence(&i)) {
            addFenceFact(ctx, zfp, &i);
          }
          if (isLwSync(&i)) {
            // TODO: how to handle lwsync
            //addLwSyncFact(ctx, zfp, &i);
            addFenceFact(ctx, zfp, &i);
          }
        }
      }
    }
  }

  // Return true if the passed instruction is a POWER lwsync 
  //
  // An lwsync preserves everything but write--read order
  bool isLwSync(Instruction *i) {
    if (CallInst *ci = dyn_cast<CallInst>(i)) {
      // TSO/PSO fence
      if (ci->getCalledFunction()->getName() == "MLWSYNC") {
        return true;
      }
    }
    return false;
  }

  // Return true if the passed instruction operates as a fence or is an
  // explicit fence. The programmer can add in explicit fences via a call to a
  // function named MFENCE.
  bool isFence(Instruction *i) {
    if (CallInst *ci = dyn_cast<CallInst>(i)) {
      // TSO/PSO fence
      if (ci->getCalledFunction()->getName() == "MFENCE") {
        return true;
      }
      // PPC/ARM full fence
      if (ci->getCalledFunction()->getName() == "MSYNC") {
        return true;
      }
      else {
        // No other call instructions are treated as fences
        return false;
      }
    }
    else if (isa<AtomicRMWInst>(i)) {
      return true;
    }
    else if (isa<AtomicCmpXchgInst>(i)) {
      return true;
    }
    else {
      // TODO: probably some other instructions to handle as fences
      return false;
    }
  }

  bool runOnModule(Module &M) {
    manG = Utils::getManager();
    ap_manager_set_abort_if_exception(manG, AP_EXC_TIMEOUT, true);
    ap_manager_set_abort_if_exception(manG, AP_EXC_OUT_OF_SPACE, true);
    ap_manager_set_abort_if_exception(manG, AP_EXC_OVERFLOW, true);
    ap_manager_set_abort_if_exception(manG, AP_EXC_INVALID_ARGUMENT, true);
    ap_manager_set_abort_if_exception(manG, AP_EXC_NOT_IMPLEMENTED, true);
    ap_manager_set_abort_if_exception(manG, AP_EXC_SIZE, true);


    // Maps from a Value to a name and vice-versa.
    //
    // These are used since Apron requires variables to have names
    std::map<Value *, std::string> valToName;
    std::map<std::string, Value *> nameToVal;

    DEBUG_MSG("[DEBUG] Starting WorklistAI\n");
    checkCommandLineArgs();


    // Initialize fixpoint solver
    z3::config cfg;
    //Z3_set_param_value(cfg, "DL_DEFAULT_RELATION", "smt_relation2");
    z3::context ctx(cfg);
    Z3_fixedpoint zfp = Z3_mk_fixedpoint(ctx);
    Z3_fixedpoint_inc_ref(ctx, zfp);
    z3::params params(ctx);
    params.set("engine", ctx.str_symbol("datalog"));
    params.set("datalog.default_table", ctx.str_symbol("hashtable"));
    params.set("datalog.magic_sets_for_queries", true);

    DEBUG_MSG("params:\n" << Z3_fixedpoint_get_help(ctx, zfp));
    DEBUG_MSG("Set Parameters:\n");
    DEBUG_MSG(Z3_params_to_string(ctx, params) << '\n');

    validateParams(ctx, zfp, params);

    ap_manager_t* man = Utils::getManager();
    ap_environment_t *env = createEnvironment(valToName, nameToVal, M);

    DEBUG(printValToNameStderr(valToName););

    // Try to find the main function
    Function *main = getMainFuncOrNULL(M);
    if (main == NULL) {
      // TODO: the user should be able to specify the entry of the analysis
      errs() << "[ERROR] main function not found\n";
      exit(EXIT_FAILURE);
    }

    // try to find any threads
    std::set<Function *> threadFuncs = getThreads(M);
    std::vector<CallInst *> threadSites = getThreadCreate(M);
    // If there are no threads, then this is a single threaded analysis
    bool noThreads = threadSites.size() == 0;
    //threadFuncs.insert(main);



    // This also addes priority and higher information
    addDomLoadStoreFacts(threadFuncs, ctx, zfp);
    addMHBRel(ctx, zfp);

    if (useConstraintsG || filterMHB) {
      DEBUG_MSG("adding constraints: number of threads: " 
                << threadFuncs.size() << '\n');
      if (tsoConstrG || psoConstrG || rmoConstrG) {
        addFenceFacts(threadFuncs, ctx, zfp);
      }
      addCreateFacts(M, ctx, zfp);
      addJoinFacts(M, ctx, zfp);
      addNotReachableFacts(M, ctx, zfp);
      if (tsoConstrG) {
        //addTSOPruningRules(ctx, zfp);
        addNoReorderRules(ctx, zfp);
        addTSOReorderRules(ctx, zfp);
        // TODO: is reads-from MHB?
        addReadsFromIsMHB(ctx, zfp);
      }
      else if (psoConstrG) {
        //addPSOPruningRules(ctx, zfp);
        addNoReorderRules(ctx, zfp);
        addPSOReorderRules(ctx, zfp);
        // TODO: is reads-from MHB?
        addReadsFromIsMHB(ctx, zfp);
      }
      else if (rmoConstrG) {
        addNoReorderRules(ctx, zfp);
        addRMOReorderRules(ctx, zfp);
        // TODO: is reads-from MHB?
        addReadsFromIsMHB(ctx, zfp);
      }
      else {
        addPruningRules(ctx, zfp);
        // TODO: is reads-from MHB?
        addReadsFromIsMHB(ctx, zfp);
        //addNoReorderRules(ctx, zfp);
        //addSCReorderRules(ctx, zfp);
      }
    }

    std::string rules = Z3_fixedpoint_to_string(ctx, zfp, 0, NULL);
    errs() << "Rules: " << rules << '\n';
    DEBUG_MSG("Rules: " << rules << '\n';);

    // This is kind of a hack: NULL in threadSites indicates main (there is not
    // CallSite for main)
    threadSites.push_back(NULL);

    // When using constraints, we start the analysis of the main thread and
    // check the reachability of any thread creation sites. If any are
    // reachable, then they are subsequently analyzed.
    // This is a little bit roundabout since we are clearing the vectors.
    if (dynInitG) {
      threadFuncs.clear();
      threadSites.clear();
      threadFuncs.insert(main);
      threadSites.push_back(NULL);
    }

		std::set<Instruction *> assertSlice = getAssertSliceInvIfEnabled(M);
    std::set<Instruction *> impacted = getImpactedIfEnabled(M);
    std::set<Instruction *> mayImpact = getMayImpactIfEnabled(M);

    // Vector containing reachable error states
    //std::vector<std::pair<Instruction *, LatticeFact>> rErrors;
    std::set<Instruction *> rErrors;

    // Repeatedly analyze the main function and any threads until the
    // interferences (facts at stores in threads) stabalize
    std::map<Function *, std::map<Instruction *, LatticeFact>> allReachStores;
    // Map from a Function (thread) to a map from each of its Loads to the
    // vector of all interfering store instructions from other threads.
    std::map<Function *, std::map<Instruction *, std::vector<Instruction *>>> interfs;
    std::map<Function *, std::map<BasicBlock *, LatticeFact>> threadRes;
    // The inital state of the thread being analyzed
    ap_abstract1_t factTop = ap_abstract1_top(man, env);
    LatticeFact latTop = LatticeFact(&factTop);
    LatticeFact progInitState = getInitState(&M, latTop, valToName);
    ap_abstract1_clear(man, &factTop);
    // A map saving the entry fact for a thread. This is only used when
    // dynamic thread initialization in enabled
    std::map<CallInst *, LatticeFact> threadEntryFacts;
    bool stable;
    size_t iter = 1;
    do {
        errs() << "Interference Iteration: " << iter << '\n';
        // The reachable stores for this round of the analysis.
        // We save this in a new map so we can compare it with 
        // the results of the previous analysis (allReachStores). 
        // If the facts stabalize, then we can stop.
        std::map<Function *, std::map<Instruction *, LatticeFact>> newReachStores = allReachStores;
        // This deque contains the thread functions which still need to be
        // processed for this interference iteration. A deque is used because
        // when constraints are used the threads are processed as they are
        // reached. In other words, the threads to processed is increasing if a
        // new thread creation site is reached
        std::deque<CallInst *> threadsToProcess;
        // Note: when dynInitG is enabled then threadSites only contains main.
        // This means that each iteration starts analyzing main 
        // and then analyzes the children of main recursively.
        threadsToProcess.insert(threadsToProcess.end(), threadSites.begin()
                , threadSites.end());
        assert(threadsToProcess.size() == threadSites.size());
        size_t tCount = 0; // only here for output
        while (threadsToProcess.size()) {
            errs() << "Analyzing thread: " << tCount++ << '\n';
            // ci is the CallInst of the pthread_create() creating the thread.
            CallInst *ci = threadsToProcess.front();
            threadsToProcess.pop_front();
            Function *f = NULL;
            if (ci == NULL) {
                // NULL is a special callsite indicating main
                f = main;
            } else {
                f = getPThreadThreadFunc(ci);
            }
            LatticeFact entryFact = progInitState;
#if 0
            if (dynInitG) {
                // If we are dynamically calculating the entry state of 
                // each thread, then we retrieve the entry fact here. 
                // Otherwise, the entry fact is simply initialized to be 
                // the initial state of the program
                if (ci != NULL) {
                    entryFact = Utils::mapAtUnsafe(threadEntryFacts, ci);
                }
            }
#endif
            // Subset of all interferences. 
            // Only those which could affect this thread.
            std::map<Instruction *, std::vector<Instruction *>> curFInterf = interfs[f];
            curFInterf = filterMHBInterfs(ctx, zfp, curFInterf);
            if (usePriG) {
                curFInterf = filterPriInterfs(ctx, zfp, curFInterf);
            }
            // A map of reachable thread creations in `f`. 
            // This is used to determine other threads which should be analyzed
            std::map<CallInst *, LatticeFact> reachThreadCreate;
            // Analyze function in the presense of interferences
            std::map<Instruction *, LatticeFact> newStores;
            std::map<BasicBlock *, LatticeFact> res = analyzeThreadInterf(man
                    , env
                    , f
                    , ci
                    , curFInterf
                    , newReachStores
                    , rErrors
                    , newStores
                    , reachThreadCreate
                    , entryFact
                    , assertSlice
                    , impacted
                    , mayImpact
                    , valToName
                    , ctx
                    , zfp);
            threadRes[f] = res;
            // Save the reachable stores for this iteration. We can erase all the
            // old ones since the reachable stores are monotonic (on iteration i+1
            // we at least reach all the stores of iteration i)
            auto fit = newReachStores.find(f);
            if (fit != newReachStores.end()) {
                newReachStores.erase(fit);
            }
            newReachStores.emplace(f, newStores);
#if 0
            if (dynInitG) {
                // When calculating the initial state dynamically, we are calculating
                // the threads to explore on-the-fly. Any reachable pthread_create
                // call at this point needs to be analyzed during this iteration.
                // Note: the facts at each callinstruction are "forgotten" on each
                // iteration. However, the facts at the callsites are increasing
                // (potentially) since the interferences on the stores are
                // increasing. 
                for (auto rci = reachThreadCreate.begin()
                        , rce = reachThreadCreate.end(); rci != rce; ++ rci) {
                    // Get the entry fact for the thread
                    CallInst *curCall = rci->first;
                    LatticeFact callFact = rci->second;
                    Function *thrdFunc = getPThreadThreadFunc(curCall);
                    DEBUG_MSG("[DEBUG] Found new thread: " << thrdFunc->getName() 
                            << '\n');
                    // This function is now saved as being one of the threads we are
                    // analyzing
                    threadFuncs.insert(thrdFunc);
                    // Add the most recent fact to the entry point. Note: we do not
                    // support threads executing the same function
                    auto old = threadEntryFacts.find(curCall);
                    if (old != threadEntryFacts.end()) {
                        threadEntryFacts.erase(old);
                    }
                    threadEntryFacts.emplace(curCall, callFact);
                    // Add the thread to be processed
                    threadsToProcess.push_back(curCall);
                }
            }
#endif
        }
        // If there are no threads, even if the main() thread has interferences
        // they will not do anything since main does not interfer with itself
        // (the analysis will finish after a useless 2nd iteration)
        if (noThreads) {
            break;
        }
        stable = stableStores(allReachStores, newReachStores);
        DEBUG_MSG("Iteration " << iter << ":\n");
        DEBUG_MSG("Previous reachable stores:\n");
        DEBUG(printStoreFacts(allReachStores););
        DEBUG_MSG("\nNew reachable stores:\n");
        DEBUG(printStoreFacts(newReachStores););
        // Setup the data for the next iteration. This only needs to be done if
        // the results have not yet stabalized.
        // We calculate the new interferences and save the reachable facts at
        // store instructions
        if (!stable) {
            // Calculate the interferences to be used for each thread 
            // to be used in the next round of the analysis.
            // This uses the reachable stores caluclated for each thread 
            // in the previous for-loop.
            for (Function *f : threadFuncs) {
                std::map<Instruction *, std::vector<Instruction *>> interfStores;
                interfStores = getInterferingStores(f, newReachStores, valToName);
                // Remove those loads which are not the slice, those can read
                // arbitrary values without affecting property reachability
                if (assertSliceG || impactG) {
                    for (auto it = interfStores.begin(), ie = interfStores.end()
                            ; it != ie; ++it) {
                        // assertSlice contains those statements *not* 
                        // on the slice.
                        // So, if it is found in the assertSlice structure 
                        // then the statement must not be on the slice.
                        // So, its interference can be removed.
                        // TODO: for some reason performing this removal 
                        // causes some false alarms??
                        // These should be mutually exclusive so as 
                        // not to double erase
                        if (assertSliceG && assertSlice.count(it->first)) {
                            interfStores.erase(it);
                        }
                        else if (impactG && !mayImpact.count(it->first) && !impacted.count(it->first)) {
                            interfStores.erase(it);
                        }
                        //else if (impactG && !impacted.count(it->first)) {
                        //  interfStores.erase(it);
                        //}
                    }
                }
                interfs[f] = interfStores;
            }
            allReachStores = newReachStores;
        }
        iter++;
    } while (!stable);

    //std::map<BasicBlock *, LatticeFact> mainRes = threadRes[main];

    //errs() << "==== Main Results ====\n";
    //printFactMapStderr(mainRes);
    //errs() << "======================\n";

    // Print out the map from values to apron names so the results can be
    // interpreted
    printValToNameStderr(valToName);
    for (auto i = threadRes.begin(), ie = threadRes.end(); i != ie; ++i) {
        Function *f = i->first;
        std::string fname = f->getName();
        errs() << "==== " << fname << "() Results ====\n";
        std::map<BasicBlock *, LatticeFact> res = i->second;
        printFactMapStderr(res);
        errs() << "======================\n";
    }

    z3_fp_helpers::valueCacheToMetadata(M, Z3_BV_SIZE);

    // The errors are counted by the number of unique reachable error
    // statements. If the statement is reachable multiple times (i.e., on
    // multiple iterations) it is only counted once.
    //std::set<Instruction *> errInsts;
    //for (auto i : rErrors) {
    //  errInsts.insert(i.first);
    //}
    //errs() << "Errors found: " << errInsts.size() << '\n';
    errs() << "Errors found: " << rErrors.size() << '\n';
    errs() << "Max Permutations: " << maxCombPermsG << '\n';
    errs() << "Total pairs: " << totalPairs << '\n';
    errs() << "Filtered pairs: " << filteredPairs << '\n';

    // cleanup the environment (Going Green (tm))
    ap_environment_free(env);

    // Cleanup Z3 stuff
    Z3_fixedpoint_dec_ref(ctx, zfp);


    // The IR was modified with metadata only, but still modified
    return true;
  }

  // Join all the facts at the vector of stores for each load into a single
  // fact
  std::map<Instruction *, LatticeFact> joinInterfs(
      const std::map<Instruction *, std::vector<Instruction *>> l2Ss
      , const std::map<Instruction *, LatticeFact> sToF
      ) const {
    std::map<Instruction *, LatticeFact> joined;
    for (auto i = l2Ss.begin(), ie = l2Ss.end(); i != ie; ++i) {
      Instruction *l = i->first;
      std::vector<Instruction *> ss = i->second;
      if (!ss.size()) {
          continue;
      }
      assert(ss.size() && "load with zero interfering stores");
      LatticeFact lfact = Utils::mapAtUnsafe(sToF, ss[0]);
      for (size_t j = 1; j < ss.size(); ++j) {
        Instruction *s = ss[j];
        LatticeFact nextF = Utils::mapAtUnsafe(sToF, s);
        lfact = LatticeFact::factJoin(lfact, nextF);
      }
      assert(joined.find(l) == joined.end() && "duplicate load");
      joined.emplace(l, lfact);
    }
    return joined;
  }

  // Print the contents of the passed interference map
  void printStoreFacts(
      std::map<Function *, std::map<Instruction *, LatticeFact>> ints) const {
    for (auto ii = ints.begin(), ie = ints.end(); ii != ie; ++ii) {
      Function *f = ii->first;
      errs() << "Interferences for Function: " << f->getName() << '\n';
      std::map<Instruction *, LatticeFact> fInts = ii->second;
      for (auto fi = fInts.begin(), fe = fInts.end(); fi != fe; ++fi) {
        const Instruction *si = fi->first;
        LatticeFact f = fi->second;
        errs() << "\tStoreInst: " << *si << "\n\tFact: ";
        f.fprint(stderr);
      }
    }
  }

  void addLoadStoreFacts(BasicBlock *b, z3::context &ctx, Z3_fixedpoint &zfp) {
    for (auto i = b->begin(), ie = b->end(); i != ie; ++i) {
      Instruction *cur = &*i;
      if (LoadInst *l = dyn_cast<LoadInst>(cur)) {
        Value *ptrOp = l->getPointerOperand();
        addLoadFact(ctx, zfp, l, ptrOp);
      }
      else if (StoreInst *s = dyn_cast<StoreInst>(cur)) {
        Value *ptrOp = s->getPointerOperand();
        addStoreFact(ctx, zfp, s, ptrOp);
      }
    }
  }

  void addLoadStoreFacts(BasicBlock *b, z3::context &ctx, Z3_fixedpoint &zfp, unsigned priority) {
    z3::func_decl linePriDecl = getLinePriFuncDecl(ctx);
    for (auto i = b->begin(), ie = b->end(); i != ie; ++i) {
      Instruction *cur = &*i;
      if (LoadInst *l = dyn_cast<LoadInst>(cur)) {
        Value *ptrOp = l->getPointerOperand();
        addLoadFact(ctx, zfp, l, ptrOp);
        // add linepri fact for each load
        addLinePriFact(ctx, zfp, linePriDecl, l, priority);
      } else if (StoreInst *s = dyn_cast<StoreInst>(cur)) {
        Value *ptrOp = s->getPointerOperand();
        addStoreFact(ctx, zfp, s, ptrOp);
        // add linepri fact for each store
        addLinePriFact(ctx, zfp, linePriDecl, s, priority);
      }
    }
  }

  // Search for any pthread_join calls and add must-happen-before constraints;
  // see the overloaded version of this function
  void addJoinFacts(Module &M, z3::context &ctx, Z3_fixedpoint &zfp) {
    for (Function &F : M) {
      for (BasicBlock &BB : F) {
        for (Instruction &I : BB) {
          CallInst *ci = dyn_cast<CallInst>(&I);
          if (ci != NULL) {
            if (ci->getCalledFunction()->getName() == "pthread_join") {
              addJoinFacts(ci, ctx, zfp);
            }
          }
        } // for (Instruciton ...)
      } // for (BasicBlock ...)
    } // for (Function ...)
  }

	// Search for pthread_join() calls and add the happens-before fact that the
	// thread must terminate before the join.
	//
	// This performs the following:
	// 1. Check if the instruction is a pthread_join call
	// 2. Attempt to find the pthread_create call where the thread ID is used
	// 3. If found, return a happens-before fact
	// Otherwise, return an emptry string
  //
  // Note: this makes a few assumptions
  // 1. The pthread_t associated with the pthread_join() was only used to
  //    create a thread in a single function
  // 2. The pthread_create() call associated with the passed join is in the
  //    same function
  //
  // Point 1 is not explicitly enforced
	void addJoinFacts(CallInst *i, z3::context &ctx, Z3_fixedpoint &zfp) {
    // Find the associated thread function 
    assert(i->getNumOperands() >= 1 && "join without operand");
    Function *thrdFunc = findCreateFromJoin(i->getOperand(0));
    if (thrdFunc == NULL) {
      errs() << "[WARNING] no create matching join: " << *i << '\n';
      return;
    }
    if (thrdFunc->empty()) {
      errs() << "[WARNING] empty thread function: " << thrdFunc->getName() 
             << '\n';
      return;
    }
    // Add happens-before edges from all the return sites of the function to
    // the join site
    std::vector<ReturnInst*> rets = getRets(thrdFunc);
    assert(rets.size() && "function without returns");
    for (ReturnInst *r : rets) {
      // The return must happen before the join
      addMHBFact(ctx, zfp, r, i);
    }
	}

  // Given a pthread_t argument used in a join call, search the def-use chain
  // to find where it is used in pthread_create. Return the thread function
  // create by the pthread_create call.
  //
  // Returns NULL if no function was found.
  //
  // Note: a program actually could use the same pthread_t in multiple
  // pthread_create() calls. This function returns the first one found
  Function* findCreateFromJoin(Value *pt) {
    assert(pt != NULL);
    Instruction *i = dyn_cast<Instruction>(pt);
    // This iteratively follows the defs of pt backwards until a
    // pthread_create() is found
    // This assumes the only things on the use-def chain are instructions
    if (i != NULL) {
      DEBUG_MSG("[DEBUG] Found pthread_t from instruction\n");
    }
    else {
      errs() << "[WARNING] Unhandled pthread_t def: " << *pt << '\n';
      return NULL;
    }
    // A vector of Values is used since, in general, the use-def chains can
    // contain any value and not just instructions
    std::vector<Value *> toVisit;
    toVisit.push_back(i);
    while (toVisit.size()) {
      Instruction *i = dyn_cast<Instruction>(toVisit.back());
      toVisit.pop_back();
      if (i == NULL) {
        errs() << "[ERROR] non instruction value on use-def chain: " 
               << *toVisit.back() << '\n';
        exit(EXIT_FAILURE);
      }
      // Check if the visited instruciton is a call to pthread_create. If it
      // is, assume that we are done.
      // Note: is there ever a case where the join argument is data dependent
      if (CallInst *ci = dyn_cast<CallInst>(i)) {
        if (ci->getCalledFunction()->getName() == "pthread_create") {
          DEBUG_MSG("paired join with create: " << *ci);
          return getPThreadThreadFunc(ci);
        }
      }
      else if (AllocaInst *ai = dyn_cast<AllocaInst>(i)) {
        // If we find an alloca, then we've found the site where the pthread_t
        // was allocated. Search forward from the alloca for the pthread_create
        // site.
        return forwardSearchCreate(ai);
      }
      else if (LoadInst *li = dyn_cast<LoadInst>(i)) {
        // Get the defs of the load instruction
        for (Use &U : li->operands()) {
          Value *useV = U.get();
          toVisit.push_back(useV);
        }
      }
      // TODO: Handle additional types here. Might be able to just blindly
      // search backwards on all the operands of the instruction
      else {
        errs() << "[ERROR] Unahandled instruction type while searching for "
               << "pthread_create: " << *i << '\n';
        DEBUG_MSG("checking isa loadinst\n");
        DEBUG_MSG("isa loadinst? " << isa<LoadInst>(i) << '\n');
        exit(EXIT_FAILURE);
      }
    }
    // Reach here means we didn't find the create site
    return NULL;
  }

  // Given an alloca instruction allocating a pthread_t, search forward for the
  // site where the pthread_t is being used. Return the function created by the
  // pthread_create call
  //
  // This returns the first site found
  Function *forwardSearchCreate(AllocaInst *ai) {
    std::vector<Instruction *> toVisit;
    toVisit.push_back(ai);
    while (toVisit.size()) {
      Instruction *cur = toVisit.back();
      toVisit.pop_back();
      for (User *u : cur->users()) {
        // Check if we've found pthread_create
        if (CallInst *ci = dyn_cast<CallInst>(u)) {
          if (ci->getCalledFunction()->getName() == "pthread_create") {
            DEBUG_MSG("found pthread_create on forward search: " 
                        << *ci << '\n');
            return getPThreadThreadFunc(ci);
          }
        }
        if (Instruction *i = dyn_cast<Instruction>(u)) {
          toVisit.push_back(i);
        }
        else {
          errs() << "[ERROR] unhandled User searching forward for "
                 << "pthread_create: " << *u << '\n';
          exit(EXIT_FAILURE);
        }
      }
    } // while (toVisit.size())
    // Reach here means we did not find pthread_create
    return NULL;
  }

  // Given a Function, return all of its return instructions
  std::vector<ReturnInst*> getRets(Function *f) {
    std::vector<ReturnInst*> ret;
    for (BasicBlock &BB : *f) {
      for (Instruction &I : BB) {
        if (ReturnInst *r = dyn_cast<ReturnInst>(&I)) {
          ret.push_back(r);
        }
      }
    }
    return ret;
  }

  // Starting from `main`, find all the children threads. Using the
  // happen-before relation caused by thread creation, get those stores which a
  // thread function must not be able to read from
  void getNotReadFrom(std::map<Function *, Function*> &child2parent
      , std::map<Function *, std::vector<Function*>> &parent2child
      , std::map<Function *, std::vector<StoreInst *>> &notReadFrom
      , std::map<Function *, CallInst *> &createLocs
      , Function *main) {
    // List containing function to be processed. The order the functions are
    // processed does matter! The parent of a thread must be processed before
    // the child. This is required because the parent's information needs to be
    // availible (the things the parent cannot read from) in order to evaluate
    // the child. 
    std::deque<Function *> worklist;
    worklist.push_back(main);
    while (worklist.size()) {
      // Get and delete the first item
      Function *curParent = worklist.front();
      DEBUG_MSG("[DEBUG] getNotReadFrom: analyzing function: " 
          << curParent->getName() << '\n');
      worklist.pop_front();

      // Get all the threads created by `f`. These are all the children of `f`
      std::vector<CallInst *> creats = getThreadCreate(curParent);
      DEBUG_MSG("\t" << creats.size() << " thread creations\n");

      std::vector<Function *> curParentChildren;

      for (CallInst *ci : creats) {
        Function *thrdFunc = getPThreadThreadFunc(ci);
        curParentChildren.push_back(thrdFunc);
        // This error could be fixed: see the comment above the assertion below
        assert(createLocs.find(thrdFunc) == createLocs.end() 
            && "thread function created twice");
        createLocs.emplace(thrdFunc, ci);
        // The child of curParent also needs to be processed (we need to find
        // if it has any children)
        worklist.push_back(thrdFunc);

        // In general, we should allow the same function to be created twice.
        // We could handle this by instead making the child2parent map a map
        // from a pair of Function * and CallInst:
        // std::map<std::pair<Function *, CallInst *>, Function*>
        // In this way, we only require each CallSite to be unique 
        assert(child2parent.find(thrdFunc) == child2parent.end() 
            && "function being created twice");
        child2parent.emplace(thrdFunc, curParent);

        // Get those store's within the thread that cannot be read by this call
        // instruction
        std::vector<StoreInst *> mnrl = getThreadLocalNotRead(ci);
        DEBUG_MSG("\t" << mnrl.size() << " local not-read from\n");
        // Get the stores which cannot be read by any ancestors of the parent
        std::vector<StoreInst *> mnrp 
          = getParentNotRead(ci->getParent()->getParent(), child2parent
              , notReadFrom);
        DEBUG_MSG("\t" << mnrp.size() << " parent not-read from\n");
        // Merge the results
        std::vector<StoreInst *> merge;
        merge.reserve(mnrl.size() + mnrp.size());
        merge.insert(merge.end(), mnrl.begin(), mnrl.end());
        merge.insert(merge.end(), mnrp.begin(), mnrp.end());
        // Update must-not read-from results
        assert(notReadFrom.find(thrdFunc) == notReadFrom.end()
            && "function being created twice");
        notReadFrom.emplace(thrdFunc, merge);
      } // for (CallInst *ci : creats)
      // save the parents children
      assert(parent2child.find(curParent) == parent2child.end()
          && "parent visited twice");
      parent2child.emplace(curParent, curParentChildren);
    }
  }

  // Given a call instruction `ci`, find those stores within the Function
  // calling `ci` which must happen-before `ci`. Any store which must happen
  // before `ci` is not visible to the child thread created by `ci`
  std::vector<StoreInst *> getThreadLocalNotRead(CallInst *ci) {
    BasicBlock *ciBB = ci->getParent();
    Function *ciF = ci->getParent()->getParent();
    // First, get the stores within the basicblock which happen before the
    // CallInst. These trivially must happen before the callinst
    std::vector<StoreInst *> ret = getStoresInBBBefore(ci);
    DEBUG_MSG("[DEBUG] getThreadLocalNotRead(): "
              "intra-BB happens-before stores: " << ret.size() << '\n');

    // Next, get all those stores in basicblock which post dominate `ci`. Since
    // we just got the stores within the same basicblock as `ci`, we can ignore
    // the block containing `ci` (any basicblock post-dominates itself).
    PostDominatorTree &PDT = getAnalysis<PostDominatorTree>(*ciF);
    SmallVector<BasicBlock *, 10> pdoms;
    PDT.getDescendants(ciBB, pdoms);
    for (BasicBlock *b : pdoms) {
      if (b == ciBB) {
        // Do not analyze the BasicBlock containing `ci`
        continue;
      }
      std::vector<StoreInst *> bStores = getStores(b);
      // Add bStores to the end of ret
      Utils::addVector(ret, bStores);
    }
    return ret;
  }

  // Given a pthread_create call, find all the stores within the calling thread
  // much must ocurr before the call. These are not visible to the child thread
  // unless the value is not overwritten. In that case, the value will be
  // present in the initial environment of the thread
  void addThreadLocalNotRead(z3::context &ctx, Z3_fixedpoint &zfp, CallInst *ci
      , Function *tfunc) {
    std::vector<StoreInst*> sts = getThreadLocalNotRead(ci);
    std::vector<Instruction*> ls = getLoads(tfunc);
    for (StoreInst *s : sts) {
      for (Instruction *l : ls) {
        addNotReadFact(ctx, zfp, l, s);
      }
    }
  }

  // Return the StoreInsts which ocurr before `i` within the same basicblock as
  // `i`.
  std::vector<StoreInst *> getStoresInBBBefore(Instruction *i) {
    BasicBlock *b = i->getParent();
    std::vector<StoreInst *> ret;
    for (auto bi = b->begin(), be = b->end(); bi != be; ++bi) {
      Instruction *cur = &*bi;
      if (cur == i) {
        return ret;
      }
      if (StoreInst *si = dyn_cast<StoreInst>(cur)) {
        ret.push_back(si);
      }
    }
    // Reaching this point means `i` was not found in its own basicblock. This
    // should never happen!
    assert(0 && "passed instruction not found");
  }

  // Given a function `f`, get all the stores which the parents of `f` cannot
  // read from. Transitively, `f` can also not read from these stores
  std::vector<StoreInst *> getParentNotRead(Function *f
      , std::map<Function*, Function*> child2parent
      , std::map<Function *, std::vector<StoreInst *>> notReadFrom) {
    std::vector<StoreInst *> ret;
    std::vector<Function *> parents = getParents(f, child2parent);
    for (Function *par : parents) {
      auto nri = notReadFrom.find(par);
      if (nri != notReadFrom.end()) {
        std::vector<StoreInst *> nr = nri->second;
        ret.insert(ret.end(), nr.begin(), nr.end());
      }
    }
    return ret;
  }

  // Given a child thread, follow the child2parent map and return all the
  // parents of `child` (i.e., its creater, its creater's creater, and so on)
  std::vector<Function *> getParents(Function *child
      , const std::map<Function *, Function*> child2parent) const {
    std::vector<Function *> ret;
    auto iter = child2parent.find(child);
    while (iter != child2parent.end()) {
      Function *parent = iter->second;
      ret.push_back(parent);
      // Find the parent of `parent`
      iter = child2parent.find(parent);
    }
    return ret;
  }

  // Return the first instruction in the passed function. If the function is
  // external (i.e., we do not have the source code, return NULL.
  Instruction *getFirstInst(Function *f) {
    if (f->begin() == f->end()) {
      // No basicblocks
      return NULL;
    }
    auto ii = f->begin()->begin();
    Instruction *ret = &*ii;
    return ret;
  }

  // Return the function being created in a thread by the passed call
  // instruction. It is assumed the call instruction is a pthread_create call.
  //
  // Otherwise, this will crash.
  Function *getPThreadThreadFunc(CallInst *ci) {
    Value *v = ci->getArgOperand(2);
    if (Function *tFunc = dyn_cast<Function>(v)) {
      return tFunc;
    }
    else {
      errs() << "[ERROR] pthread_create with non function 2nd arg\n";
      errs() << *v << '\n';
      exit(EXIT_FAILURE);
    }
  }

  std::set<BasicBlock *> getAllBBs(Function *F) {
    std::set<BasicBlock *> ret;
    for (BasicBlock &B : *F) {
      ret.insert(&B);
    }
    return ret;
  }

  // Update fixedpoint and context to contain not-reachable information
  void addNotReachableFacts(Module &M, z3::context &ctx, Z3_fixedpoint &zfp) {
    addNotReachRel(ctx, zfp);
    for (Function &F : M) {
      std::set<BasicBlock*> allBBs = getAllBBs(&F);
      std::map<BasicBlock *, std::set<BasicBlock*>> reachMap;
      reachMap = getReachability(&F);
      for (BasicBlock &B : F) {
        auto it = reachMap.find(&B);
        assert(it != reachMap.end());
        std::set<BasicBlock *> reach = it->second;
        if (reach.find(&B) != reach.end()) {
          DEBUG_MSG("Self reachable basicblock\n");
        }
        // At most there will be allBBs.size() items in the result
        std::vector<BasicBlock*> notReach(allBBs.size());
        // allBbs - reach
        // i.e., the not reachable BBs
        auto nrIt = set_difference(allBBs.begin()
                      , allBBs.end()
                      , reach.begin()
                      , reach.end()
                      , notReach.begin());
        // Resize notReach to remove any of the non-used slots
        notReach.resize(nrIt - notReach.begin());
        for (BasicBlock *notReachB : notReach) {
          // Every instruyction in B cannot reach notReachB
          for (Instruction &BI : B) {
            if (!isaStoreLoadCallOrTerm(&BI)) {
              continue;
            }
            for (Instruction &nrBI : *notReachB) {
              if (!isaStoreLoadCallOrTerm(&nrBI)) {
                continue;
              }
              addNotReachFact(ctx, zfp, &BI, &nrBI);
            }
          }
        }
      }
    }
  }

  // Return true if the passed instruction is a StoreInst, LoadInst, or
  // TerminatorInst
  bool isaStoreLoadCallOrTerm(Instruction *I) const {
    if (isa<StoreInst>(I)) {
      return true;
    }
    else if (isa<LoadInst>(I)) {
      return true;
    }
    else if (isa<TerminatorInst>(I)) {
      return true;
    }
    else if (isa<CallInst>(I)) {
      return true;
    }
    else {
      return false;
    }
  }

  std::map<BasicBlock *, std::set<BasicBlock*>> getReachability(Function *F) {
    std::map<BasicBlock *, std::set<BasicBlock*>> ret;
    for (BasicBlock &B : *F) {
      ret.emplace(&B, forwardDfs(&B));
    }
    return ret;
  }

  // Return all the basicblocks forwardly reachable from `b`
  //
  // This only includes `b`in the return if there exists  a path from `b` to
  // `b`.
  std::set<BasicBlock *> forwardDfs(BasicBlock *b) {
    std::set<BasicBlock*> ret;
    std::set<BasicBlock*> visited;
    std::vector<BasicBlock*> s;
    // Do not add `b` to ret unless there is a path from `b` to `b`.
    // To do this, initialize the stack with the successors 
    std::vector<BasicBlock*> bSuccs = getSuccsFromBB(b);
    s.insert(s.end(), bSuccs.begin(), bSuccs.end());
    while (!s.empty()) {
      BasicBlock *cur = s.back();
      s.pop_back();
      if (visited.find(cur) == visited.end()) {
        visited.insert(cur);
        if (cur == b) { 
          DEBUG_MSG("self-reachable basicblock: " << b->front() << '\n');
        }
        ret.insert(cur);
        std::vector<BasicBlock*> curSuccs = getSuccsFromBB(cur);
        s.insert(s.end(), curSuccs.begin(), curSuccs.end());
      }
    }
    if (ret.find(b) != ret.end()) {
      DEBUG_MSG("basicblock can reach itself\n");
    }
    return ret;
  }

  std::vector<BasicBlock*> getSuccsFromBB(BasicBlock *B) {
    std::vector<BasicBlock *> ret;
    TerminatorInst *ti = B->getTerminator();
    assert(ti && "malformed basic block");
    for (size_t i = 0; i < ti->getNumSuccessors(); ++i) {
      ret.push_back(ti->getSuccessor(i));
    }
    return ret;
  }

  // Return a const bitvector ID for the passed instruction.
  //
  // This uses the address of the instruction; LLVM guarantees this is unique.
  //
  // The size of the bitvector is the size of the pointer-type (e.g.,
  // sizeof(uintptr_t))
  z3::expr getValueBVID(z3::context &ctx, Value *v) const {
    return getValueBVIDSz(ctx, v, Z3_BV_SIZE);
  }

  // get function declaration of ld relation
  z3::func_decl getLdFuncDecl(z3::context &ctx) const {
    return getBVBVFuncDecl(ctx, "ld");
  }

  // get function declarion of st relation
  z3::func_decl getStFuncDecl(z3::context &ctx) {
    return getBVBVFuncDecl(ctx, "st");
  }

  z3::func_decl getMHBFuncDecl(z3::context &ctx) const {
    return getBVBVFuncDecl(ctx, "mhb");
  }

  z3::func_decl getNoReorderFuncDecl(z3::context &ctx) const {
    return getBVBVFuncDecl(ctx, "noreorder");
  }

  z3::func_decl getFenceFuncDecl(z3::context &ctx) const {
    return getBVFuncDecl(ctx, "fence");
  }

  z3::func_decl getLwSyncFuncDecl(z3::context &ctx) const {
    return getBVFuncDecl(ctx, "lwsync");
  }

  // Return func_decl of dominance relation
  z3::func_decl getDomFuncDecl(z3::context &ctx) const {
    return getBVBVFuncDecl(ctx, "dom");
  }

  // Return func_decl of post dominance relation
  z3::func_decl getPostDomFuncDecl(z3::context &ctx) const {
    return getBVBVFuncDecl(ctx, "postDom");
  }

  // Return func_decl of priority 
  z3::func_decl getPriFuncDecl(z3::context &ctx) const {
    return getBVFuncDecl(ctx, "pri");
  }

  // Return func_decl of higher 
  z3::func_decl getHighFuncDecl(z3::context &ctx) const {
    return getBVBVFuncDecl(ctx, "higher");
  }

  // Return func_decl of lineHigh
  z3::func_decl getLineHighFuncDecl(z3::context &ctx) const {
    return getBVBVFuncDecl(ctx, "linehigh");
  }

  // return func_decl of compLoad
  z3::func_decl getCompLdFuncDecl(z3::context &ctx) const {
    return getBVFuncDecl(ctx, "compLd");
  }

  // return func_decl of nonLastStore
  z3::func_decl getNonLastStFuncDecl(z3::context &ctx) const {
    return getBVFuncDecl(ctx, "nonLastSt");
  }

  // Return func_decl of linePri
  z3::func_decl getLinePriFuncDecl(z3::context &ctx) const {
    return getBVBVFuncDecl(ctx, "linepri");
  }

  z3::func_decl getNotReachFuncDecl(z3::context &ctx) const {
    return getBVBVFuncDecl(ctx, "not-reach");
  }

  z3::func_decl getNotReadsFromFuncDecl(z3::context &ctx) const {
    return getBVBVFuncDecl(ctx, "not-rf");
  }

  z3::func_decl getReadsFromFuncDecl(z3::context &ctx) const {
    return getBVBVFuncDecl(ctx, "rf");
  }

  // Given the name of a relation with arguments (bitvector, bitvector) return
  // its function declaration.
  z3::func_decl getBVBVFuncDecl(z3::context &ctx, const char *name) const {
    return z3_fp_helpers::getBVBVFuncDecl(ctx, name, Z3_BV_SIZE);
  }

  // Given the name of a relation with arguments (bitvector) return
  // its function declaration.
  z3::func_decl getBVFuncDecl(z3::context &ctx, const char *name) const {
    return z3_fp_helpers::getBVFuncDecl(ctx, name, Z3_BV_SIZE);
  }

  void addLinePriFact(z3::context &ctx
              , Z3_fixedpoint &zfp
              , z3::func_decl fd
              , llvm::Instruction *l
              , unsigned priority) const {
    addLinePriFactSz(ctx, zfp, fd, l, priority, Z3_BV_SIZE);
  }

  void addFact2(z3::context &ctx
              , Z3_fixedpoint &zfp
              , z3::func_decl fd
              , llvm::Value *v1
              , llvm::Value *v2) const {
    addFact2Sz(ctx, zfp, fd, v1, v2, Z3_BV_SIZE);
  }
  
  void addFact1(z3::context &ctx
              , Z3_fixedpoint &zfp
              , z3::func_decl fd
              , llvm::Value *v) const {
    addFact1Sz(ctx, zfp, fd, v, Z3_BV_SIZE);
  }

  // Add a fact that `l` loads `v`
  void addLoadFact(z3::context &ctx
              , Z3_fixedpoint &zfp
              , Instruction *l
              , Value *v) {
    addFact2(ctx, zfp, getLdFuncDecl(ctx), l, v);
  }


  // Add a fact that `f` is a fence
  void addFenceFact(z3::context &ctx
              , Z3_fixedpoint &zfp
              , Instruction *f) {
    addFact1(ctx, zfp, getFenceFuncDecl(ctx), f);
  }

  void addLwSyncFact(z3::context &ctx
                     , Z3_fixedpoint &zfp
                     , Instruction *f) {
    addFact1(ctx, zfp, getLwSyncFuncDecl(ctx), f);
  }

  // Add (rf l v)
  void addReadsFromFact(z3::context &ctx
              , Z3_fixedpoint &zfp
              , Instruction *l
              , Value *v) const {
    assert(Utils::isSomeLoad(l) && "non load reading");
    addFact2(ctx, zfp, getReadsFromFuncDecl(ctx), l, v);
  }

  // Add a fact that `l` stores `v`
  void addStoreFact(z3::context &ctx
              , Z3_fixedpoint &zfp
              , Instruction *s
              , Value *v) {
    addFact2(ctx, zfp, getStFuncDecl(ctx), s, v);
  }

  // Add (mhb to from)
  void addMHBFact(z3::context &ctx, Z3_fixedpoint &zfp, Instruction *from
                  , Instruction *to) {
    //Z3_ast fromId = getValueBVID(ctx, from);
    //Z3_ast toId = getValueBVID(ctx, to);
    //addMHBFact(ctx, zfp, fromId, toId);
    addFact2(ctx, zfp, getMHBFuncDecl(ctx), from, to);
  }
  
  // Add fact: (dom a b)
  // Converts to instructions to bitvector ids
  void addDomFact(z3::context &ctx
                  , Z3_fixedpoint &zfp
                  , Instruction *a
                  , Instruction *b) {
    addFact2(ctx, zfp, getDomFuncDecl(ctx), a, b);
  }

  // Add fact: (dom a b)
  // Converts to instructions to bitvector ids
  void addPostDomFact(z3::context &ctx
                  , Z3_fixedpoint &zfp
                  , Instruction *a
                  , Instruction *b) {
    addFact2(ctx, zfp, getPostDomFuncDecl(ctx), a, b);
  }

  // Add fact: (not-read s l)
  void addNotReadFact(z3::context &ctx
                     , Z3_fixedpoint &zfp
                     , Instruction *s
                     , Instruction *l) {
    addFact2(ctx, zfp, getNotReadsFromFuncDecl(ctx), s, l);
  }

  // Add: (not-reach a b): a cannot reach b
  void addNotReachFact(z3::context &ctx
                  , Z3_fixedpoint &zfp
                  , Instruction *a
                  , Instruction *b) {
    addFact2(ctx, zfp, getNotReachFuncDecl(ctx), a, b);
  }

  // Return the dominance and post dominance facts
  // within the basicblock (i.e., the order of instructions in the block).
  // Since we are only interested in loads and stores, this will ignore all
  // other instruction types except the basicblock terminator and calls.
  void addInternalDomAndPostDomFacts(BasicBlock *b, z3::context &ctx, Z3_fixedpoint &zfp) {
      std::vector<Instruction *> loadsNStores = getLoadStoreCallOrdered(b);
      loadsNStores.push_back(b->getTerminator());
      assert(loadsNStores.size() > 0 && "basicblock with no terminator");
      if (loadsNStores.size() == 1) {
          // With only one instruction (the terminator) there is no internal
          // dominance facts
          return;
      }
      assert(loadsNStores.size() > 1);
      Instruction *prev = loadsNStores[0];
      for (size_t i = 1; i < loadsNStores.size(); ++i) {
          Instruction *cur = loadsNStores[i];
          // prev dominates cur
          addDomFact(ctx, zfp, prev, cur);
          addPostDomFact(ctx, zfp, cur, prev);
          // Update prev for the next iteration
          prev = cur;
      }
  }

  // (=> (and (dom a b) (not-reach b a)) (mhb a b))
  void addDomNotReachRule(z3::context &ctx, Z3_fixedpoint &zfp) {
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    //Z3_ast a = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "aDom"), bvSort);
    //Z3_ast b = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "bDom"), bvSort);
    z3::expr a = z3::expr(ctx, Z3_mk_bound(ctx, 0, bvSort));
    z3::expr b = z3::expr(ctx, Z3_mk_bound(ctx, 1, bvSort));

    z3::func_decl domDecl = getDomFuncDecl(ctx);
    z3::func_decl nrDecl = getNotReachFuncDecl(ctx);
    z3::func_decl mhbDecl = getMHBFuncDecl(ctx);

    // (dom a b)
    // (mhb a b)
    z3::expr args[2] = {a, b};
    z3::expr domAB = domDecl(2, args);
    z3::expr mhbAB = mhbDecl(2, args);

    // (not-reach b a)
    args[0] = b;
    args[1] = a;
    z3::expr nrBA = nrDecl(2, args);

    // (and (dom a b) (not-reach b a))
    //args[0] = domAB;
    //args[1] = nrBA;
    z3::expr domNrAnd = domAB && nrBA;

    z3::expr imp = z3::implies(domNrAnd, mhbAB);
    Z3_fixedpoint_add_rule(ctx, zfp, imp, NULL);
  }

  // (=> (and (dom a b) (not-reach b a)) (mhb a b))
  void addDomNotReachReorderRule(z3::context &ctx, Z3_fixedpoint &zfp) {
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    //Z3_ast a = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "aDom"), bvSort);
    //Z3_ast b = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "bDom"), bvSort);
    z3::expr a = z3::expr(ctx, Z3_mk_bound(ctx, 0, bvSort));
    z3::expr b = z3::expr(ctx, Z3_mk_bound(ctx, 1, bvSort));

    z3::func_decl domDecl = getDomFuncDecl(ctx);
    z3::func_decl nrDecl = getNotReachFuncDecl(ctx);
    z3::func_decl mhbDecl = getMHBFuncDecl(ctx);
    z3::func_decl noReoDecl = getNoReorderFuncDecl(ctx);

    // (dom a b)
    // (mhb a b)
    // (noreorder a b)
    z3::expr args[2] = {a, b};
    z3::expr domAB = domDecl(2, args);
    z3::expr mhbAB = mhbDecl(2, args);
    z3::expr noReoAB = noReoDecl(2, args);

    // (not-reach b a)
    args[0] = b;
    args[1] = a;
    z3::expr nrBA = nrDecl(2, args);

    // (and (dom a b) (not-reach b a) (noreorder a b))
    //args[0] = domAB;
    //args[1] = nrBA;
    z3::expr domNrAnd = domAB && nrBA && noReoAB;

    z3::expr imp = z3::implies(domNrAnd, mhbAB);
    Z3_fixedpoint_add_rule(ctx, zfp, imp, NULL);
  }

  // (=> (and (reads-from l s1)
  //       (mhb s1 s2)
  //       (is-load l v)
  //       (is-store s1 v)
  //       (is-store s2 v))
  //     (mhb l s2))
  void addReadsFromMHB(z3::context &ctx, Z3_fixedpoint &zfp) {
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    //Z3_ast s1 = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "s1"), bvSort);
    //Z3_ast s2 = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "s2"), bvSort);
    //Z3_ast l = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "l"), bvSort);
    //Z3_ast v = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "v"), bvSort);
    z3::expr s1 = z3::expr(ctx, Z3_mk_bound(ctx, 0, bvSort));
    z3::expr s2 = z3::expr(ctx, Z3_mk_bound(ctx, 1, bvSort));
    z3::expr l  = z3::expr(ctx, Z3_mk_bound(ctx, 2, bvSort));
    z3::expr v  = z3::expr(ctx, Z3_mk_bound(ctx, 3, bvSort));


    z3::func_decl mhbDecl = getMHBFuncDecl(ctx);
    z3::func_decl rfDecl = getReadsFromFuncDecl(ctx);
    z3::func_decl ldDecl = getLdFuncDecl(ctx);
    z3::func_decl stDecl = getStFuncDecl(ctx);

    z3::expr args[2] = {l, s1};
    z3::expr rf_l_s1 = rfDecl(2, args);

    args[0] = s1;
    args[1] = s2;
    z3::expr mhb_s1_s2 = mhbDecl(2, args);

    args[0] = l;
    args[1] = v;
    z3::expr ld_l_v = ldDecl(2, args);

    args[0] = s1;
    args[1] = v;
    z3::expr st_s1_v = stDecl(2, args);

    args[0] = s2;
    args[1] = v;
    z3::expr st_s2_v = stDecl(2, args);

    args[0] = l;
    args[1] = s2;
    z3::expr mhb_l_s2 = mhbDecl(2, args);

    //z3::expr andArgs[5] = {rf_l_s1, mhb_s1_s2, ld_l_v, st_s1_v, st_s2_v};
    //z3::expr bigAnd = z3::expr(ctx, Z3_mk_and(ctx, 5, andArgs));
    z3::expr bigAnd = rf_l_s1 && mhb_s1_s2 && ld_l_v && st_s1_v && st_s2_v;

    z3::expr imp = z3::implies(bigAnd, mhb_l_s2);
    Z3_fixedpoint_add_rule(ctx, zfp, imp, NULL);
  }

  // (=> (and (mhb a b) (mhb b c)) 
  //     (mhb a c))
  void addMHBTrans(z3::context &ctx, Z3_fixedpoint &zfp) {
    addTransRuleBVBV(ctx, zfp, getMHBFuncDecl(ctx), Z3_BV_SIZE);
  }

  // (=> (and (reads-from l1 s1)
  //          (mhb l1 s2)
  //          (mhb s2 l2)
  //          (is-load l1 v)
  //          (is-load l2 v)
  //          (is-store s2 v))
  //     (not-rf l2 s1))
  void addOverwriteNotReads(z3::context &ctx, Z3_fixedpoint &zfp) {
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    //Z3_ast l1 = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "l1"), bvSort);
    //Z3_ast l2 = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "l2"), bvSort);
    //Z3_ast s1 = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "s1"), bvSort);
    //Z3_ast s2 = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "s2"), bvSort);
    z3::expr l1 = z3::expr(ctx, Z3_mk_bound(ctx, 0, bvSort));
    z3::expr l2 = z3::expr(ctx, Z3_mk_bound(ctx, 1, bvSort));
    z3::expr s1 = z3::expr(ctx, Z3_mk_bound(ctx, 2, bvSort));
    z3::expr s2 = z3::expr(ctx, Z3_mk_bound(ctx, 3, bvSort));
    z3::expr v  = z3::expr(ctx, Z3_mk_bound(ctx, 4, bvSort));

    z3::func_decl rfDecl = getReadsFromFuncDecl(ctx);
    z3::func_decl ldDecl = getLdFuncDecl(ctx);
    z3::func_decl stDecl = getStFuncDecl(ctx);
    z3::func_decl mhbDecl = getMHBFuncDecl(ctx);
    z3::func_decl nrfDecl = getNotReadsFromFuncDecl(ctx);

    z3::expr args[2] = {l1, s1};
    z3::expr rf_l1_s1 = rfDecl(2, args);

    args[0] = l1;
    args[1] = s2;
    z3::expr mhb_l1_s2 = mhbDecl(2, args);

    args[0] = s2;
    args[1] = l2;
    z3::expr mhb_s2_l2 = mhbDecl(2, args);

    args[0] = l1;
    args[1] = v;
    z3::expr ld_l1_v = ldDecl(2, args);

    args[0] = l2;
    args[1] = v;
    z3::expr ld_l2_v = ldDecl(2, args);

    args[0] = s2;
    args[1] = v;
    z3::expr st_s2_v = stDecl(2, args);

    args[0] = l2;
    args[1] = s1;
    z3::expr nrf_l2_s1 = nrfDecl(2, args);

    //z3::expr andArgs[6] = {rf_l1_s1, mhb_l1_s2, mhb_s2_l2, ld_l1_v, ld_l2_v
    //                    , st_s2_v};
    //z3::expr bigAnd = z3::expr(ctx, Z3_mk_and(ctx, 6, andArgs));
    z3::expr bigAnd = rf_l1_s1 && mhb_l1_s2 && mhb_s2_l2 && ld_l1_v 
                      && ld_l2_v && st_s2_v;

    z3::expr imp = z3::implies(bigAnd, nrf_l2_s1);
    Z3_fixedpoint_add_rule(ctx, zfp, imp, NULL);
  }

  // (=> (mhb a b) (not-rf a b))
  void addMHBNotReads(z3::context &ctx, Z3_fixedpoint &zfp) {
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    //Z3_ast a = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "a"), bvSort);
    //Z3_ast b = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "b"), bvSort);
    z3::expr a = z3::expr(ctx, Z3_mk_bound(ctx, 0, bvSort));
    z3::expr b = z3::expr(ctx, Z3_mk_bound(ctx, 1, bvSort));

    z3::expr args[2] = {a, b};
    z3::expr mhbAB = getMHBFuncDecl(ctx)(2, args);
    z3::expr nrfAB = getNotReadsFromFuncDecl(ctx)(2, args);

    Z3_fixedpoint_add_rule(ctx, zfp, Z3_mk_implies(ctx, mhbAB, nrfAB), NULL);
  }

  //// Add a rule sating that reads-from(a, b) ==> MHB(b, a). A write must ocurr
  //// before a read for it to be witnessed
  void addMHBNRFRule(z3::context &ctx, Z3_fixedpoint &zfp) const {
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    z3::expr a = z3::expr(ctx, Z3_mk_bound(ctx, 0, bvSort));
    z3::expr b = z3::expr(ctx, Z3_mk_bound(ctx, 1, bvSort));

    z3::expr args[2] = {a, b};
    z3::expr rfAB = getReadsFromFuncDecl(ctx)(2, args);
    args[0] = b;
    args[1] = a;
    z3::expr mhbBA = getMHBFuncDecl(ctx)(2, args);
    Z3_fixedpoint_add_rule(ctx, zfp, Z3_mk_implies(ctx, rfAB, mhbBA), NULL);
  }

  // Add rules which can prune interferences:
  void addPruningRules(z3::context &ctx, Z3_fixedpoint &zfp) {
    addReadsFromRel(ctx, zfp);
    addNotReadsFromRel(ctx, zfp);

    addMHBNRFRule(ctx, zfp);
    addDomNotReachRule(ctx, zfp);
    addReadsFromMHB(ctx, zfp);
    addMHBTrans(ctx, zfp);
    addMHBNotReads(ctx, zfp);
    addOverwriteNotReads(ctx, zfp);
  }

  void addSCReorderRules(z3::context &ctx, Z3_fixedpoint &zfp) {
    addNoReorderRel(ctx, zfp);
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    z3::expr v1 = z3::expr(ctx, Z3_mk_bound(ctx, 2, bvSort));
    z3::expr v2 = z3::expr(ctx, Z3_mk_bound(ctx, 2, bvSort));
    z3::func_decl ldDecl = getLdFuncDecl(ctx);
    z3::func_decl stDecl = getStFuncDecl(ctx);
    z3::func_decl fenceDecl = getFenceFuncDecl(ctx);

    /* Single Variable Rules */

    // R(v) -> R(v)
    addNoReoFunc2Func2(ctx, zfp, ldDecl, ldDecl, v1, v1);

    // R(v) -> W(v)
    addNoReoFunc2Func2(ctx, zfp, ldDecl, stDecl, v1, v1);

    // W(v) -> R(v)
    addNoReoFunc2Func2(ctx, zfp, stDecl, ldDecl, v1, v1);

    // W(v) -> W(v)
    addNoReoFunc2Func2(ctx, zfp, stDecl, stDecl, v1, v1);

    /* End Single Variable Rules */

    /* Double Variables Rules */
    // R(v) -> R(v)
    addNoReoFunc2Func2(ctx, zfp, ldDecl, ldDecl, v1, v2);

    //// R(v) -> W(v)
    addNoReoFunc2Func2(ctx, zfp, ldDecl, stDecl, v1, v2);

    //// W(v) -> R(v)
    addNoReoFunc2Func2(ctx, zfp, stDecl, ldDecl, v1, v2);

    //// W(v) -> W(v)
    addNoReoFunc2Func2(ctx, zfp, stDecl, stDecl, v1, v2);

    /* End Double Variables Rules */

    // Fences dont exist on SC
    // Fences and all loads/stores are not reordered
    //addNoReoFunc1Func2(ctx, zfp, fenceDecl, stDecl);
    //addNoReoFunc1Func2(ctx, zfp, fenceDecl, ldDecl);

    //addNoReoFunc2Func1(ctx, zfp, stDecl, fenceDecl);
    //addNoReoFunc2Func1(ctx, zfp, ldDecl, fenceDecl);
  }

  void addTSOReorderRules(z3::context &ctx, Z3_fixedpoint &zfp) {
    addNoReorderRel(ctx, zfp);
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    z3::expr v1 = z3::expr(ctx, Z3_mk_bound(ctx, 2, bvSort));
    z3::expr v2 = z3::expr(ctx, Z3_mk_bound(ctx, 2, bvSort));
    z3::func_decl ldDecl = getLdFuncDecl(ctx);
    z3::func_decl stDecl = getStFuncDecl(ctx);
    z3::func_decl fenceDecl = getFenceFuncDecl(ctx);

    /////// OLD DEFINITIONS

    //// Loads can never be reordered with anything subsequent
    //addNoReoFunc2Func2(ctx, zfp, ldDecl, stDecl, v1, v2);
    //addNoReoFunc2Func2(ctx, zfp, ldDecl, ldDecl, v1, v2);

    ////// Nothing can ever drift past a store
    //addNoReoFunc2Func2(ctx, zfp, ldDecl, stDecl, v1, v2);
    ////addNoReoFunc2Func2(ctx, zfp, stDecl, stDecl, v1, v2);

    ////// Store--Load to the same variable cannot be re-ordered
    //addNoReoFunc2Func2(ctx, zfp, stDecl, ldDecl, v1, v1);

    ////// Store--Store to any variable cannot be reordered
    //////////////addNoReoFunc2Func2(ctx, zfp, stDecl, stDecl, v1, v2);

    //// Fences and all loads/stores are not reordered
    //addNoReoFunc1Func2(ctx, zfp, fenceDecl, stDecl);
    //addNoReoFunc1Func2(ctx, zfp, fenceDecl, ldDecl);

    //addNoReoFunc2Func1(ctx, zfp, stDecl, fenceDecl);
    //addNoReoFunc2Func1(ctx, zfp, ldDecl, fenceDecl);
    /////// END OLD DEFINITIONS

    /* Single Variable Rules */

    // R(v) -> R(v)
    addNoReoFunc2Func2(ctx, zfp, ldDecl, ldDecl, v1, v1);

    // R(v) -> W(v)
    addNoReoFunc2Func2(ctx, zfp, ldDecl, stDecl, v1, v1);

    // W(v) -> R(v)
    // This rule is not enforced because if it is it prevents store buffer
    // forwarding
    //addNoReoFunc2Func2(ctx, zfp, stDecl, ldDecl, v1, v1);

    // W(v) -> W(v)
    addNoReoFunc2Func2(ctx, zfp, stDecl, stDecl, v1, v1);

    /* End Single Variable Rules */

    /* Double Variables Rules */
    // RMO/PowerPC allow all of these

    // R(v) -> R(v)
    addNoReoFunc2Func2(ctx, zfp, ldDecl, ldDecl, v1, v2);

    //// R(v) -> W(v)
    addNoReoFunc2Func2(ctx, zfp, ldDecl, stDecl, v1, v2);

    //// W(v) -> R(v)
    //addNoReoFunc2Func2(ctx, zfp, stDecl, ldDecl, v1, v2);

    //// W(v) -> W(v)
    addNoReoFunc2Func2(ctx, zfp, stDecl, stDecl, v1, v2);

    /* End Double Variables Rules */

    addFenceReorderRules(ctx, zfp);

    // Fences and all loads/stores are not reordered
    //addNoReoFunc1Func2(ctx, zfp, fenceDecl, stDecl);
    //addNoReoFunc1Func2(ctx, zfp, fenceDecl, ldDecl);

    //addNoReoFunc2Func1(ctx, zfp, stDecl, fenceDecl);
    //addNoReoFunc2Func1(ctx, zfp, ldDecl, fenceDecl);
  }

  void addRMOReorderRules(z3::context &ctx, Z3_fixedpoint &zfp) {
    addNoReorderRel(ctx, zfp);
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    z3::expr v1 = z3::expr(ctx, Z3_mk_bound(ctx, 2, bvSort));
    z3::expr v2 = z3::expr(ctx, Z3_mk_bound(ctx, 2, bvSort));
    z3::func_decl ldDecl = getLdFuncDecl(ctx);
    z3::func_decl stDecl = getStFuncDecl(ctx);

    /* Single Variable Rules */

    // R(v) -> R(v)
    addNoReoFunc2Func2(ctx, zfp, ldDecl, ldDecl, v1, v1);

    // R(v) -> W(v)
    addNoReoFunc2Func2(ctx, zfp, ldDecl, stDecl, v1, v1);

    // W(v) -> R(v)
    //addNoReoFunc2Func2(ctx, zfp, stDecl, ldDecl, v1, v1);

    // W(v) -> W(v)
    addNoReoFunc2Func2(ctx, zfp, stDecl, stDecl, v1, v1);

    /* End Single Variable Rules */

    /* Double Variables Rules */
    // RMO/PowerPC allow all of these

    // R(v) -> R(v)
    //addNoReoFunc2Func2(ctx, zfp, ldDecl, ldDecl, v1, v2);

    //// R(v) -> W(v)
    //addNoReoFunc2Func2(ctx, zfp, ldDecl, stDecl, v1, v2);

    //// W(v) -> R(v)
    //addNoReoFunc2Func2(ctx, zfp, stDecl, ldDecl, v1, v2);

    //// W(v) -> W(v)
    //addNoReoFunc2Func2(ctx, zfp, stDecl, stDecl, v1, v2);

    /* End Double Variables Rules */

    addFenceReorderRules(ctx, zfp);

    //z3::func_decl lwDecl = getLwSyncFuncDecl(ctx);
    //addNoReoFunc1Func2(ctx, zfp, lwDecl, stDecl);
    //addNoReoFunc1Func2(ctx, zfp, lwDecl, ldDecl);

    //addNoReoFunc2Func1(ctx, zfp, stDecl, lwDecl);
    //addNoReoFunc2Func1(ctx, zfp, ldDecl, lwDecl);

    // Fences and all loads/stores are not reordered
    //addNoReoFunc1Func2(ctx, zfp, fenceDecl, stDecl);
    //addNoReoFunc1Func2(ctx, zfp, fenceDecl, ldDecl);

    //addNoReoFunc2Func1(ctx, zfp, stDecl, fenceDecl);
    //addNoReoFunc2Func1(ctx, zfp, ldDecl, fenceDecl);
  }

  // Common reordering rules involving fences.
  void addFenceReorderRules(z3::context &ctx, Z3_fixedpoint &zfp) {
    z3::func_decl ldDecl = getLdFuncDecl(ctx);
    z3::func_decl stDecl = getStFuncDecl(ctx);
    z3::func_decl fenceDecl = getFenceFuncDecl(ctx);
    //z3::func_decl lwDecl = getLwSyncFuncDecl(ctx);

    addNoReoFunc1Func2(ctx, zfp, fenceDecl, stDecl);
    addNoReoFunc1Func2(ctx, zfp, fenceDecl, ldDecl);

    addNoReoFunc2Func1(ctx, zfp, stDecl, fenceDecl);
    addNoReoFunc2Func1(ctx, zfp, ldDecl, fenceDecl);

    // lwsync preserves everything but write-lwsync--read order
    //addNoReoFunc1Func2(ctx, zfp, lwDecl, stDecl);
    //addNoReoFunc1Func2(ctx, zfp, lwDecl, ldDecl);

    //addNoReoFunc2Func1(ctx, zfp, stDecl, lwDecl);
    //addNoReoFunc2Func1(ctx, zfp, ldDecl, lwDecl);
  }

  void addNoReorderRules(z3::context &ctx, Z3_fixedpoint &zfp) {
    addReadsFromRel(ctx, zfp);
    addNotReadsFromRel(ctx, zfp);
    addDomNotReachReorderRule(ctx, zfp);

    addMHBNRFRule(ctx, zfp);
    addReadsFromMHB(ctx, zfp);
    addMHBTrans(ctx, zfp);
    addMHBNotReads(ctx, zfp);
    addOverwriteNotReads(ctx, zfp);
  }

  void addPSOReorderRules(z3::context &ctx, Z3_fixedpoint &zfp) {
    addNoReorderRel(ctx, zfp);
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    z3::expr v1 = z3::expr(ctx, Z3_mk_bound(ctx, 2, bvSort));
    z3::expr v2 = z3::expr(ctx, Z3_mk_bound(ctx, 2, bvSort));
    z3::func_decl ldDecl = getLdFuncDecl(ctx);
    z3::func_decl stDecl = getStFuncDecl(ctx);
    //z3::func_decl fenceDecl = getFenceFuncDecl(ctx);

    /////// OLD DEFINITIONS
    //
    //// Loads can never be reordered with anything subsequent
    //addNoReoFunc2Func2(ctx, zfp, ldDecl, stDecl, v1, v2);
    //addNoReoFunc2Func2(ctx, zfp, ldDecl, ldDecl, v1, v2);

    //// Store--Load to the same variable cannot be re-ordered
    //addNoReoFunc2Func2(ctx, zfp, stDecl, ldDecl, v1, v1);

    //// Store--Store to the same variable cannot be reordered
    ////addNoReoFunc2Func2(ctx, zfp, stDecl, stDecl, v1, v1);

    //// Fences and all loads/stores are not reordered
    //addNoReoFunc1Func2(ctx, zfp, fenceDecl, stDecl);
    //addNoReoFunc1Func2(ctx, zfp, fenceDecl, ldDecl);

    //addNoReoFunc2Func1(ctx, zfp, stDecl, fenceDecl);
    //addNoReoFunc2Func1(ctx, zfp, ldDecl, fenceDecl);


    /* Single Variable Rules */

    // R(v) -> R(v)
    addNoReoFunc2Func2(ctx, zfp, ldDecl, ldDecl, v1, v1);

    // R(v) -> W(v)
    addNoReoFunc2Func2(ctx, zfp, ldDecl, stDecl, v1, v1);

    // W(v) -> R(v)
    // This rule is not enforced because if it is it prevents store buffer
    // forwarding
    //addNoReoFunc2Func2(ctx, zfp, stDecl, ldDecl, v1, v1);

    // W(v) -> W(v)
    addNoReoFunc2Func2(ctx, zfp, stDecl, stDecl, v1, v1);

    /* End Single Variable Rules */

    /* Double Variables Rules */
    // RMO/PowerPC allow all of these

    // R(v) -> R(v)
    addNoReoFunc2Func2(ctx, zfp, ldDecl, ldDecl, v1, v2);

    //// R(v) -> W(v)
    addNoReoFunc2Func2(ctx, zfp, ldDecl, stDecl, v1, v2);

    //// W(v) -> R(v)
    //addNoReoFunc2Func2(ctx, zfp, stDecl, ldDecl, v1, v2);

    //// W(v) -> W(v)
    //addNoReoFunc2Func2(ctx, zfp, stDecl, stDecl, v1, v2);

    /* End Double Variables Rules */

    addFenceReorderRules(ctx, zfp);

    // Fences and all loads/stores are not reordered
    //addNoReoFunc1Func2(ctx, zfp, fenceDecl, stDecl);
    //addNoReoFunc1Func2(ctx, zfp, fenceDecl, ldDecl);

    //addNoReoFunc2Func1(ctx, zfp, stDecl, fenceDecl);
    //addNoReoFunc2Func1(ctx, zfp, ldDecl, fenceDecl);
  }

  void addTSOPruningRules(z3::context &ctx, Z3_fixedpoint &zfp) {
    addReadsFromRel(ctx, zfp);
    addNotReadsFromRel(ctx, zfp);

    // General Rules
    addMHBNRFRule(ctx, zfp);
    addReadsFromMHB(ctx, zfp);
    addMHBTrans(ctx, zfp);
    addMHBNotReads(ctx, zfp);
    addOverwriteNotReads(ctx, zfp);

    // MHB Rules
    addDomStoreStoreTSO(ctx, zfp);
    addLoadStoreMHB(ctx, zfp);
    addStoreLoadMHB(ctx, zfp);
    addLoadLoadMHB(ctx, zfp);

    // Fence Rules
    addPOLoadStoreFence(ctx, zfp);
  }

  void addPSOPruningRules(z3::context &ctx, Z3_fixedpoint &zfp) {
    addReadsFromRel(ctx, zfp);
    addNotReadsFromRel(ctx, zfp);

    // General Rules
    addMHBNRFRule(ctx, zfp);
    addReadsFromMHB(ctx, zfp);
    addMHBTrans(ctx, zfp);
    addMHBNotReads(ctx, zfp);
    addOverwriteNotReads(ctx, zfp);

    // MHB Rules
    addDomStoreStorePSO(ctx, zfp);
    addLoadStoreMHB(ctx, zfp);
    addStoreLoadMHB(ctx, zfp);
    addLoadLoadMHB(ctx, zfp);

    // Fence Rules
    addPOLoadStoreFence(ctx, zfp);
  }

  // Add ordering of loads/stores before/after a fence
  void addPOLoadStoreFence(z3::context &ctx, Z3_fixedpoint &zfp) {
    addDomFenceFunc2(ctx, zfp, getLdFuncDecl(ctx));
    addDomFenceFunc2(ctx, zfp, getStFuncDecl(ctx));
    addDomFunc2Fence(ctx, zfp, getLdFuncDecl(ctx));
    addDomFunc2Fence(ctx, zfp, getStFuncDecl(ctx));
  }

  // Add:
  // (=> (and (dom a b) 
  //          (not-reach b a)
  //          (is-fence a)
  //          (fd b v) )
  //     (mhb a b))
  //
  // where fd is the passed func_decl
  void addDomFenceFunc2(z3::context &ctx, Z3_fixedpoint &zfp
      , const z3::func_decl fd) {
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    z3::expr a = z3::expr(ctx, Z3_mk_bound(ctx, 0, bvSort));
    z3::expr b = z3::expr(ctx, Z3_mk_bound(ctx, 1, bvSort));
    z3::expr v = z3::expr(ctx, Z3_mk_bound(ctx, 2, bvSort));

    z3::func_decl domDecl = getDomFuncDecl(ctx);
    z3::func_decl nrDecl = getNotReachFuncDecl(ctx);
    z3::func_decl mhbDecl = getMHBFuncDecl(ctx);
    z3::func_decl fenceDecl = getFenceFuncDecl(ctx);

    // (dom a b)
    // (mhb a b)
    z3::expr args[2] = {a, b};
    z3::expr domAB = domDecl(2, args);
    z3::expr mhbAB = mhbDecl(2, args);

    // (not-reach b a)
    args[0] = b;
    args[1] = a;
    z3::expr nrBA = nrDecl(2, args);

    // (is-fence a)
    args[0] = a;
    z3::expr fenceA = fenceDecl(1, args);

    // (fd b v)
    args[0] = b;
    args[1] = v;
    z3::expr fdBV = fd(2, args);

    z3::expr bigAnd = domAB && nrBA && fenceA && fdBV;

    z3::expr imp = z3::implies(bigAnd, mhbAB);
    Z3_fixedpoint_add_rule(ctx, zfp, imp, NULL);
  }

  // Add:
  // (=> (and (dom a b) 
  //          (not-reach b a)
  //          (fd a v)
  //          (is-fence b))
  //     (mhb a b))
  //
  // where fd is the passed func_decl
  void addDomFunc2Fence(z3::context &ctx, Z3_fixedpoint &zfp
      , const z3::func_decl fd) {
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    z3::expr a = z3::expr(ctx, Z3_mk_bound(ctx, 0, bvSort));
    z3::expr b = z3::expr(ctx, Z3_mk_bound(ctx, 1, bvSort));
    z3::expr v = z3::expr(ctx, Z3_mk_bound(ctx, 2, bvSort));

    z3::func_decl domDecl = getDomFuncDecl(ctx);
    z3::func_decl nrDecl = getNotReachFuncDecl(ctx);
    z3::func_decl mhbDecl = getMHBFuncDecl(ctx);
    z3::func_decl fenceDecl = getFenceFuncDecl(ctx);

    // (dom a b)
    // (mhb a b)
    z3::expr args[2] = {a, b};
    z3::expr domAB = domDecl(2, args);
    z3::expr mhbAB = mhbDecl(2, args);

    // (not-reach b a)
    args[0] = b;
    args[1] = a;
    z3::expr nrBA = nrDecl(2, args);

    // (is-fence b)
    args[0] = b;
    z3::expr fenceB = fenceDecl(1, args);

    // (fd a v)
    args[0] = a;
    args[1] = v;
    z3::expr fdAV = fd(2, args);

    z3::expr bigAnd = domAB && nrBA && fenceB && fdAV;

    z3::expr imp = z3::implies(bigAnd, mhbAB);
    Z3_fixedpoint_add_rule(ctx, zfp, imp, NULL);
  }

  // (=> (reads-from b a) (mhb a b))
  void addReadsFromIsMHB(z3::context &ctx, Z3_fixedpoint &zfp) {
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    z3::expr a  = z3::expr(ctx, Z3_mk_bound(ctx, 2, bvSort));
    z3::expr b  = z3::expr(ctx, Z3_mk_bound(ctx, 3, bvSort));

    z3::func_decl rfDecl = getReadsFromFuncDecl(ctx);
    z3::func_decl mhbDecl = getMHBFuncDecl(ctx);
    
    z3::expr argsAB[2] = {a, b};
    z3::expr argsBA[2] = {b, a};
    z3::expr rf_b_a = rfDecl(2, argsBA);
    z3::expr mhb_a_b = mhbDecl(2, argsAB);

    z3::expr imp = z3::implies(rf_b_a, mhb_a_b);
    Z3_fixedpoint_add_rule(ctx, zfp, imp, NULL);
  }

  // (=> (and (dom a b) 
  //          (not-reach b a)
  //          (is-store a _) 
  //          (is-store b _) )
  //     (mhb a b))
  //
  // i.e., stores to any variable are ordered on TSO
  void addDomStoreStoreTSO(z3::context &ctx, Z3_fixedpoint &zfp) {
    addStoreStoreMHB(ctx, zfp, false);
    //z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);

    //z3::expr a = z3::expr(ctx, Z3_mk_bound(ctx, 0, bvSort));
    //z3::expr b = z3::expr(ctx, Z3_mk_bound(ctx, 1, bvSort));
    //z3::expr v1 = z3::expr(ctx, Z3_mk_bound(ctx, 2, bvSort));
    //z3::expr v2 = z3::expr(ctx, Z3_mk_bound(ctx, 3, bvSort));

    //z3::func_decl domDecl = getDomFuncDecl(ctx);
    //z3::func_decl nrDecl = getNotReachFuncDecl(ctx);
    //z3::func_decl stDecl = getStFuncDecl(ctx);
    //z3::func_decl mhbDecl = getMHBFuncDecl(ctx);

    //// (dom a b)
    //// (mhb a b)
    //z3::expr args[2] = {a, b};
    //z3::expr domAB = domDecl(2, args);
    //z3::expr mhbAB = mhbDecl(2, args);

    //// (not-reach b a)
    //args[0] = b;
    //args[1] = a;
    //z3::expr nrBA = nrDecl(2, args);

    //// (is-store a v1)
    //// (is-store b v2)
    //// v1 may or may not equal v2 to satisfy the deduction
    //args[0] = a;
    //args[1] = v1;
    //z3::expr isStAV1 = stDecl(2, args);
    //args[0] = b;
    //args[1] = v2;
    //z3::expr isStBV2 = stDecl(2, args);

    //// (and (dom a b) 
    ////      (not-reach b a)
    ////      (is-store a _) 
    ////      (is-store b _))
    //z3::expr bigAnd = domAB && nrBA && isStAV1 && isStBV2;

    //z3::expr imp = z3::implies(bigAnd, mhbAB);
    //Z3_fixedpoint_add_rule(ctx, zfp, imp, NULL);
  }


  // (=> (and (dom a b) 
  //          (not-reach b a)
  //          (is-store a v) 
  //          (is-store b v) )
  //     (mhb a b))
  //
  // i.e., stores to the same variable are ordered on TSO
  void addDomStoreStorePSO(z3::context &ctx, Z3_fixedpoint &zfp) {
    addStoreStoreMHB(ctx, zfp, true);
  }

  // (=> (and (dom a f)
  //          (dom f b)
  //          (fd1 a v1)
  //          (fd2 b v2)
  //          (fd3 f)
  //      ))
  void addNoReoDomFunc2Func2Func1(z3::context, Z3_fixedpoint &zfp
      , const z3::func_decl fd1, const z3::func_decl fd2
      , const z3::func_decl fd3, const z3::expr v1
      , const z3::expr v2, const z3::expr f) {
  }

  // (=> (and (fd1 a v1)
  //          (fd2 b v2))
  //     (noreorder a b)
  void addNoReoFunc2Func2(z3::context &ctx, Z3_fixedpoint &zfp
      , const z3::func_decl fd1, const z3::func_decl fd2
      , const z3::expr v1, const z3::expr v2) {
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    z3::expr a = z3::expr(ctx, Z3_mk_bound(ctx, 0, bvSort));
    z3::expr b = z3::expr(ctx, Z3_mk_bound(ctx, 1, bvSort));

    z3::func_decl noReoDecl = getNoReorderFuncDecl(ctx);

    z3::expr args[2] = {a, v1};
    z3::expr fd1AV1 = fd1(2, args);

    args[0] = a;
    args[1] = b;
    z3::expr noReo = noReoDecl(2, args);

    args[0] = b;
    args[1] = v2;
    z3::expr fd2BV2 = fd2(2, args);

    z3::expr bigAnd = fd1AV1 && fd2BV2;

    z3::expr imp = z3::implies(bigAnd, noReo);
    Z3_fixedpoint_add_rule(ctx, zfp, imp, NULL);
  }

  // (=> (and (fd1 a)
  //          (fd2 b v))
  //     (noreorder a b)
  void addNoReoFunc1Func2(z3::context &ctx, Z3_fixedpoint &zfp
      , const z3::func_decl fd1, const z3::func_decl fd2) {
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    z3::expr a = z3::expr(ctx, Z3_mk_bound(ctx, 0, bvSort));
    z3::expr b = z3::expr(ctx, Z3_mk_bound(ctx, 1, bvSort));
    z3::expr v = z3::expr(ctx, Z3_mk_bound(ctx, 1, bvSort));

    z3::func_decl noReoDecl = getNoReorderFuncDecl(ctx);

    z3::expr args[2] = {a, v};
    z3::expr fd1A = fd1(1, args); // v is not used in args here

    args[0] = a;
    args[1] = b;
    z3::expr noReo = noReoDecl(2, args);

    args[0] = b;
    args[1] = v;
    z3::expr fd2BV = fd2(2, args);

    z3::expr bigAnd = fd1A && fd2BV;

    z3::expr imp = z3::implies(bigAnd, noReo);
    Z3_fixedpoint_add_rule(ctx, zfp, imp, NULL);
  }

  // (=> (and (fd1 a v)
  //          (fd2 b))
  //     (noreorder a b)
  void addNoReoFunc2Func1(z3::context &ctx, Z3_fixedpoint &zfp
      , const z3::func_decl fd1, const z3::func_decl fd2) {
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    z3::expr a = z3::expr(ctx, Z3_mk_bound(ctx, 0, bvSort));
    z3::expr b = z3::expr(ctx, Z3_mk_bound(ctx, 1, bvSort));
    z3::expr v = z3::expr(ctx, Z3_mk_bound(ctx, 1, bvSort));

    z3::func_decl noReoDecl = getNoReorderFuncDecl(ctx);

    z3::expr args[2] = {a, v};
    z3::expr fd1AV = fd1(2, args);

    args[0] = a;
    args[1] = b;
    z3::expr noReo = noReoDecl(2, args);

    args[0] = b;
    z3::expr fd2B = fd2(1, args);

    z3::expr bigAnd = fd1AV && fd2B;

    z3::expr imp = z3::implies(bigAnd, noReo);
    Z3_fixedpoint_add_rule(ctx, zfp, imp, NULL);
  }


  // Order two statements based on program order and restricted based on two
  // binary predicates with the passed variables as their second item.
  // (=> (and (dom a b) 
  //          (not-reach b a)
  //          (fd1 a v1) 
  //          (fd2 b v2) )
  //     (mhb a b))
  void addPOFunc2Func2MHB(z3::context &ctx, Z3_fixedpoint &zfp
      , const z3::func_decl fd1, const z3::func_decl fd2
      , const z3::expr v1, const z3::expr v2) {
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    z3::expr a = z3::expr(ctx, Z3_mk_bound(ctx, 0, bvSort));
    z3::expr b = z3::expr(ctx, Z3_mk_bound(ctx, 1, bvSort));

    z3::func_decl domDecl = getDomFuncDecl(ctx);
    z3::func_decl nrDecl = getNotReachFuncDecl(ctx);
    z3::func_decl mhbDecl = getMHBFuncDecl(ctx);

    // (dom a b)
    // (mhb a b)
    z3::expr args[2] = {a, b};
    z3::expr domAB = domDecl(2, args);
    z3::expr mhbAB = mhbDecl(2, args);

    // (not-reach b a)
    args[0] = b;
    args[1] = a;
    z3::expr nrBA = nrDecl(2, args);

    // (fd1 a v1)
    args[0] = a;
    args[1] = v1;
    z3::expr fd1AV1 = fd1(2, args);

    // (fd2 b v2)
    args[0] = b;
    args[1] = v2;
    z3::expr fd2BV2 = fd2(2, args);

    // (and (dom a b) 
    //      (not-reach b a)
    //      (fd1 a v1) 
    //      (fs2 b v2))
    z3::expr bigAnd = domAB && nrBA && fd1AV1 && fd2BV2;
    z3::expr imp = z3::implies(bigAnd, mhbAB);
    Z3_fixedpoint_add_rule(ctx, zfp, imp, NULL);
  }

  // Order a store and load based on program-order and if they are to the same
  // variable
  // (=> (and (dom a b) 
  //          (not-reach b a)
  //          (is-store a v)
  //          (is-load b v))
  //     (mhb a b))
  void addStoreLoadMHB(z3::context &ctx, Z3_fixedpoint &zfp) {
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    z3::expr v = z3::expr(ctx, Z3_mk_bound(ctx, 2, bvSort));

    addPOFunc2Func2MHB(ctx, zfp, getStFuncDecl(ctx), getLdFuncDecl(ctx), v, v);
  }


  // Order a load and store based on program-order and if they are to the same
  // variable
  // (=> (and (dom a b) 
  //          (not-reach b a)
  //          (is-load a v))
  //          (is-store b v)
  //     (mhb a b))
  void addLoadStoreMHB(z3::context &ctx, Z3_fixedpoint &zfp) {
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    z3::expr v = z3::expr(ctx, Z3_mk_bound(ctx, 2, bvSort));

    addPOFunc2Func2MHB(ctx, zfp, getLdFuncDecl(ctx), getStFuncDecl(ctx), v, v);
  }

  void addLoadLoadMHB(z3::context &ctx, Z3_fixedpoint &zfp) {
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    z3::expr v = z3::expr(ctx, Z3_mk_bound(ctx, 2, bvSort));

    addPOFunc2Func2MHB(ctx, zfp, getLdFuncDecl(ctx), getLdFuncDecl(ctx), v, v);
  }


  // Add an ordering rule between two stores. If sameVar is true then the two
  // stores are ordered based on the program-order and if they access the same
  // variable (as on PSO). Otherwise, the stores may access any variable (as on
  // TSO).
  //
  // That is:
  //
  // (=> (and (dom a b) 
  //          (not-reach b a)
  //          (is-store a v1) 
  //          (is-store b v2) )
  //     (mhb a b))
  //
  // Where v1 = v2 is sameVar is true
  void addStoreStoreMHB(z3::context &ctx, Z3_fixedpoint &zfp, bool sameVar) {
    z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);
    z3::expr v1 = z3::expr(ctx, Z3_mk_bound(ctx, 2, bvSort));
    z3::expr v2 = v1;
    if (!sameVar) {
      v2 = z3::expr(ctx, Z3_mk_bound(ctx, 3, bvSort));
    }

    addPOFunc2Func2MHB(ctx, zfp, getStFuncDecl(ctx), getStFuncDecl(ctx), v1
        , v2);
    //z3::sort bvSort = ctx.bv_sort(Z3_BV_SIZE);

    //z3::expr a = z3::expr(ctx, Z3_mk_bound(ctx, 0, bvSort));
    //z3::expr b = z3::expr(ctx, Z3_mk_bound(ctx, 1, bvSort));
    //z3::expr v1 = z3::expr(ctx, Z3_mk_bound(ctx, 2, bvSort));
    //
    //z3::expr v2 = v1;

    //if (!sameVar) {
    //  v2 = z3::expr(ctx, Z3_mk_bound(ctx, 3, bvSort));
    //}

    //z3::func_decl domDecl = getDomFuncDecl(ctx);
    //z3::func_decl nrDecl = getNotReachFuncDecl(ctx);
    //z3::func_decl stDecl = getStFuncDecl(ctx);
    //z3::func_decl mhbDecl = getMHBFuncDecl(ctx);

    //// (dom a b)
    //// (mhb a b)
    //z3::expr args[2] = {a, b};
    //z3::expr domAB = domDecl(2, args);
    //z3::expr mhbAB = mhbDecl(2, args);

    //// (not-reach b a)
    //args[0] = b;
    //args[1] = a;
    //z3::expr nrBA = nrDecl(2, args);

    //// (is-store a v1)
    //// (is-store b v2)
    //args[0] = a;
    //args[1] = v1;
    //z3::expr isStAV1 = stDecl(2, args);
    //args[0] = b;
    //args[1] = v2;
    //z3::expr isStBV2 = stDecl(2, args);

    //// (and (dom a b) 
    ////      (not-reach b a)
    ////      (is-store a _) 
    ////      (is-store b _))
    //z3::expr bigAnd = domAB && nrBA && isStAV1 && isStBV2;

    //z3::expr imp = z3::implies(bigAnd, mhbAB);
    //Z3_fixedpoint_add_rule(ctx, zfp, imp, NULL);
  }

  // Return the first load, store, or terminator instruction in the basic
  // block.
  static Instruction *getFirstLoadStoreTerm(BasicBlock *b) {
    assert(b && "NULL passed");
    for (auto i = b->begin(), ie = b->end(); i != ie; ++i) {
      Instruction *cur = &*i;
      if (isa<LoadInst>(cur)) {
        return cur;
      }
      else if (isa<StoreInst>(cur)) {
        return cur;
      }
    }
    // Reaching this point means the basicblock does not have any load or store
    // instructions. So, just return the terminator
    return b->getTerminator();
  }

  static Instruction *getFirstLoadStoreCallTerm(BasicBlock *b) {
      assert(b && "NULL passed");
      for (auto i = b->begin(), ie = b->end(); i != ie; ++i) {
          Instruction *cur = &*i;
          if (isa<LoadInst>(cur)) {
              return cur;
          }
          else if (isa<StoreInst>(cur)) {
              return cur;
          }
          else if (isa<CallInst>(cur)) {
              return cur;
          }
      }
      // Reaching this point means the basicblock does not have 
      // any load or store instructions. So, just return the terminator
      return b->getTerminator();
  }

  static Instruction *getLastLoadStoreCallTerm(BasicBlock *b) {
      assert(b && "NULL passed");
      bool find = false;
      Instruction *cur;
      for (auto i = b->begin(), ie = b->end(); i != ie; ++i) {
          cur = &*i;
          if (isa<LoadInst>(cur)) {
              find = true;
          }
          else if (isa<StoreInst>(cur)) {
              find = true;
          }
          else if (isa<CallInst>(cur)) {
              find = true;
          }
      }
      if (find) {
          return cur;
      } else {
          // Reaching this point means the basicblock does not have 
          // any load or store instructions. So, just return the front one
          return &(b->front());
      }
  }
#if 0
  // Given the passed basicblock, return, starting from the first, the loads
  // and stores in the basicblock
  static std::vector<Instruction *> getLoadStoreOrdered(BasicBlock *b) {
      std::vector<Instruction *> ret;
    for (auto i = b->begin(), ie = b->end(); i != ie; ++i) {
      Instruction *cur = &*i;
      if (isa<LoadInst>(cur)) {
        DEBUG_MSG("[DEBUG] Found Load: " << cur << ": " << *cur << '\n');
        ret.push_back(cur);
      }
      else if (isa<StoreInst>(cur)) {
        DEBUG_MSG("[DEBUG] Found Store: " << cur << ": " << *cur << '\n');
        ret.push_back(cur);
      }
    }
    return ret;
  }
#endif

  static std::vector<Instruction *> getLoadStoreCallOrdered(BasicBlock *b) {
      std::vector<Instruction *> ret;
      for (auto i = b->begin(), ie = b->end(); i != ie; ++i) {
          Instruction *cur = &*i;
          if (isa<LoadInst>(cur)) {
              DEBUG_MSG("[DEBUG] Found Load: " << cur << ": " << *cur << '\n');
              ret.push_back(cur);
          }
          else if (isa<StoreInst>(cur)) {
              DEBUG_MSG("[DEBUG] Found Store: " << cur << ": " << *cur << '\n');
              ret.push_back(cur);
          }
          else if (isa<CallInst>(cur)) {
              DEBUG_MSG("[DEBUG] Found Call" << cur << ": " << *cur << '\n');
              ret.push_back(cur);
              //DEBUG_MSG("[DEBUG] Found Store: " << cur << ": " << *cur << '\n');
              //ret.push_back(cur);
          }
      }
      return ret;
  }

  // Ensure the global command line flags are valid.
  // This will crash with an error message if they are not
  void checkCommandLineArgs() const {
    // Atleast one abstract domain must be true
    if (!useBoxG && !useOctG && !usePolyG && !useLinEqG) {
      errs() << "[ERROR] Select an abstract domain (-box, -oct, -pkpoly "
        << "-pklineq)\n";
      exit(EXIT_FAILURE);
    }

    // TSO and PSO cannot both be enabled at the same time
    if (tsoConstrG && psoConstrG) {
      errs() << "[ERROR] both -pso and -tso cannot be used\n";
      exit(EXIT_FAILURE);
    }

    // Only one abstract domain should be selected
    if ((useBoxG && (useOctG || usePolyG || useLinEqG))
        || (useOctG && (useBoxG || usePolyG || useLinEqG)) 
        || (usePolyG && (useBoxG || useOctG || useLinEqG))
        || (useLinEqG && (useBoxG || useOctG || usePolyG))) {
      errs() << "[ERROR] More than one abstract domain selected\n";
      exit(EXIT_FAILURE);
    }

    // The program-order constraint solver is only relevant in combinational
    // exploration mode
    if (useConstraintsG && noCombinsG) {
      errs() << "[ERROR] Constraint solver mode (-constraints) must not use " 
        "-nocombs\n";
      exit(EXIT_FAILURE);
    }
    // If we are using constraints, we need to know where Z3 lives
    if (useConstraintsG && (z3BinLocG.size() == 0)) {
      errs() << "[ERROR] -constraints without specify location of Z3 "
                "(-z3 <loc>)\n";
      exit(EXIT_FAILURE);
    }
    // Using constraints also uses dynamic thread init. Note: this is not
    // strictly necessary.
    if (useConstraintsG) {
      dynInitG = true;
    }
  }

  void printFuncInterfMap(
      const std::map<Function *, std::map<Instruction *, LatticeFact>> fim) const {
    for (auto i = fim.begin(), ie = fim.end(); i != ie; ++i) {
      std::map<Instruction *, LatticeFact> stm = i->second;
      for (auto j = stm.begin(), je = stm.end(); j != je; ++j) { 
        errs() << "Store: " << *(j->first) << '\n';
      }
    }
  }

  std::map<StoreInst *, LatticeFact> joinStFacts(
      const std::map<StoreInst *, LatticeFact> m1
      , const std::map<StoreInst *, LatticeFact> m2) const {
    std::map<StoreInst *, LatticeFact> ret(m1);
    for (auto i = m2.begin(), ie = m2.end(); i != ie; ++i) {
      StoreInst *cur = i->first;
      auto f = ret.find(cur);
      if (f != ret.end()) {
        LatticeFact newF = LatticeFact::factJoin(i->second, f->second);
        ret.erase(cur);
        ret.emplace(cur, newF);
      }
      else {
        ret.emplace(cur, f->second);
      }
    }
    return ret;
  }

  // Return the number of instructions in M
  unsigned numInstructions(Module &M) {
    unsigned n = 0;
		for (auto mit = M.begin(), me = M.end(); mit != me; ++mit) {
			Function &f = *mit;
			for (inst_iterator ii = inst_begin(f), ie = inst_end(f); ii != ie
					; ++ii) {
        n++;
      }
    }
    return n;
  }

  std::set<Instruction *> instsWithoutMetadata(Module &M, std::string md) {
    assert(md.length() && "instsWithoutMetadata: length zero metadata");
    std::set<Instruction *> ret;
    return ret;
		for (auto mit = M.begin(), me = M.end(); mit != me; ++mit) {
			Function &f = *mit;
			for (inst_iterator ii = inst_begin(f), ie = inst_end(f); ii != ie
					; ++ii) {
				Instruction *i = &*ii;
        if (!i->getMetadata(md)) {
          ret.insert(i);
        }
      }
    }
    return ret;
  }
  
  std::set<Instruction *> instsWithMetadata(Module &M, std::string md) {
    assert(md.length() && "instsWithoutMetadata: length zero metadata");
    std::set<Instruction *> ret;
		for (auto mit = M.begin(), me = M.end(); mit != me; ++mit) {
			Function &f = *mit;
			for (inst_iterator ii = inst_begin(f), ie = inst_end(f); ii != ie
					; ++ii) {
				Instruction *i = &*ii;
        if (i->getMetadata(md)) {
          ret.insert(i);
        }
      }
    }
    return ret;
  }

  // If -impact is used then return the set of all statements in M which may
  // impact a change.
  //
  // Otherwise, return an empty set
  std::set<Instruction *> getMayImpactIfEnabled(Module &M) {
		std::set<Instruction *> ret;
    if (!impactG) {
      return ret;
    }
    ret = instsWithMetadata(M, "MayImpact");
    //if (ret.size() == 0) {
    //  errs() << "[ERROR] No may-impact statements found, did you run the change-impact pass?\n";
    //  exit(EXIT_FAILURE);
    //}
    return ret;
  }

  // If -impact is used then return the set of all statements in M which are
  // impacted by a change.
  //
  // Otherwise, return an empty set
  std::set<Instruction *> getImpactedIfEnabled(Module &M) {
		std::set<Instruction *> ret;
    if (!impactG) {
      return ret;
    }
    ret = instsWithMetadata(M, "Impacted");
    if (ret.size() == 0) {
      errs() << "[ERROR] No impacted statements found, did you run the change-impact pass?\n";
      exit(EXIT_FAILURE);
    }
    return ret;
  }

  // Return the inversion of the assertion slice if assertSliceG is true.
  //
  // The "slice inversion" is all the statements *not* on the slice.
  //
  // If assertSliceG is false then return an empty set.
	std::set<Instruction *> getAssertSliceInvIfEnabled(Module &M) {
		std::set<Instruction *> ret;
    unsigned numInsts = numInstructions(M);
		if (!assertSliceG) {
			return ret; // size zero
		}
    ret = instsWithoutMetadata(M, "AssertSlice");
		//for (auto mit = M.begin(), me = M.end(); mit != me; ++mit) {
		//	Function &f = *mit;
		//	for (inst_iterator ii = inst_begin(f), ie = inst_end(f); ii != ie
		//			; ++ii) {
    //    numInsts++;
		//		Instruction *i = &*ii;
		//		if (!i->getMetadata("AssertSlice")) {
		//			ret.insert(i);
		//		}
		//	}
		//}
		//if (!ret.size()) {
		if (numInsts == ret.size()) {
			//errs() << "[ERROR] no statements found on slice of an assertion\n"
			//					"Are you sure the PDG pass has been run?\n";
      errs() << "[ERROR] no statements not found on slice of an assertion\n"
                "Are you sure the PDG pass has been run?\n";
			exit(EXIT_FAILURE);
		}
		return ret;
	}
};

char WorklistAI::ID = 0;
static RegisterPass<WorklistAI> X("worklist-ai"
    , "worklist based abstract interpretation"
    , false  // unmodified CFG
    , true); // analysis pass
