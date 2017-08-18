/**
 * Author: Markus Kusano
 *
 * Some general utility functions that didn't have a home
 */

#include <string>
#include <map>
#include <cstdint>
#include "llvm/IR/Value.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"

#include "ap_manager.h"

namespace Utils {

  // Lookup the passed key and return its value.
  //
  // If the item is not found, crash.
  std::string valueToStringUnsafe(std::map<llvm::Value*, std::string> m
      , llvm::Value *v);

  // Depending on the command line flags (see worklistai.cpp), return the
  // appropriate manager.
  ap_manager_t *getManager();

  // Return true if the passed global variable is a (pointer to) an integer
  bool isIntegerGlobal(llvm::GlobalVariable *gv);

  // Return true if the passed global variable is a (point to) a floatint point
  // type
  bool isFloatGlobal(llvm::GlobalVariable *gv);

  // Return true if l is a load of some sort (e.g., LoadInst, AtomicRMW)
  bool isSomeLoad(llvm::Instruction *l);

  // Same as isSomeLoad but for a store
  bool isSomeStore(llvm::Instruction *s);

  // Given some store instruction (StoreInst, AtomicRMW) return its pointer operand
  llvm::Value *getStorePtr(llvm::Instruction *i);

  // Return the integer form of the passed constant.
  //
  // If it is not a constant int, crash.
  // If the constant is larger than INT_MAX then crash.
  int getConstantIntUnsafe(const llvm::ConstantInt * const c);

  // Generic function to call std::map::at().
  // This will crash if the value is not found in the map
  template <typename K, typename V>
  V mapAtUnsafe(const std::map<K, V> m, K key) { 
    try {
      V ret = m.at(key);
      return ret;
    }
    catch (const std::out_of_range &oor) {
      llvm::errs() << "[ERROR] mapAtUnsafe: value not in map\n";
      assert(0 && "see above");
      exit(EXIT_FAILURE);
    }
  }

  // Add the passed item to the vector at the passed key.
  template <typename K, typename V>
  void mapInsertToVector(std::map<K, std::vector<V>> &m, K key, V val) {
    auto it = m.find(key);
    if (it == m.end()) {
      // Vector does not exist. Create a size one vector and insert it
      std::vector<V> sz1;
      sz1.push_back(val);
      m.emplace(key, sz1);
      return;
    }
    else {
      // Vector already exists.
      std::vector<V> old = it->second;
      old.push_back(val);
      m.erase(it);
      m.emplace(key, old);
      return;
    }
  }

  template <typename K>
  void addVector(std::vector<K> &base, const std::vector<K> a) {
    base.reserve(base.size() + a.size());
    base.insert(base.end(), a.begin(), a.end());
  }
} // namespace Utils
