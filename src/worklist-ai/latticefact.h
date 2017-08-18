/**
 * Author: Markus Kusano
 *
 * Definition of lattice facts (i.e., points in the lattice).
 *
 * Each BasicBlock has one of these on which it performs its transfer function
 *
 * Most of the functions defined here are functional (they return a copy of the
 * `this` argument after performing the operation). The ones which perform an
 * internal update have the "Destr" (destructive) suffix.
 */
#pragma once
#include "ap_abstract1.h"

#include <map>
#include "llvm/IR/Value.h"
#include "llvm/IR/Instructions.h"

class LatticeFact {
  public:
    // Initializes the internal abstract value to bottom
    //LatticeFact(ap_environment_t* e, ap_manager_t *m);
    LatticeFact(ap_environment_t* e);

    // Construct a lattice fact storing the passed abstract value.
    LatticeFact(ap_abstract1_t *av);

    // Copy constructor:
    //
    // Deep copies the abstract value and manager
    LatticeFact(const LatticeFact &other);

    // Move constructor
    LatticeFact(LatticeFact&& other);

    // Move assignment operator
    LatticeFact& operator=(LatticeFact&& other);

    // Assignment operator
    // Deep copies abstract value and manager
    LatticeFact& operator=(const LatticeFact&);

    // Print the contents of the abstract value to the passed stream
    void fprint(FILE *s) const;

    // Return the result of assigning `to <- from`. 
    //
    // Note: this does not modify the internal state of this object.
    LatticeFact assign(std::string to, std::string from) const;

    // Assign a constant to a variable
    //
    // Note: this does not modify the internal state of this object.
    LatticeFact assign(const std::string to, const int from) const;

    // If setTrue is true, do: to <- 1 in the internal abstract value.
    // If setFalse is true, do: to <- 0 in the internal abstract value.
    //
    // Both can be true (the result with a `to <- 1 OR 0`
    LatticeFact assignTrueFalse(std::string to, bool setTrue, bool setFalse);


    // Return this LatticeFact with the passed variable name to be constrained
    // to either true or false. Specifically, if tf is true, then b will be
    // one. Otherwise, b will be zero.
    //
    // All other variables in the abstract value will be unchanged.
    LatticeFact constrainTF(std::string b, const bool tf);


    // Return this lattice fact with the passed value set to either zero or
    // one.
    //
    // This differs from the overloaded call which accepts a string in that it
    // can look at what the value represents and further constrain the abstract
    // values.
    //
    // For example:
    //    %b icmp eq %1 %2
    // Simply constraining b to 0 or 1 forgets the relationship between 1 and 2
    // and b. TODO: implement this
    LatticeFact constrainTF(llvm::Value *v, const bool tf
        , std::map<llvm::Value*, std::string> v2n);

    // Constrain this lattice fact such that the passed ICmp (evaluating a
    // predicate) is either true or false.
    LatticeFact constrainICmp(llvm::ICmpInst *ic, const bool tf
        , const std::map<llvm::Value*, std::string> v2n);

    // Constrain this fact such that op1 (cons) op2 where cons is an constraint
    // type (NEQ, >=, etc...).
    // Remember that you can switch the order of op1 and op2 to get more
    // "types" of constraints.
    LatticeFact constrainFact(llvm::Value *op0
        , llvm::Value *op1, ap_constyp_t cons
        , const std::map<llvm::Value *, std::string> v2n);

    LatticeFact constrainFact(std::string op0, const int cv1
        , ap_constyp_t cons);
    LatticeFact constrainFact(const int cv0, std::string op1
        , ap_constyp_t cons);
    LatticeFact constrainFact(const std::string op0, const std::string op1
        , ap_constyp_t cons);

    // Assign the passed expression to the passed var in the objects internal
    // abstract state. This operation is destructive (the old abstract values
    // within the object are modified).
    //
    // This will NOT free the memory used by the expression
    //
    // TODO: This function should also provide access to the last parameter of
    // ap_abstract1_assign_linexpr
    void assignLinExprDestr(ap_var_t var, ap_linexpr1_t *expr);

    // Same as assignLinExpr() but frees the memory of the expression
    void assignLinExprAndClearDestr(ap_var_t var, ap_linexpr1_t *expr);

    void assignTexprDestr(ap_var_t var, ap_texpr1_t *cons);
    void assignTexprAndClearDestr(ap_var_t var, ap_texpr1_t *cons);

    // Perform the operation: to icmp eq op1 op2
    //
    // Note: this does not modify the internal state of this object.
    LatticeFact icmpEq(const std::string to, int op1, const std::string op2);
    LatticeFact icmpEq(const std::string to, const std::string op1, int op2);

    // Perform the operation: to = icmp gt op1 op2
    LatticeFact icmpGt(const std::string to, const std::string op1, int op2);
    LatticeFact icmpGt(const std::string to, int op1, const std::string op2);

    //LatticeFact icmpLt(const std::string to, const std::string op1, const std::string op2);
    LatticeFact icmpLt(const std::string to, const std::string op1
        , const int op2);
    LatticeFact icmpLt(const std::string to, const int op1
        , const std::string op2);

		// Perform the operation: to = icmp leq op1 op2
    LatticeFact icmpLeq(const std::string to, const std::string op1, int op2);
    LatticeFact icmpLeq(const std::string to, int op1, const std::string op2);

    LatticeFact icmpGeq(const std::string to, const std::string op1, int op2);
    LatticeFact icmpGeq(const std::string to, const int op1
        , const std::string op2);

    // Perform the operation: to icmp neq op1 op2
    // Return the resulting fact which is this fact updated with the result
    //
    // Note: this does not modify the internal state of this object.
    LatticeFact icmpNEq(const std::string to, int op1, const std::string op2) const;
    LatticeFact icmpNEq(const std::string to, const std::string op1, int op2);

    // Perform the operation: `to <- op + c` Note: due to the communative
    // property of addition this also does `to <- c + op` if you switch the
    // operands.
    LatticeFact add(const std::string to, const std::string op
        , const int c) const;

    // perform: to <- op1 + op2
    LatticeFact add(const std::string to, const std::string op1
        , const std::string op2) const;

    // Perform the operation: `to <- op `mod` c`.
    LatticeFact mod(const std::string to, const std::string op
        , const long int c) const;

    // Perform the operation: `to <- op * c`. Due to communitivity, this is
    // also `to <- c * op` if you just switch the operands around
    LatticeFact mul(const std::string to, const std::string op
        , const int c) const;

    // Return the interval `v` takes on in this fact. This will return NULL if
    // the global variable is not found in the map.
    ap_interval_t *getVarInterval(llvm::Value *v
        , std::map<llvm::Value *, std::string> v2n);

    // Get the value of `v` in this fact. This will crash if the operation
    // fails
    ap_interval_t *getVarInterval(std::string v);


    // Assign the passed interval to the passed variable and return the
    // resulting fact. This perform the operation on a copy of `this` so no
    // modifications are made to the underlying object
    LatticeFact assignInterval(std::string v, ap_interval_t *interv);

    // Same as first-arg string version except attempts to find the passed
    // value in the passed map. This will crash if the Value is not found in
    // the map.
    LatticeFact assignInterval(llvm::Value *v
        , ap_interval_t *interv
        , std::map<llvm::Value *, std::string> v2n);

    // Return the result of setting the passed variable to bottom.
    LatticeFact setBottom(std::string to);

    // Same as string version except looks up the string representation in the
    // passed map
    LatticeFact setBottom(llvm::Value *v
        , std::map<llvm::Value *, std::string> v2n);



    // Meet the passed constraint expression with `this` and return the result
    // NOTE: This will return a copy of `this` if it is unknown wheter consExpr
    // can be satisfied or not.
    LatticeFact meetAndFreeConstr(ap_lincons1_t *consExpr);

    LatticeFact meetEq(std::string op0, std::string op1);
    LatticeFact meetEq(std::string op0, const int cv1);
    LatticeFact meetEq(const int cv0, std::string op1);

    LatticeFact meetNEq(std::string op0, std::string op1);
    LatticeFact meetNEq(std::string op0, const int op1);
    LatticeFact meetNEq(const int op0, std::string op1);

    LatticeFact meetGeq(std::string op0, std::string op1);
    LatticeFact meetGeq(std::string op0, const int op1);
    LatticeFact meetGeq(const int op0, std::string op1);

    LatticeFact meetGt(std::string op0, std::string op1);
    LatticeFact meetGt(std::string op0, const int cv1);
    LatticeFact meetGt(const int cv0, std::string op1);

    LatticeFact meetLt(std::string op0, std::string op1);
    LatticeFact meetLt(std::string op0, const int cv1);
    LatticeFact meetLt(const int cv0, std::string op1);

    LatticeFact meetLeq(std::string op0, std::string op1);
    LatticeFact meetLeq(std::string op0, const int cv1);
    LatticeFact meetLeq(const int cv0, std::string op1);

    LatticeFact meetConstrTy(const std::string op0, const std::string op1
        , ap_constyp_t cons);
    LatticeFact meetConstrTy(const int op0, const std::string op1
        , ap_constyp_t cons);
    LatticeFact meetConstrTy(const std::string op0, const int op1
        , ap_constyp_t cons);


    // Create constraints with the two passed operands and type:
    //
    // op0 - op1 (cons) 0
    //
    // Be sure to free it afterwards!
    ap_lincons1_t createSubConstr(const std::string op0, const std::string op1
        , ap_constyp_t cons);
    ap_lincons1_t createSubConstr(const int op0, const std::string op1
        , ap_constyp_t cons);
    ap_lincons1_t createSubConstr(const std::string op0, const int op1
        , ap_constyp_t cons);

    // Create the negation of the constraint op1 (cons) op2. Be sure to free it
    // afterwards
    ap_lincons1_t negateConstr(const std::string op0, const std::string op1
        , ap_constyp_t cons);
    ap_lincons1_t negateConstr(const int op0, const std::string op1
        , ap_constyp_t cons);
    ap_lincons1_t negateConstr(const std::string op0, const int
        , ap_constyp_t cons);

    // Given the constraint type, return the coefficient values for an
    // expression. For example, is cons is (==), then the coeffecients should
    // be (1, -1). That is, the constraint will be:
    // op1 - op2 == 0
    //static std::pair<int, int> getConstrCoeffs(ap_constyp_t cons);

    // Return true if `this` satisfies the passed constraint.
    // Return false if the constraint cannot be satisfied or it is unknown.
    bool isSat(ap_lincons1_t cons);

    // Debugging function. Check if current fact is properly initialized
    bool valid() const;

    // Is this less than or equal to other?
    bool leq(const LatticeFact &other);

    ~LatticeFact();

    // Initialize the internal abstract value and manager. Like the
    // constructor,  this will deep copy the abstract value and the manager.
    //
    // If this is called on an already initialized LatticeFact it will crash
    void init(const ap_abstract1_t *av);

    // Join this lattice with the passed lattice. This will update the internal
    // state of this lattice with the result of the join.
    void factJoinDestr(const LatticeFact other);

    // Return the result of joining lf1 with lf2
    static LatticeFact factJoin(const LatticeFact lf1, const LatticeFact lf2);

    // Return the widen result of joining lf1 with lf2
    // lf1 is supposed to include lf2
    static LatticeFact factJoinWiden(const LatticeFact lf1, const LatticeFact lf2);

    // Return true if op1 == op2 within the context of this fact
    bool isEq(const std::string op1, const int op2);
    bool isEq(const int op1, const std::string op2);
    bool isNEq(const std::string op1, const int op2);
    bool isNEq(const int op1, const std::string op2);
    // Return true if op1 >= op2 within the context of this fact;
    bool isGeq(const std::string op1, const int op2);
    bool isGeq(const int op1, const std::string op2);
    // Return true if op1 <= op2 within the context of this fact
    bool isLeq(const std::string op1, const int op2);
    bool isLeq(const int op1, const std::string op2);
    // Return true if op1 > op2 within the context of this fact
    bool isGT(const std::string op1, const int op2);
    bool isGT(const int op1, const std::string op2);
    // Return true if op1 < op2 within the context of this fact
    bool isLT(const std::string op1, const int op2);
    bool isLT(const int op1, const std::string op2);

    // Is the abstract value in f1 less than or equal to f2 (i.e., is f1
    // contained in f2)
    static bool areValsLEq(LatticeFact f1, LatticeFact f2);

    // Is the abstract value in f1 greater than or equal to f2?
    static bool areValsGEq(LatticeFact f1, LatticeFact f2);

    // Is the abstract value in f1 equal to that in f2?
    static bool areValsEq(LatticeFact f1, LatticeFact f2);

    // Return true if the abstract value is bottom
    bool isBottom();

    // Get a Top lattice fact using the environment of `this`
    LatticeFact getTop();

  private:
    // Disable default constructor and assignment operator
    LatticeFact() = delete;
    // Set to true if abs_vals_ is initialized
    bool init_ = false;
    // Each fact has its own abtract representation of all the variables in the
    // program
    ap_abstract1_t abs_vals_;

    //ap_manager_t *man_ = NULL;

    // Set the internal abstract values to bottom
    void setBottom(ap_environment_t* e);
};
