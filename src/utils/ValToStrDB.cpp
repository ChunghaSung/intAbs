#include "ValToStrDB.h"
#include "utils.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"

#include "mk_debug.h"

using namespace llvm;

Type *ValToStrDB::getFuncArgTy(Function *f, unsigned i) {
  return getFuncArg(f, i).getType();
}

Argument &ValToStrDB::getFuncArg(Function *f, unsigned i) {
  assert(f != NULL && "NULL passed");
  // Iterate down the list i times
  for (auto ai = f->arg_begin(), e = f->arg_end(); ai != e; ++ai) {
    Argument &a = *ai;
    if (a.getArgNo() == i) {
      return a;
    }
  }
  llvm_unreachable("Operand out of bounds");
}

Value *ValToStrDB::getConstExprUse(ConstantExpr *ce) {
  Instruction *asI = ce->getAsInstruction();
  if (GetElementPtrInst *gep = dyn_cast<GetElementPtrInst>(asI)) {
    DEBUG_MSG("\tConstExpr is GEP\n");
    // Have the argument points to the pointer in the GEP.
    // TODO: Optimization:
    // All GEP instructions to the same base pointer are considered to
    // alias. See visitGetElementPtr for more information
    return gep->getPointerOperand();
  }
  else if (CastInst *ci = dyn_cast<CastInst>(asI)) {
    // A bit cast is dependent on the value being casted
    return ci->getOperand(0);
  }
  else {
    errs() << "[ERROR] to: ConstExpr as Instruction: " << *asI << '\n';
    llvm_unreachable("unhandled ConstExpr type (see above)");
  }
}

std::string ValToStrDB::saveAndGetID(Value *v) {
  assert(v && "NULL passed");
  DEBUG_MSG("saveAndGetID():\n");
  DEBUG_MSG("\tvalue: " << *v << '\n');

  // attempt to convert the value into something we can handle
  if (ConstantExpr *ce = dyn_cast<ConstantExpr>(v)) {
    v = getConstExprUse(ce);
  }

  // Check if the value is something we can save
  if (!(isa<Instruction>(v) 
          || isa<Argument>(v)
          || isa<GlobalVariable>(v)
          || isa<Constant>(v))) {
    errs() << "Value: " << *v << '\n';
    llvm_unreachable("unhandled types (see above)");
    exit(EXIT_FAILURE);
  }

  // For constants, they have a static string reperesentation which is not
  // saved in the IDMap
  // Const globals are treated as values
  if (ConstantInt *c = dyn_cast<ConstantInt>(v)) {
    return Constants::getStr(c);
  }
  if (ConstantPointerNull *c = dyn_cast<ConstantPointerNull>(v)) {
    return Constants::getStr(c);
  }

  auto i = IDMap.find(v);
  if (i == IDMap.end()) {
    DEBUG_MSG("\tvalue not found in map\n");
    // Assign a new integer to the value
    uint64_t id = IDMap.size();

    // Append the number of stars for the given type to the ID
    Type *ty = v->getType();
    assert(ty != NULL && "NULL Type");

    //DEBUG_MSG("saveAndGetID(): Type: " << *ty << '\n');
    //assert(ty->isPointerTy() && "Non-pointer type");

    // Append the number of nested pointer values on the ID
    std::string newID;
    if (ty->isPointerTy()) {
      std::string stars = getStars(ty);
      newID = stars + std::to_string(id);
    }
    else {
      newID = std::to_string(id);
    }
    assert(newID.size() && "newID not set");

    mapInsert(v, newID);

    return newID;
  }
  else {
    return i->second;
  }
  llvm_unreachable("unreachable");
}

void ValToStrDB::mapInsert(Value *v, std::string s) {
  assert(IDMap.find(v) == IDMap.end() && "Overwritting value in map");
	assert(RevIDMap.find(s) == RevIDMap.end() 
			&& "Overwritting value in rev map");
  // insert into the value to string map
  IDMap[v] = s;
	RevIDMap[s] = v;

  // Create a unique identifier for the passed string.
  auto f = StrIDToInt.find(s);

  // TODO: What happens if the item already exists in the map?
  assert(f == StrIDToInt.end());
  unsigned intID = StrIDToInt.size();
  StrIDToInt[s] = intID;
	RevStrIDToInt[intID] = s;
}

unsigned ValToStrDB::saveAndGetIntID(std::string s) {
  assert(s.size() && "size zero string passed");
  auto f = StrIDToInt.find(s);

  if (f == StrIDToInt.end()) {
    unsigned newID = StrIDToInt.size();
    StrIDToInt[s] = newID;
    return newID;
  }

  return f->second;
}

unsigned ValToStrDB::saveAndGetIntID(Value *v) {
  assert(v && "NULL passed");
  std::string s = saveAndGetID(v);
  assert(s.size() && "0 length string");
  return saveAndGetIntID(s);
}


void ValToStrDB::dumpIDMapToModule(Module &M) {
  NamedMDNode *funcArgNMDN = 
      M.getOrInsertNamedMetadata(FUNC_ARGS_METADATA_NAME);
  NamedMDNode *globalsNMDN = 
      M.getOrInsertNamedMetadata(GLOBALS_METADATA_NAME);
  NamedMDNode *funcNMDN = 
      M.getOrInsertNamedMetadata(FUNC_METADATA_NAME);

  // Delete whatever used to be in the NamedMDNode
  funcArgNMDN->dropAllReferences();
  globalsNMDN->dropAllReferences();
  funcNMDN->dropAllReferences();

  // Function argument values. Will be stored in an MDNode in funcArgNMDN
  std::vector<Value *> funcArgs;
  // Global values. Will be stored in an MDNode in globalsNMDN
  std::vector<Value *> globalVars;
  // Functions. Will be stored in a NMDNode in funcNMDN
  std::vector<Value *> funcs;

  for (auto i = IDMap.begin(), e = IDMap.end(); i != e; ++i) {
    // Assumption: There two types of values stored in the ID map: instructions
    // and function arguments
    // TODO: Probably need to handle constants too

    Value *v = i->first;
    std::string strId = i->second;
    // Convert the ID into either a bitvector or int
    MDString *mds = NULL;
    if (useBitvectors) {
      std::string bvStr = Utils::to_const_bitvec(saveAndGetIntID(strId));
      mds = MDString::get(M.getContext(), bvStr);
    }
    else {
      // Turn the ID string into a metadata string
      mds = MDString::get(M.getContext(), strId.c_str());
    }
    assert(mds && "MetaDataString not set");

    if (Instruction *inst = dyn_cast<Instruction>(v)) {
      DEBUG_MSG("Dumping instruction to module\n");
      DEBUG_MSG("\t" << *inst << '\n');
      
      // Instruction metadata is attached to the instruction itself
      MDNode *mdn = MDNode::get(M.getContext(), mds);
      inst->setMetadata(METADATA_NAME, mdn);

    }
    else if (Argument *arg = dyn_cast<Argument>(v)) {
      DEBUG_MSG("Dumping argument to module\n");
      DEBUG_MSG("\t" << *arg << '\n');

      Function *f = arg->getParent();

      // Create a constant int for the argument position
      ConstantInt *ci = ConstantInt::get(M.getContext(), 
          unsignedToAPInt(arg->getArgNo()));

      // First the function 
      funcArgs.push_back(f);
      // Then the argument position
      funcArgs.push_back(ci);
      // Then the ID
      funcArgs.push_back(mds);
    }
    else if (GlobalVariable *gv = dyn_cast<GlobalVariable>(v)) {
      DEBUG_MSG("ValToStrDB: Visiting Global: " << *gv << '\n');
      assert(M.getGlobalVariable(gv->getName()) && "Module does not contain global");
      //if (M.getGlobalVariable(gv->getName())  == NULL) {
      //  errs() << "[WARNING] Global Variable not in module. "
      //         << "It will not be saved: " << *gv << '\n';
      //}
      // First the global
      globalVars.push_back(gv);
      // Then the ID 
      globalVars.push_back(mds);
    }
    else if (ConstantExpr *ce = dyn_cast<ConstantExpr>(v)) {
      errs() << "[ERROR] ConstantExpr: " << *ce << '\n';
      llvm_unreachable("ConstantExpr in IDmap is unhandled");
      exit(EXIT_FAILURE);
    }
    else if (isa<Function>(v)) {
      // First the function
      funcs.push_back(v);
      // Then the ID
      funcs.push_back(mds);
    }
    else if (isa<Constant>(v)) {
      DEBUG_MSG("Constant: " << *v << '\n');
      llvm_unreachable("Constant in IDmap is unhandled");
      exit(EXIT_FAILURE);
    }
    else {
      // TODO: Probably need to handle other types
      errs() << "[ERROR] Value: " << *v << '\n';
      llvm_unreachable("unhandled value in IDMap (see above)");
      exit(EXIT_FAILURE);
    }
  }

  // Insert the function args to the named metadata node
  assert((funcArgs.size() % 3 == 0) && "incorrect number of func arg MDs");
  MDNode *md = MDNode::get(M.getContext(), funcArgs);
  funcArgNMDN->addOperand(md);
  assert(funcArgNMDN->getNumOperands() == 1 
      && "Incorrect size of func args NamedMDNode (!= 1)");

  // Insert the global vars to the named metadata node
  assert((globalVars.size() % 2 == 0) && "incorrect number of global MDs");
  md = MDNode::get(M.getContext(), globalVars);
  globalsNMDN->addOperand(md);
  assert(globalsNMDN->getNumOperands() == 1 
      && "Incorrect size of globals NamedMDNode (!= 1)");

  // Insert the functions to the named metadata node
  assert((funcs.size() % 2 == 0) && "incorrect number of function MDs");
  md = MDNode::get(M.getContext(), funcs);
  funcNMDN->addOperand(md);
  assert(funcNMDN->getNumOperands() == 1
      && "Incorrect size of funcs NamedMDNode (!= 1)");
}

std::map<Value *, std::string> ValToStrDB::parseIDMapFromModule(Module &M) {
  std::map<Value *, std::string> ret;

  // Iterate over every instruction searching for those with the metadata
  for (auto mi = M.begin(), me = M.end(); mi != me; ++mi) {
    Function &f = *mi;
    for (auto ii = inst_begin(f), ie = inst_end(f); ii != ie; ++ii) {
      Instruction &i = *ii;
      // Check if instruction has one of our database ID metadatas
      if (MDNode *mdn = i.getMetadata(METADATA_NAME)) {
        //Value *md = i->getMetadata(MD_STR);
        //MDNode *mdn = dyn_cast<MDNode>(md);
        //assert(mdn != NULL && "Non-MDNode metadata found");
        assert(mdn->getNumOperands() == 1 && "Malformed MDNode found (size != 1)");
        Value *val = mdn->getOperand(0);

        MDString *mds = dyn_cast<MDString>(val);
        assert(mds && "Instruction with non MDString metadata");
        std::string str = mds->getString();
        assert(str.size() && "Instruction with empty metadata string");
        DEBUG_MSG("Found instruction with metadata: " << str << '\n');
        ret[&i] = str;
      }
    }
  }

  // Find the function argument metadata node and parse it
  NamedMDNode *funcArgMD = M.getNamedMetadata(FUNC_ARGS_METADATA_NAME);
  if (funcArgMD) {
    assert(funcArgMD->getNumOperands() == 1 
        && "function Named MD of wrong size");
    MDNode *mdn = funcArgMD->getOperand(0);
    assert(mdn);
    assert((mdn->getNumOperands() % 3 == 0) && "func mdnode of wrong size");
    for (size_t i = 0; i < mdn->getNumOperands(); i += 3) {
      Value *fVal = mdn->getOperand(i);
      Value *posVal = mdn->getOperand(i + 1);
      Value *strVal = mdn->getOperand(i + 2);

      Function *f = dyn_cast<Function>(fVal);
      assert(f && "First operand not function");

      //ConstantInt *ci = dyn_cast<ConstantInt>(posVal);
      //assert(f && "Second operand not ConstantInt");
      uint64_t pos = constantIntToUint64(posVal);

      MDString *mds = dyn_cast<MDString>(strVal);
      assert(mds && "Third operand not MDString");

      std::string str = mds->getString();
      assert(str.size() && "Empty metadata string");

      // Get the argument from the function
      Argument &a = getFuncArg(f, pos);
      DEBUG_MSG("Found Function argument metadata\n");
      DEBUG_MSG("\tID: " << str << '\n');
      ret[&a] = str;
    }
  }

  // Find the global var metadata node and parse it
  NamedMDNode *globMD = M.getNamedMetadata(GLOBALS_METADATA_NAME);
  if (globMD) {
    assert(globMD->getNumOperands() == 1 
        && "global var Named MD of wrong size");
    MDNode *mdn = globMD->getOperand(0);
    assert(mdn);
    assert((mdn->getNumOperands() % 2 == 0) && "global mdnode of wrong size");
    for (size_t i = 0; i < mdn->getNumOperands(); i += 2) {
      Value *global = mdn->getOperand(i);
      Value *strVal = mdn->getOperand(i + 1);

      GlobalVariable *g = dyn_cast<GlobalVariable>(global);
      assert(g && "First operand not GlobalVar");

      MDString *mds = dyn_cast<MDString>(strVal);
      assert(mds && "Second operand not MDString");

      std::string str = mds->getString();
      assert(str.size() && "Empty metadata string");

      ret[g] = str;
    }
  }

  // Find the function metadata node and parse it
  NamedMDNode *funcMD = M.getNamedMetadata(FUNC_METADATA_NAME);
  if (funcMD) {
    assert(funcMD->getNumOperands() == 1 && "function named MD of wrong size");
    MDNode *mdn = funcMD->getOperand(0);
    assert(mdn);
    assert((mdn->getNumOperands() % 2 == 0) && "func mdnode of wrong size");
    for (size_t i = 0; i < mdn->getNumOperands(); i += 2) {
      Value *func = mdn->getOperand(i);
      Value *strVal = mdn->getOperand(i + 1);

      Function *f = dyn_cast<Function>(func);
      assert(f && "first operand not Function");
      MDString *mds = dyn_cast<MDString>(strVal);
      assert(mds && "Second operand not MDString");

      std::string str = mds->getString();
      assert(str.size() && "Empty metadata string");

      ret[f] = str;
    }
  }

  if (ret.size() == 0) {
    errs() << "[WARNING] Parsing IDMap from module, found 0 items\n";
  }
  DEBUG_MSG("Parsed ID Map: Size: " << ret.size() << '\n');
  return ret;
}

uint64_t ValToStrDB::constantIntToUint64(Value *v) {
  assert(v != NULL && "NULL value passed");
  ConstantInt *ci = dyn_cast<ConstantInt>(v);
  assert(v != NULL && "Non-constant int passed");

  return constantIntToUint64(ci);
}

uint64_t ValToStrDB::constantIntToUint64(ConstantInt *c) {
  assert(c != NULL && "Null value passed");
  return c->getZExtValue();
}

APInt ValToStrDB::uint64ToAPInt(uint64_t u) {
  // Size of uint64_t in bits
  size_t size = sizeof(uint64_t) * 8;
  return APInt(size, u);
}

APInt ValToStrDB::unsignedToAPInt(unsigned u) {
  // Size of size_t in bits
  size_t size = sizeof(size_t) * 8;
  return APInt(size, u);
}

bool ValToStrDB::idMapEqual(std::map<Value *, std::string> m1, 
    std::map<Value *, std::string> m2) {
  // Check if every item in the first map is in the second (and vice versa).
  // Also check that the stored values are the same
  for (auto i = m1.begin(), ie = m1.end(); i != ie; ++i) {
    Value *v = i->first;
    std::string str = i->second;
    auto find = m2.find(v);
    if (find == m2.end()) {
      return false;
    }
    // Key is contained in m2, check if the value is contained
    if (find->second != str) {
      return false;
    }
  }
  for (auto i = m2.begin(), ie = m2.end(); i != ie; ++i) {
    Value *v = i->first;
    std::string str = i->second;
    auto find = m1.find(v);
    if (find == m1.end()) {
      return false;
    }
    // Key is contained in m2, check if the value is contained
    if (find->second != str) {
      return false;
    }
  }
  return true;
}


bool ValToStrDB::idMapEqual(ValToStrDB db1, ValToStrDB db2) {
  DEBUG_MSG("Checking map equality\n");
  return idMapEqual(db1.IDMap, db2.IDMap);
}

std::string ValToStrDB::getStars(Type *ty) {
  assert(ty != NULL && "NULL passed");
  assert(ty->isPointerTy() && "Non-pointer type passed");

  std::string ret;
  while (ty->isPointerTy()) {
    PointerType *pty = dyn_cast<PointerType>(ty);
    ret += '*';
    ty = pty->getElementType();
  }

  assert(ret.size() && "returning size 0 string");
  return ret;
}

std::string ValToStrDB::rmStar(std::string str) {
  assert(str.size() && "empty string passed");
  assert(str[0] == '*' && "removing star that does not exist");

  return str.substr(1);
}

std::string ValToStrDB::Constants::getStr(Constant *c) {
  if (isa<ConstantPointerNull>(c)) {
    return NULL_STR;
  }
  else if (ConstantInt *ci = dyn_cast<ConstantInt>(c)) {
    APInt v = ci->getValue();
    std::string str = v.toString(10, true);
    return str + CONST_INT_SUFFIX;
  }
  else {
    DEBUG_MSG("Constant: " << *c << '\n');
    llvm_unreachable("unhandled constant, see above");
  }
}

Value *ValToStrDB::getValueOrNULL(std::string s) {
	auto it = RevIDMap.find(s);
	if (it == RevIDMap.end()) {
		return NULL;
	}
	return it->second;
}

Value *ValToStrDB::getValueOrNULL(unsigned id) {
	DEBUG_MSG("[DEBUG] getValueOrNULL(unsigned)\n");
	DEBUG_MSG("id: " << id << '\n');
	auto str2i = RevStrIDToInt.find(id);
	if (str2i == RevStrIDToInt.end()) {
		DEBUG_MSG("\tnot found in RevStrIDToInt\n");
		return NULL;
	}
	std::string strId = str2i->second;
	auto it = RevIDMap.find(strId);
	if (it == RevIDMap.end()) {
		DEBUG_MSG("\tnot found in RevIDMap\n");
		return NULL;
	}
	return it->second;
}
