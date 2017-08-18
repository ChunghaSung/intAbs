/*
 * Author: Markus Kusano
 *
 * Utility database.
 *
 * Maps an llvm::Value* to a string.
 *
 * Also supports dumping the entire map to the Metadata of a module. This
 * allows for the data to later be parsed in in a different pass/tool.
 */
#pragma once

#include "llvm/IR/Instruction.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Value.h"
#include <string>

class ValToStrDB {
  public:
    // Given a constant in the program, return a string representation
    struct Constants {
      static constexpr char const *const NULL_STR = "NULL";
      static constexpr char const *const CONST_INT_SUFFIX = "CONST_INT";
      static std::string getStr(llvm::Constant *c);
    };

    // If true, the LLVM module database will use bitvector representation of
    // all the IDs. If false, it will use the string representation Note that
    // when this is true, the module will only contain the bitvector strings:
    // there will be no way to get the old string versions back.
    bool useBitvectors = false;

    // Convert the passed Value which is assumed to be a ConstantInt to a
    // uint64_t.
    //
    // This will crash if the passed value is not a ConstantInt.
    // This will crash if the passed ConstantInt is larger than 64 bits.
    static uint64_t constantIntToUint64(llvm::Value *c);

    // Convert the passed ConstantInt to a uint64_t
    //
    // This will crash if the passed ConstantInt is larger than 64 bits
    static uint64_t constantIntToUint64(llvm::ConstantInt *c);

    // Convert the passed uint64_t to APInt
    static llvm::APInt uint64ToAPInt(uint64_t u);

    // Convert the passed unsigned to APInt
    static llvm::APInt unsignedToAPInt(unsigned u);

    // Return the type of the ith argument of the function
    static llvm::Type *getFuncArgTy(llvm::Function *f, unsigned i);

    // Return a reference to the ith argument of the function
    static llvm::Argument &getFuncArg(llvm::Function *f, unsigned i);

    // Return a string containing '*'s equal to the number of nested pointers.
    //
    // For example, an "**i8" type will return "**"
    static std::string getStars(llvm::Type *ty);

    // Remove one star from the prefix of the passed string.
    //
    // This will crash if a star does not exist
    static std::string rmStar(std::string str);

    // Scan the passed module's metadata and populate a map from any Value to
    // its database ID
    static std::map<llvm::Value *, std::string> 
        parseIDMapFromModule(llvm::Module &M);

    // Name of the metadata on each instruction containing the the database ID
    static constexpr char const *const METADATA_NAME = "CIAA_INFO";

    // Name of the named metadata of the module containing the function
    // argument IDs
    static constexpr char const *const 
        FUNC_ARGS_METADATA_NAME = "FUNC_ARGS_INFO";

    // Name of the named metadata of the module containing the global IDs
    static constexpr char const *const GLOBALS_METADATA_NAME = "GLOBALS_INFO";

    // Name of the named metadata of the module containing the function IDs
    static constexpr char const *const FUNC_METADATA_NAME = "FUNC_INFO";

    // Dump the internal IDMap of this class to the metadata of the module. 
    //
    // For instructions stored in the map, they will have their Database ID in
    // a metadata node named "METADATA_NAME" attached to the instruction. The
    // ID is stored in a MDNode as a MDString.
    //
    // Function arguments will be stored in a named metadata node globally in
    // the module. The named metadata node will be named
    // FUNC_ARG_METADATA_NAME. It will contain one MDNode which will be an
    // array of the form:
    //
    // <(func1, argpos1, ID1), (func2, argpos2, ID2), ...>
    //
    // Similarly, globals will be stored in a named MD node with the name
    // <GLOBALS_METADATA_NAME> in the form:
    //
    // <(global var, ID), ... >
    //
    // Where GlobalVar is a llvm::GlobalVariable and ID is a MDString
    // representing the database ID of the global
    //
    // Where func1 is an LLVM FUnction, argpos is a ConstantInt and ID is an
    // MDString
    //
    // Functions themselves will be stored similar to globals. They will be in
    // a named metadata node named FUNC_METADATA_NAME:
    //
    // <(function, ID), ...>
    //
    // Where function is an LLVM::Function and ID is an MDString
    void dumpIDMapToModule(llvm::Module &M);

    // Debugging function. Returns true if the passed two maps are equal
    static bool idMapEqual(std::map<llvm::Value *, std::string> m1, 
        std::map<llvm::Value *, std::string> m2);
    static bool idMapEqual(ValToStrDB db1, ValToStrDB db2);

    // Given a ConstExpr, return the value being used in the expression
    static llvm::Value *getConstExprUse(llvm::ConstantExpr *ce);

    // Returns the integer ID of the passed string. It will be saved and
    // returned on future calls
    //
    // This should be used to convert stored values (with string keys) values
    // with unsigned keys
    unsigned saveAndGetIntID(std::string s);

    // convienience function: given a value, directly return its integer ID. It
    // will be added to both the val->str and str->unsigned maps
    unsigned saveAndGetIntID(llvm::Value *v);

		//Value *getValOrNULL(

    // Get the ID of the passed value (or create a new one).
    //
    // The ID will be saved in an internal map and returned on future calls.
    //
    // If the value is already stored in the map, it will simply be returned.
    std::string saveAndGetID(llvm::Value *v);

		// Return the associated value of the passed string.
		// If the string is not in the map (i.e., there is not value with it as a
		// key) returns NULL.
		llvm::Value *getValueOrNULL(std::string s);
		llvm::Value *getValueOrNULL(unsigned id);

    // Internal map:
    // TODO: This should be private
    std::map<llvm::Value *, std::string> IDMap;
		// Reverse map of IDMap
		std::map<std::string, llvm::Value *> RevIDMap;


    // This map mirrors IDMap except it maps each string (the values in IDMap)
    // to a unique integer. That is, it assigns a unique identifier to each
    // string
    //
    // It can be used as a chain: given an Value, pass to IDMap for a string;
    // given that string, pass to StrIdToInt for an unsigned
    //
    std::map<std::string, unsigned> StrIDToInt;
		// Reverse map of StrIDToint
		std::map <unsigned, std::string> RevStrIDToInt;

  private:
    // Insert an item into the internal map. This will map v to s.
    //
    // Since there are multiple internal maps to update on insertion, this
    // function should be used instead of directly inserting the item into the
    // map.
    //
    // If the item already exists in the map, this function will crash.
    void mapInsert(llvm::Value *v, std::string s);

};
