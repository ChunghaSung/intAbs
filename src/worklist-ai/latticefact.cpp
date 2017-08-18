/**
 * Author: Markus Kusano
 *
 * Implementation of latticefact.h
 */
#include "latticefact.h"
#include <string>

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"
#include "../utils/mk_debug.h"
#include "utils.h"

using namespace llvm;

// Initialized in worklistai.cpp
extern cl::opt<bool> useBoxG;
extern cl::opt<bool> useOctG;
extern cl::opt<bool> usePolyG;
extern cl::opt<bool> useLinEqG;
extern cl::opt<bool> constrICmp;

extern ap_manager_t *manG;

//LatticeFact::LatticeFact(ap_environment_t* e) {
//  //man_ = Utils::getManager();
//  setBottom(e);
//}

//void LatticeFact::setBottom(ap_environment_t* e) {
//  //assert(man_ && "setBottom: unitialized manager");
//  this->abs_vals_ = ap_abstract1_bottom(manG, e);
//  init_ = true;
//}

LatticeFact::LatticeFact(ap_abstract1_t *av) {
  abs_vals_ = ap_abstract1_copy(manG, const_cast<ap_abstract1_t*>(av));
  init_ = true;
}

// Copy constructor
LatticeFact::LatticeFact(const LatticeFact &other) {
  assert(other.abs_vals_.abstract0 && "copying unintialized abstract0");
  //assert(other.man_ && "copying uninitialized LatticeFact (man_)");
  //this->init(&other.abs_vals_);
  //this->man_ = Utils::getManager();
  //if (abs_vals_.abstract0 && init_) {
  //  ap_abstract1_clear(manG, const_cast<ap_abstract1_t*>(&abs_vals_));
  //}
  this->abs_vals_ = ap_abstract1_copy(manG, const_cast<ap_abstract1_t*>(&other.abs_vals_));
  init_ = true;
}

// Move constructor
LatticeFact::LatticeFact(LatticeFact&& other) {
  // Just use copy constructor
  LatticeFact tmp(other);
  *this = tmp;
  //if (this == &other) {
  //  return;
  //}
  //if (abs_vals_.abstract0
  //    && init_) {
  //  ap_abstract1_clear(manG, &this->abs_vals_);
  //}
  //this->abs_vals_ = ap_abstract1_copy(manG, &other.abs_vals_);
  //init_ = true;
  //this->init_ = other.init_;
  //other.init_ = false;
}

LatticeFact::~LatticeFact() {
  // cleanup the memory associated with the abstract value
  if (abs_vals_.abstract0 && init_) {
    ap_abstract1_clear(manG, &abs_vals_);
    //init_ = false;
  }
  //if (man_) {
  //  ap_manager_free(man_);
  //}
}

// Move assignment
LatticeFact& LatticeFact::operator=(LatticeFact&& other) {
  //if (this == &other) {
  //  return *this;
  //}
  //if (this->abs_vals_.abstract0
  //    && init_) {
  //  ap_abstract1_clear(manG, &(this->abs_vals_));
  //}
  //this->abs_vals_ = ap_abstract1_copy(manG, &other.abs_vals_);
  //init_ = true;
  //return *this;
  std::swap(this->abs_vals_, other.abs_vals_);
  std::swap(this->init_, other.init_);
  return *this;
}

// Copy Assigment
LatticeFact& LatticeFact::operator=(const LatticeFact &other) {
  if (this == &other) {
    return *this;
  }
  ap_abstract1_t cp 
    = ap_abstract1_copy(manG, const_cast<ap_abstract1_t*>(&other.abs_vals_));
  if (this->abs_vals_.abstract0 && init_) {
    ap_abstract1_clear(manG, &(this->abs_vals_));
  }
  this->abs_vals_ = cp;
  this->init_ = other.init_;
  return *this;
  //ap_manager_free(this->man_);
  //man_ = NULL;
  //init(&other.abs_vals_);
  //return *this;
  ////LatticeFact tmp(other);
  ////*this = std::move(tmp);
  ////return *this;
  //LatticeFact tmp(other);
  //*this = std::move(tmp);
  //return *this;
}

bool LatticeFact::valid() const {
  //if (!this->abs_vals_.abstract0
  //   || !this->man_) {
  if (!this->abs_vals_.abstract0) {
    return false;
  }
  return true;
}

void LatticeFact::fprint(FILE *s) const {
  assert(this->valid() && "printing invalid fact");
  ap_abstract1_fprint(s, manG, const_cast<ap_abstract1_t*>(&abs_vals_));
}

bool LatticeFact::leq(const LatticeFact &other) {
  // Compare the abstract values
  ap_abstract1_t other_abs = other.abs_vals_;
  ap_abstract1_t this_abs = this->abs_vals_;

  return ap_abstract1_is_leq(manG, &this_abs, &other_abs);
}

// Assign a constant to a variable
LatticeFact LatticeFact::assign(const std::string to, const int from) const {
  const char *toStr = to.c_str();
  // Copy constructor
  LatticeFact ret(*this);

  ap_environment_t *env = abs_vals_.env;
  // Create an expression 
  ap_linexpr1_t expr = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, 2);
  ap_linexpr1_set_list(&expr
      , AP_CST_S_INT, from
      , AP_END);

  ret.assignLinExprAndClearDestr((ap_var_t)toStr, &expr);
  return ret;
}

void LatticeFact::assignLinExprDestr(ap_var_t var, ap_linexpr1_t *expr) { 
  abs_vals_ = ap_abstract1_assign_linexpr(manG
      , true
      , &abs_vals_
      , var
      , expr
      , NULL);
  assert(&abs_vals_ && "assignment failed");
}

void LatticeFact::assignLinExprAndClearDestr(ap_var_t var, ap_linexpr1_t *expr) { 
  assignLinExprDestr(var, expr);
  ap_linexpr1_clear(expr);
}

LatticeFact LatticeFact::assign(const std::string to
    , const std::string from) const {

  const char *toStr = to.c_str();
  const char *fromStr = from.c_str();

  ap_environment_t *env = abs_vals_.env;
  // Create an expression and a new set of linear constraints
  ap_linexpr1_t expr = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, 2);
  bool fail = ap_linexpr1_set_list(&expr
      , AP_COEFF_S_INT, 1, fromStr
      , AP_END);

  if (fail) {
    assert(0 && "error setting linear expression");
  }

  DEBUG_MSG("[DEBUG] LatticeFact::assign(): new constraints: ");
  DEBUG(ap_linexpr1_fprint(stderr, &expr););
  DEBUG_MSG("\n");

  LatticeFact ret(*this);
  ret.assignLinExprAndClearDestr((ap_var_t)toStr, &expr);

  return ret;
}

LatticeFact LatticeFact::icmpNEq(const std::string to
    , int op1, const std::string op2) const {
  assert(0 && "unimplemented");
}

LatticeFact LatticeFact::icmpNEq(const std::string to
    , const std::string op1, int op2) {
  DEBUG_MSG("[DEBUG] LatticeFact::icmpNEq: Assigning to: " << to << '\n');

  // Note: both (op1 == op2) and (op1 != op2) may be true
  bool eqSat = false;
  bool eqUnSat = false;

  if (this->isEq(op1, op2)) {
    eqSat = true;
  }
  if (this->isNEq(op1, op2)) {
    eqUnSat = true;
  }

  DEBUG_MSG("[DEBUG] icmpNEq: eqSat? " << eqSat << '\n');
  DEBUG_MSG("[DEBUG] icmpNEq: eqUnSat? " << eqUnSat << '\n');

  // If both items are false, we assume we do not know which direction the
  // branch could take. So, to over-approximate, we assume both are true
  if (!eqSat && !eqUnSat) {
    eqSat = true;
    eqUnSat = true;
  }

  // Set the result of the icmp neq to zero if unsat or one if sat. Note: these
  // both could be true.
  // If the second argument is true `to` will be set to one (we want this when
  // they are not equal (eqUnSat))
  // If the second argument is true, `to` will be set fo zero (we want this
  // when they are equan (eqSat))
  // This is backwards compared to icmpEq
  return assignTrueFalse(to, eqUnSat, eqSat);
}

bool LatticeFact::isEq(const std::string op1, const int op2) {
  if (useOctG) {
    // Octagons cannot directly check ==. Check:
    // op1 >= op2
    // AND
    // op1 <= op2
    bool geq, leq;
    geq = this->isGeq(op1, op2);
    leq = this ->isLeq(op1, op2);
    return geq && leq;
  }
  else {
    const char *op1Str = op1.c_str();
    ap_environment_t *env = abs_vals_.env;
    ap_linexpr1_t expr = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, 2);
    ap_lincons1_t cons = ap_lincons1_make(AP_CONS_EQ, &expr, NULL);
    // expr is managed by consEq

    // op1 == op2 -> op1 - op2 = 0
    ap_lincons1_set_list(&cons
                    , AP_COEFF_S_INT,1, op1Str
                    , AP_CST_S_INT, (-1 * op2)
                    , AP_END);

    DEBUG_MSG("[DEBUG] EqCons: ");
    DEBUG(ap_lincons1_fprint(stderr, &cons););
    DEBUG_MSG("\n");

    bool ret = ap_abstract1_sat_lincons(manG, &abs_vals_, &cons);
    ap_lincons1_clear(&cons);
    return ret;
  }
}

bool LatticeFact::isEq(const int op1, const std::string op2) {
  if (useOctG) {
    // Octagons cannot directly check ==. Check:
    // op1 >= op2
    // AND
    // op1 <= op2
    bool geq, leq;
    geq = this->isGeq(op1, op2);
    leq = this ->isLeq(op1, op2);
    return geq && leq;
  }
  else {
    const char *op2Str = op2.c_str();
    ap_environment_t *env = abs_vals_.env;
    ap_linexpr1_t expr = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, 2);
    ap_lincons1_t cons = ap_lincons1_make(AP_CONS_EQ, &expr, NULL);
    // expr is managed by consEq

    // op1 == op2 -> -op2 + op1 == 0
    ap_lincons1_set_list(&cons
                    , AP_COEFF_S_INT, -1, op2Str
                    , AP_CST_S_INT, op1
                    , AP_END);

    DEBUG_MSG("[DEBUG] EqCons: ");
    DEBUG(ap_lincons1_fprint(stderr, &cons););
    DEBUG_MSG("\n");

    bool ret = ap_abstract1_sat_lincons(manG, &abs_vals_, &cons);
    ap_lincons1_clear(&cons);
    return ret;
  }
}

bool LatticeFact::isGeq(const std::string op1, const int op2) {
  const char *op1Str = op1.c_str();
  ap_environment_t *env = abs_vals_.env;
  ap_linexpr1_t expr = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, 2);
  ap_lincons1_t cons = ap_lincons1_make(AP_CONS_SUPEQ, &expr, NULL);
  // expr is managed by consEq

  // op1 - op2 >= 0
  ap_lincons1_set_list(&cons
                  , AP_COEFF_S_INT,1, op1Str
                  , AP_CST_S_INT, (-1 * op2)
                  , AP_END);

  DEBUG_MSG("[DEBUG] GEqCons: ");
  DEBUG(ap_lincons1_fprint(stderr, &cons););
  DEBUG_MSG("\n");
  bool ret = ap_abstract1_sat_lincons(manG, &abs_vals_, &cons);
  ap_lincons1_clear(&cons);
  return ret;
}

bool LatticeFact::isGeq(const int op1, const std::string op2) {
  const char *op2Str = op2.c_str();
  ap_environment_t *env = abs_vals_.env;
  ap_linexpr1_t expr = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, 2);
  ap_lincons1_t cons = ap_lincons1_make(AP_CONS_SUPEQ, &expr, NULL);
  // expr is managed by consEq

  // c >= op2 <--> -op2 + c >= 0
  ap_lincons1_set_list(&cons
                  , AP_COEFF_S_INT, -1, op2Str
                  , AP_CST_S_INT, op1
                  , AP_END);

  DEBUG_MSG("[DEBUG] GEqCons: ");
  DEBUG(ap_lincons1_fprint(stderr, &cons););
  DEBUG_MSG("\n");
  bool ret = ap_abstract1_sat_lincons(manG, &abs_vals_, &cons);
  ap_lincons1_clear(&cons);
  return ret;
}

bool LatticeFact::isLeq(const std::string op1, const int op2) {
  // op1 <= op2 <--> op2 >= op1
  return isGeq(op2, op1);
}

bool LatticeFact::isLeq(const int op1, std::string op2) {
  // op1 <= op2 <--> op2 >= op1
  return isGeq(op2, op1);
}

bool LatticeFact::isNEq(const std::string op1, const int op2) {
  if (useOctG) {
    /// Octagon cannot directly test !=. Check:
    /// op1 < op2 
    /// OR
    /// op1 > op2
    bool lt = isLT(op1, op2);
    if (lt) {
      return true;
    }
    else {
      return isGT(op1, op2);
    }
  }
  else {
    const char *op1Str = op1.c_str();
    ap_environment_t *env = abs_vals_.env;
    ap_linexpr1_t expr = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, 2);
    ap_lincons1_t cons = ap_lincons1_make(AP_CONS_DISEQ, &expr, NULL);
    // expr is managed by consEq

    // op1 != op2 -> op1 - op2 != 0
    ap_lincons1_set_list(&cons
                    , AP_COEFF_S_INT,1, op1Str
                    , AP_CST_S_INT, (-1 * op2)
                    , AP_END);

    DEBUG_MSG("[DEBUG] isNeq const: ");
    DEBUG(ap_lincons1_fprint(stderr, &cons););
    DEBUG_MSG("\n");

    bool ret = ap_abstract1_sat_lincons(manG, &abs_vals_, &cons);
    ap_lincons1_clear(&cons);
    return ret;
  }
}

bool LatticeFact::isNEq(const int op1, const std::string op2) {
  if (useOctG) {
    /// Octagon cannot directly test !=. Check:
    /// op1 < op2 
    /// OR
    /// op1 > op2
    bool lt = isLT(op1, op2);
    if (lt) {
      return true;
    }
    else {
      return isGT(op1, op2);
    }
  }
  else {
    const char *op2Str = op2.c_str();
    ap_environment_t *env = abs_vals_.env;
    ap_linexpr1_t expr = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, 2);
    ap_lincons1_t cons = ap_lincons1_make(AP_CONS_DISEQ, &expr, NULL);
    // expr is managed by consEq

    // op1 != op2 -> -op2 + op1 != 0
    ap_lincons1_set_list(&cons
                    , AP_COEFF_S_INT, -1, op2Str
                    , AP_CST_S_INT, op1
                    , AP_END);

    DEBUG_MSG("[DEBUG] isNeq const: ");
    DEBUG(ap_lincons1_fprint(stderr, &cons););
    DEBUG_MSG("\n");

    bool ret = ap_abstract1_sat_lincons(manG, &abs_vals_, &cons);
    ap_lincons1_clear(&cons);
    return ret;
  }
}

bool LatticeFact::isGT(const std::string op1, const int op2) {
  const char *op1Str = op1.c_str();
  ap_environment_t *env = abs_vals_.env;
  ap_linexpr1_t expr = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, 2);
  ap_lincons1_t cons = ap_lincons1_make(AP_CONS_SUP, &expr, NULL);
  // expr is managed by consEq

  // op1 > op2 <--> op1 - op2 > 0
  ap_lincons1_set_list(&cons
                  , AP_COEFF_S_INT,1, op1Str
                  , AP_CST_S_INT, (-1 * op2)
                  , AP_END);

  DEBUG_MSG("[DEBUG] isGT cons: ");
  DEBUG(ap_lincons1_fprint(stderr, &cons););
  DEBUG_MSG("\n");

  bool ret = ap_abstract1_sat_lincons(manG, &abs_vals_, &cons);
  ap_lincons1_clear(&cons);
  return ret;
}

bool LatticeFact::isGT(const int op1, const std::string op2) {
  const char *op2Str = op2.c_str();
  ap_environment_t *env = abs_vals_.env;
  ap_linexpr1_t expr = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, 2);
  ap_lincons1_t cons = ap_lincons1_make(AP_CONS_SUP, &expr, NULL);
  // expr is managed by consEq

  // op1 > op2 <--> op1 - op2 > 0
  // <--> -op2 + op1 > 0
  ap_lincons1_set_list(&cons
                  , AP_COEFF_S_INT, -1, op2Str
                  , AP_CST_S_INT, op1
                  , AP_END);

  DEBUG_MSG("[DEBUG] isGT cons: ");
  DEBUG(ap_lincons1_fprint(stderr, &cons););
  DEBUG_MSG("\n");

  bool ret = ap_abstract1_sat_lincons(manG, &abs_vals_, &cons);
  ap_lincons1_clear(&cons);
  return ret;
}

bool LatticeFact::isLT(const std::string op1, const int op2) {
  // op1 < op2 <--> op2 > op1
  return this->isGT(op2, op1);
}

bool LatticeFact::isLT(const int op1, const std::string op2) {
  // op1 < op2 <--> op2 > op1
  return this->isGT(op2, op1);
}

LatticeFact LatticeFact::add(const std::string to
    , std::string op
    , int c) const {
  // To be more accurate, this should really perform the add operation modulo
  // the bitwidth (see mod(...))
  const char *opStr = op.c_str();
  const char *toStr = to.c_str();
  LatticeFact ret(*this);
  ap_environment_t *env = abs_vals_.env;
  ap_linexpr1_t expr = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, 1);
  ap_linexpr1_set_list(&expr,
                      AP_COEFF_S_INT, 1, opStr
                      , AP_CST_S_INT, c
                      , AP_END);
  DEBUG_MSG("[DEBUG] LatticeFact::add(): expression: ");
  DEBUG(ap_linexpr1_fprint(stderr, &expr););
  DEBUG_MSG('\n');
  ret.assignLinExprAndClearDestr((ap_var_t)toStr, &expr);
  return ret;
}

LatticeFact LatticeFact::add(const std::string to, const std::string op1
    , const std::string op2) const {
  LatticeFact ret(*this);
  ap_environment_t *env = abs_vals_.env;
  ap_texpr1_t *op1Expr = ap_texpr1_var(env, (ap_var_t)op1.c_str());
  ap_texpr1_t *op2Expr = ap_texpr1_var(env, (ap_var_t)op2.c_str());

  // op1 and op2 memory manged by res
  ap_texpr1_t *res = ap_texpr1_binop(AP_TEXPR_ADD
                        , op1Expr
                        , op2Expr
                        , AP_RTYPE_INT /* Integer result */
                        , AP_RDIR_ZERO); /* Round toward zero */

  // Assgin the expression to `to`
  ret.abs_vals_ = ap_abstract1_assign_texpr(manG
                                            , true
                                            , &(ret.abs_vals_)
                                            , (ap_var_t)to.c_str()
                                            , res
                                            , NULL);
  // Free the tree expression (recursively)
  ap_texpr1_free(res);
  return ret;
}

LatticeFact LatticeFact::mod(const std::string to, const std::string op
        , const long int c) const {
  LatticeFact ret(*this);
  ap_environment_t *env = abs_vals_.env;
  ap_texpr1_t *opExpr = ap_texpr1_var(env, (ap_var_t)op.c_str());
  ap_texpr1_t *cst = ap_texpr1_cst_scalar_int(env, c);
  // Note: op1 and cst are memory managed by res!
  ap_texpr1_t *res = ap_texpr1_binop(AP_TEXPR_MOD
                        , opExpr
                        , cst
                        , AP_RTYPE_INT /* Integer result */
                        , AP_RDIR_ZERO); /* Round toward zero */
  // Assgin the expression to `to`
  ret.abs_vals_ = ap_abstract1_assign_texpr(manG
                                            , true
                                            , &(ret.abs_vals_)
                                            , (ap_var_t)to.c_str()
                                            , res
                                            , NULL);
  // Free the tree expression (recursively)
  ap_texpr1_free(res);
  return ret;
}

LatticeFact LatticeFact::mul(const std::string to
    , std::string op
    , int c) const {
  LatticeFact ret(*this);
  ap_environment_t *env = abs_vals_.env;
  // Use tree expression to perform multiplication
  // Create the constant
  ap_texpr1_t *cstVal = ap_texpr1_cst_scalar_int(env, c);
  assert(cstVal && "NULL constant in mutliplication");

  // Create the variable
  ap_texpr1_t *val = ap_texpr1_var(env, (ap_var_t)op.c_str());
  assert(val && "NULL non-constant in multiplication");

  // Create the multiplication expression
  ap_texpr1_t *exp = ap_texpr1_binop(AP_TEXPR_MUL
                                  , cstVal
                                  , val
                                  , AP_RTYPE_INT // integer return type
                                  , AP_RDIR_ZERO); // round toward zero
  assert(exp && "NULL multiplication expression");

  ret.abs_vals_ = ap_abstract1_assign_texpr(manG
                                            , true
                                            , &(ret.abs_vals_)
                                            , (ap_var_t)to.c_str()
                                            , exp
                                            , NULL);

  // free ap_texpr1_t, all of the child nodes in the tree are memory managed by
  // the parent
  ap_texpr1_free(exp);
  return ret;
}

LatticeFact LatticeFact::icmpEq(const std::string to
    , int op1
    , const std::string op2) {
  bool areEq = isEq(op1, op2);
  bool areNotEq = isNEq(op1, op2);

  // Both being false implies we may not know if they are equal or not. So,
  // over approximate and assume they both are true.
  if (!areEq && !areNotEq) {
    areEq = true;
    areNotEq = true;
  }
  return assignTrueFalse(to, areEq, areNotEq);
}


LatticeFact LatticeFact::icmpEq(const std::string to
    , const std::string op1
    , int op2) {
  DEBUG_MSG("[DEBUG] LatticeFact::icmpEq: Assigning to: " << to << '\n');

  // Note: both (op1 == op2) and (op1 != op2) may be true
  bool eqSat = false;
  bool eqUnSat = false;

  if (this->isEq(op1, op2)) {
    eqSat = true;
  }
  if (this->isNEq(op1, op2)) {
    eqUnSat = true;
  }


  DEBUG_MSG("[DEBUG] icmpEq: eqSat? " << eqSat << '\n');
  DEBUG_MSG("[DEBUG] icmpEq: eqUnSat? " << eqUnSat << '\n');

  // When isEq or isNEq, since they use the apron solvers, returns false this
  // means either "false" or "don't know". True definitely means true. So, if
  // both return true, the answer is sound. If one returns true and one returns
  // false, then we know the one returning false is definitely not true. But,
  // if both are false then we do not know anything. In this case, we should
  // probably assume both are true and continue the analysis
  // (over-approximate).
  //
  // If both items are false, we assume we do not know which direction the
  // branch could take. So, to over-approximate, we assume both are true
  if (!eqSat && !eqUnSat) {
    eqSat = true;
    eqUnSat = true;
  }

  // Set the result of the icmpeq to zero if unsat or one if sat. Note: these
  // both could be true
  return assignTrueFalse(to, eqSat, eqUnSat);
}

LatticeFact LatticeFact::icmpGt(const std::string to
    , const std::string op1
    , int op2) {
  bool areGt = isGT(op1, op2); // if true, set `to` to one
  bool areLeq = isLeq(op1, op2); // if true, set `to` to zero

  // Both can be set to false if it is unknown whether op1 > op2. In this case,
  // we overapproximate and assume they both could be true
  if (!areGt && !areLeq) {
    areGt = true;
    areLeq = true;
  }

  return assignTrueFalse(to, areGt, areLeq);
}

LatticeFact LatticeFact::icmpGt(const std::string to
		, int op1
		, const std::string op2) {
  bool areGt = isGT(op1, op2); // if true, set `to` to one
  bool areLeq = isLeq(op1, op2); // if true, set `to` to zero

  // Both can be set to false if it is unknown whether op1 > op2. In this case,
  // we overapproximate and assume they both could be true
  if (!areGt && !areLeq) {
    areGt = true;
    areLeq = true;
  }

  return assignTrueFalse(to, areGt, areLeq);
}

LatticeFact LatticeFact::icmpLeq(const std::string to
		, const std::string op1
		, int op2) {
	bool areLeq = isLeq(op1, op2);
	bool areGt = isGT(op1 ,op2);

	if (!areLeq && !areGt) {
		areLeq = true;
		areGt = true;
	}

	return assignTrueFalse(to, areLeq, areGt);
}

LatticeFact LatticeFact::icmpLeq(const std::string to
		, int op1
		, const std::string op2) {
	bool areLeq = isLeq(op1, op2);
	bool areGt = isGT(op1 ,op2);

	if (!areLeq && !areGt) {
		areLeq = true;
		areGt = true;
	}

	return assignTrueFalse(to, areLeq, areGt);
}

LatticeFact LatticeFact::icmpLt(const std::string to
                              , const std::string op1
                              , const int op2) {
  bool areLT = isLT(op1, op2);
  bool areGeq = isGeq(op1, op2);

  // Both can be false when it is unknown whether it is LT or GEQ so
  // over-approximate and assume both could be true
  if (!areLT && !areGeq) {
    areLT = true;
    areGeq = true;
  }
  return assignTrueFalse(to, areLT, areGeq);
}

LatticeFact LatticeFact::icmpLt(const std::string to
                              , const int op1
                              , const std::string op2) {
  bool areLT = isLT(op1, op2);
  bool areGeq = isGeq(op1, op2);

  // Both can be false when it is unknown whether it is LT or GEQ so
  // over-approximate and assume both could be true
  if (!areLT && !areGeq) {
    areLT = true;
    areGeq = true;
  }
  return assignTrueFalse(to, areLT, areGeq);
}

LatticeFact LatticeFact::icmpGeq(const std::string to, const std::string op1
    , int op2) {
  bool areGeq = isGeq(op1, op2);
  bool areLT = isLT(op1, op2);
  
  // Both being false means we do not no so overapproximate and assume both are
  // true
  if (!areGeq && !areLT) {
    areGeq = true;
    areLT = true;
  }
  return assignTrueFalse(to, areGeq, areLT);
}


LatticeFact LatticeFact::icmpGeq(const std::string to, const int op1
    , const std::string op2) {
  bool areGeq = isGeq(op1, op2);
  bool areLT = isLT(op1, op2);
  
  // Both being false means we do not no so overapproximate and assume both are
  // true
  if (!areGeq && !areLT) {
    areGeq = true;
    areLT = true;
  }
  return assignTrueFalse(to, areGeq, areLT);
}

LatticeFact LatticeFact::setBottom(std::string to) {
  ap_interval_t *botVal = ap_interval_alloc();
  ap_interval_set_bottom(botVal);
  LatticeFact ret = this->assignInterval(to, botVal);
  ap_interval_free(botVal);
  return ret;
}


LatticeFact LatticeFact::setBottom(Value *v
    , std::map<Value *, std::string> v2n) {
  ap_interval_t *botVal = ap_interval_alloc();
  ap_interval_set_bottom(botVal);
  LatticeFact ret = this->assignInterval(v, botVal, v2n);
  ap_interval_free(botVal);
  return ret;
}

LatticeFact LatticeFact::getTop() {
  ap_manager_t* man = Utils::getManager();
  ap_environment_t *env = abs_vals_.env;
  ap_abstract1_t factTop = ap_abstract1_top(man, env);
  return LatticeFact(&factTop);
}

LatticeFact LatticeFact::assignTrueFalse(std::string to, bool setTrue, bool setFalse) {
  if (setTrue && setFalse) {
    ap_interval_t *tfInt = ap_interval_alloc();
    ap_interval_set_int(tfInt, 0, 1);
    LatticeFact ret = assignInterval(to, tfInt);
    ap_interval_free(tfInt);
    return ret;
  }
  else if (setTrue) {
    return assign(to, 1);
  }
  else if (setFalse) {
    return assign(to, 0);
  }
  else {
    assert(0 && "neither set-true nor set-false");
  }
}

void LatticeFact::assignTexprDestr(ap_var_t var, ap_texpr1_t *texpr) {
  abs_vals_ = ap_abstract1_assign_texpr(manG, true, &abs_vals_, var, texpr
      , NULL);
  assert(&abs_vals_ && "assign texpr failed");
}
void LatticeFact::assignTexprAndClearDestr(ap_var_t var, ap_texpr1_t *cons) {
  assignTexprDestr(var, cons);
  ap_texpr1_free(cons);
}

void LatticeFact::factJoinDestr(LatticeFact other) {
  // NOTE!!!: Some callers of this function intend it NOT to widen. Do not
  // widen here!
  abs_vals_ = ap_abstract1_join(manG, true, &abs_vals_, &other.abs_vals_);
}

LatticeFact LatticeFact::factJoin(const LatticeFact lf1, const LatticeFact lf2) {
  // NOTE!!!: Some callers of this function intend it NOT to widen. Do not
  // widen here!
  ap_abstract1_t n = ap_abstract1_join(manG, false
      , const_cast<ap_abstract1_t*>(&lf1.abs_vals_)
      , const_cast<ap_abstract1_t*>(&lf2.abs_vals_));
  LatticeFact ret(&n);
  //ap_abstract1_clear(lf1.man_, &n);
  ap_abstract1_clear(manG, &n);
  return ret;
}

LatticeFact LatticeFact::factJoinWiden(const LatticeFact lf1, const LatticeFact lf2) {
  // NOTE!!!: Some callers of this function intend it NOT to widen. Do not
  // widen here!
  ap_abstract1_t n = ap_abstract1_widening(manG 
      , const_cast<ap_abstract1_t*>(&lf1.abs_vals_)
      , const_cast<ap_abstract1_t*>(&lf2.abs_vals_));
  LatticeFact ret(&n);
  //ap_abstract1_clear(lf1.man_, &n);
  ap_abstract1_clear(manG, &n);
  return ret;
}


LatticeFact LatticeFact::constrainTF(Value *v, const bool tf
    , const std::map<Value*, std::string> v2n) {
  std::string vstr = Utils::valueToStringUnsafe(v2n, v);
  // If v is the result of an ICmpInst then it is actually the calculation of
  // some pedicate. We can further constrain the fact with the predicate for
  // more acurracy.
  if (constrICmp) {
    if (ICmpInst *ic = dyn_cast<ICmpInst>(v)) {
      DEBUG_MSG("[DEBUG] constrainICmp: before fact:\n");
      DEBUG(this->fprint(stderr););
      DEBUG_MSG('\n');
      DEBUG_MSG("True or False? " << (tf  ? "true" : "false") << '\n');
      LatticeFact ret = constrainICmp(ic, tf, v2n);
      DEBUG_MSG("after\n");
      DEBUG(ret.fprint(stderr););
      DEBUG_MSG('\n');
      // Also constrain the actual return value of the icmp instruction
      ret = ret.constrainTF(vstr, tf);
      DEBUG_MSG("After constraining return:\n");
      DEBUG(ret.fprint(stderr););
      return ret;
    }
  }
  // Here, we are unable to resolve `v` into something more meaningful other
  // than a boolean value. Simply constrain the value of `v` within the fact to
  // be either true/false
  return constrainTF(vstr, tf);
}

LatticeFact LatticeFact::constrainTF(std::string b, const bool tf) {
  int subVal;
  if (tf) {
    // b - 1 == 0
    subVal = 1;
  }
  else {
    // b - 0 == 0
    subVal = 0;
  }
  return meetEq(b, subVal);
  //int subVal;
  //if (tf) {
  //  // b - 1 == 0
  //  subVal = 1;
  //}
  //else {
  //  // b - 0 == 0
  //  subVal = 0;
  //}
  //ap_lincons1_t cons = createSubConstr(b, subVal, AP_CONS_EQ);

  //DEBUG_MSG("[DEBUG] constrainTF: constraint: ");
  //DEBUG(ap_lincons1_fprint(stderr, &cons););
  //DEBUG_MSG('\n');

  //LatticeFact ret = meetAndFreeConstr(&cons);
  //DEBUG_MSG("Result: ");
  //DEBUG(ret.fprint(stderr););
  //DEBUG_MSG('\n');

  //if (ret.isBottom()) {
  //  DEBUG_MSG("\tret is bottom\n");
  //  cons = createSubConstr(b, subVal, AP_CONS_EQ);
  //  ap_lincons1_t negCons = createSubConstr(b, subVal, AP_CONS_DISEQ);
  //  DEBUG_MSG("\tneg cons: ");
  //  DEBUG(ap_lincons1_fprint(stderr, &negCons););
  //  DEBUG_MSG('\n');
  //  // Ensure that it is not unknown whether the constraint can be satisfied
  //  if (!isSat(negCons) && !isSat(cons)) {
  //    DEBUG_MSG("\tneither constraint is sat");
  //    // Return a copy of this
  //    ret = *this;
  //  }
  //  ap_lincons1_clear(&cons);
  //  ap_lincons1_clear(&negCons);
  //}
  //return ret;
}

LatticeFact LatticeFact::constrainICmp(ICmpInst *ic, const bool tf
    , const std::map<Value*, std::string> v2n) {
  std::string icStr = Utils::valueToStringUnsafe(v2n, ic);
  CmpInst::Predicate pred = ic->getPredicate();
  if (pred == llvm::CmpInst::ICMP_NE) {
    if (!tf) {
      // the result of the icmp ne was false, i.e., 
      // !(op1 != op2)
      // ==> op1 == op2
      return constrainFact(ic->getOperand(0), ic->getOperand(1), AP_CONS_EQ
          , v2n);
    }
    else {
      // the result of the icmp ne was true,
      // op1 != op2
      return constrainFact(ic->getOperand(0), ic->getOperand(1)
          , AP_CONS_DISEQ, v2n);
    }
  }
  else if (pred == llvm::CmpInst::ICMP_EQ) {
    if (!tf) {
      // !(op1 == op2) ==> op1 != op2
      return constrainFact(ic->getOperand(0), ic->getOperand(1)
          , AP_CONS_DISEQ, v2n);
    }
    else {
      // op1 == op2
      return constrainFact(ic->getOperand(0), ic->getOperand(1)
          , AP_CONS_EQ, v2n);
    }
  }
  else if ((pred == llvm::CmpInst::ICMP_UGT)
        || (pred == llvm::CmpInst::ICMP_SGT)) {
    // We ignore signed/unsigned values: TODO: probably shouldn't do this. It
    // would require a bit vector aware numerical abstract domain.
    if (!tf) {
      // !(op0 > op1) ==> (op0 <= op1) ==> (op1 >= op0)
      return constrainFact(ic->getOperand(1), ic->getOperand(0)
          , AP_CONS_SUPEQ, v2n);
    }
    else {
      // op0 > op1
      return constrainFact(ic->getOperand(0), ic->getOperand(1)
          , AP_CONS_SUP, v2n);
    }
  }
  else if ((pred == llvm::CmpInst::ICMP_ULT)
        || (pred == llvm::CmpInst::ICMP_SLT)) {
    if (!tf) {
      // !(op0 < op1) ==> (op0 >= op1)
      return constrainFact(ic->getOperand(0), ic->getOperand(1)
          , AP_CONS_SUPEQ, v2n);
    }
    else {
      // op0 < op1 ==> op1 > op0
      return constrainFact(ic->getOperand(1), ic->getOperand(0)
          , AP_CONS_SUP, v2n);
    }
  }
  else if ((pred == llvm::CmpInst::ICMP_UGE)
        || (pred == llvm::CmpInst::ICMP_SGE)) {
    if (!tf) {
      // !(op0 >= op1) ==> (op0 < op1) ==> (op1 > op0)
      return constrainFact(ic->getOperand(1), ic->getOperand(0)
          , AP_CONS_SUP, v2n);
    }
    else {
      // op0 >= op1
      return constrainFact(ic->getOperand(0), ic->getOperand(1)
          , AP_CONS_SUPEQ, v2n);
    }
  }
  else if ((pred == llvm::CmpInst::ICMP_ULE)
        || (pred == llvm::CmpInst::ICMP_SLE)) {
    if (!tf) {
      // !(op0 <= op1) ==> (op0 > op1)
      return constrainFact(ic->getOperand(0), ic->getOperand(1)
          , AP_CONS_SUP, v2n);
    }
    else {
      // op0 <= op1 ==> op1 >= op0
      return constrainFact(ic->getOperand(1), ic->getOperand(0)
          , AP_CONS_SUPEQ, v2n);
    }
  }
  else {
    // This should be unreachable
    errs() << "[ERROR] Constraining unhandled ICmpInst: " << *ic << '\n';
    assert(0 && "see above");
    exit(EXIT_FAILURE);
  }
}

LatticeFact LatticeFact::constrainFact(Value *op0
    , Value *op1, ap_constyp_t cons, const std::map<Value *, std::string> v2n) {
  assert(!(isa<ConstantInt>(op0) && isa<ConstantInt>(op1))
      && "constraining with two constants");
  if (ConstantInt *ci0 = dyn_cast<ConstantInt>(op0)) {
    // Since both operands are non-constantints, op1 is not a constant int
    int val0 = Utils::getConstantIntUnsafe(ci0);
    std::string op1Str = Utils::valueToStringUnsafe(v2n, op1);
    return constrainFact(val0, op1Str, cons);
  }
  else if (ConstantInt *ci1 = dyn_cast<ConstantInt>(op1)) {
    int val1 = Utils::getConstantIntUnsafe(ci1);
    std::string op0Str = Utils::valueToStringUnsafe(v2n, op0);
    return constrainFact(op0Str, val1, cons);
  }
  else {
    // both are not ConstantInts
    std::string op0Str = Utils::valueToStringUnsafe(v2n, op0);
    std::string op1Str = Utils::valueToStringUnsafe(v2n, op1);
    return constrainFact(op0Str, op1Str, cons);
  }
}

LatticeFact LatticeFact::constrainFact(std::string op0, const int cv1
    , ap_constyp_t cons) {
  return meetConstrTy(op0, cv1, cons);
  //ap_lincons1_t consExpr = createSubConstr(op0, cv1, cons);
  //LatticeFact ret = meetAndFreeConstr(&consExpr);
  //if (ret.isBottom()) {
  //  DEBUG_MSG("[DEBUG] constrainFact: return is bottom\n");
  //  ap_lincons1_t negCons = negateConstr(op0, cv1, cons);
  //  ap_lincons1_t consExpr = createSubConstr(op0, cv1, cons);
  //  DEBUG_MSG("Constraint: ");
  //  DEBUG(ap_lincons1_fprint(stderr, &consExpr););
  //  DEBUG_MSG("\nNegation: ");
  //  DEBUG(ap_lincons1_fprint(stderr, &negCons););
  //  DEBUG_MSG('\n');
  //  // Ensure that it is not unknown whether the constraint can be satisfied
  //  if (!(isSat(negCons) || isSat(consExpr))) {
  //    // Return a copy of this
  //    ret = *this;
  //  }
  //  DEBUG_MSG("[DEBUG] one constraint is sat\n");
  //  ap_lincons1_clear(&consExpr);
  //  ap_lincons1_clear(&negCons);
  //}
  //return ret;
}

LatticeFact LatticeFact::constrainFact(const int cv0, std::string op1
    , ap_constyp_t cons) {
  return meetConstrTy(cv0, op1, cons);
  //ap_lincons1_t consExpr = createSubConstr(cv0, op1, cons);
  //LatticeFact ret = meetAndFreeConstr(&consExpr);
  //if (ret.isBottom()) {
  //  ap_lincons1_t consExpr = createSubConstr(cv0, op1, cons);
  //  ap_lincons1_t negCons = negateConstr(cv0, op1, cons);
  //  // Ensure that it is not unknown whether the constraint can be satisfied
  //  if (!isSat(negCons) && !isSat(consExpr)) {
  //    // Return a copy of this
  //    ret = *this;
  //  }
  //  ap_lincons1_clear(&consExpr);
  //  ap_lincons1_clear(&negCons);
  //}
  //return ret;
}

LatticeFact LatticeFact::constrainFact(const std::string op0
    , const std::string op1
    , ap_constyp_t cons) {
  return meetConstrTy(op0, op1, cons);
  //ap_lincons1_t consExpr = createSubConstr(op0, op1, cons);
  //LatticeFact ret = meetAndFreeConstr(&consExpr);
  //if (ret.isBottom()) {
  //  ap_lincons1_t negCons = negateConstr(op0, op1, cons);
  //  ap_lincons1_t consExpr = createSubConstr(op0, op1, cons);
  //  // Ensure that it is not unknown whether the constraint can be satisfied
  //  if (!isSat(negCons) && !isSat(consExpr)) {
  //    // Return a copy of this
  //    ret = *this;
  //  }
  //  ap_lincons1_clear(&consExpr);
  //  ap_lincons1_clear(&negCons);
  //}
  //return ret;
}

ap_lincons1_t LatticeFact::negateConstr(const std::string op0
    , const std::string op1
    , ap_constyp_t cons) {
  if (cons == AP_CONS_EQ) {
    // !(op0 == op1) <==> op0 != op1
    return createSubConstr(op0, op1, AP_CONS_DISEQ);
  }
  else if (cons == AP_CONS_SUPEQ) {
    // !(op0 >= op1) <==> op0 < op1 <==> op1 > op0
    return createSubConstr(op1, op0, AP_CONS_SUP);
  }
  else if (cons == AP_CONS_SUP) {
    // !(op0 > op1) <==> op0 <= op1 <==> op1 >= op0
    return createSubConstr(op1, op0, AP_CONS_SUPEQ);
  }
  else if (cons == AP_CONS_EQMOD) {
    assert(0 && "eqmod unhandled");
  }
  else if (cons == AP_CONS_DISEQ) {
    return createSubConstr(op1, op0, AP_CONS_EQ);
  }
  else {
    assert(0 && "unreachable");
  }
}

ap_lincons1_t LatticeFact::negateConstr(const std::string op0
    , const int op1
    , ap_constyp_t cons) {
  if (cons == AP_CONS_EQ) {
    // !(op0 == op1) <==> op0 != op1
    return createSubConstr(op0, op1, AP_CONS_DISEQ);
  }
  else if (cons == AP_CONS_SUPEQ) {
    // !(op0 >= op1) <==> op0 < op1 <==> op1 > op0
    return createSubConstr(op1, op0, AP_CONS_SUP);
  }
  else if (cons == AP_CONS_SUP) {
    // !(op0 > op1) <==> op0 <= op1 <==> op1 >= op0
    return createSubConstr(op1, op0, AP_CONS_SUPEQ);
  }
  else if (cons == AP_CONS_EQMOD) {
    assert(0 && "eqmod unhandled");
  }
  else if (cons == AP_CONS_DISEQ) {
    return createSubConstr(op1, op0, AP_CONS_EQ);
  }
  else {
    assert(0 && "unreachable");
  }
}

ap_lincons1_t LatticeFact::negateConstr(const int op0
    , const std::string op1
    , ap_constyp_t cons) {
  if (cons == AP_CONS_EQ) {
    // !(op0 == op1) <==> op0 != op1
    return createSubConstr(op0, op1, AP_CONS_DISEQ);
  }
  else if (cons == AP_CONS_SUPEQ) {
    // !(op0 >= op1) <==> op0 < op1 <==> op1 > op0
    return createSubConstr(op1, op0, AP_CONS_SUP);
  }
  else if (cons == AP_CONS_SUP) {
    // !(op0 > op1) <==> op0 <= op1 <==> op1 >= op0
    return createSubConstr(op1, op0, AP_CONS_SUPEQ);
  }
  else if (cons == AP_CONS_EQMOD) {
    assert(0 && "eqmod unhandled");
  }
  else if (cons == AP_CONS_DISEQ) {
    return createSubConstr(op1, op0, AP_CONS_EQ);
  }
  else {
    assert(0 && "unreachable");
  }
}

//std::pair<int, int> LatticeFact::getConstrCoeffs(ap_constyp_t cons) {
//  if (cons == AP_CONS_EQ) {
//    // op0 - op1 == 0
//    return std::make_pair(1, -1);
//  }
//  else if (cons == AP_CONS_SUPEQ) {
//    // op0 
//    return std::make_pair(1, -1);
//  }
//  else if (cons == AP_CONS_SUP) {
//  }
//  else if (cons == AP_CONS_EQMOD) {
//    assert(0 && "eqmod unhandled");
//  }
//  else if (cons == AP_CONS_DISEQ) {
//  }
//  else {
//    assert(0 && "unreachable");
//  }
//}

ap_lincons1_t LatticeFact::createSubConstr(const std::string op0
    , const std::string op1, ap_constyp_t cons) {
    ap_environment_t *env = abs_vals_.env;
    ap_linexpr1_t expr = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, 2);
    ap_lincons1_t consExpr = ap_lincons1_make(cons, &expr, NULL);
    ap_lincons1_set_list(&consExpr
                    , AP_COEFF_S_INT, 1, (ap_var_t) op0.c_str()
                    , AP_COEFF_S_INT, -1, (ap_var_t) op1.c_str()
                    , AP_END);
    return consExpr;
}

ap_lincons1_t LatticeFact::createSubConstr(const int op0, const std::string op1
    , ap_constyp_t cons) {
    ap_environment_t *env = abs_vals_.env;
    ap_linexpr1_t expr = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, 2);
    ap_lincons1_t consExpr = ap_lincons1_make(cons, &expr, NULL);
    ap_lincons1_set_list(&consExpr
                    , AP_CST_S_INT, op0
                    , AP_COEFF_S_INT, -1, (ap_var_t) op1.c_str()
                    , AP_END);
    return consExpr;
}
ap_lincons1_t LatticeFact::createSubConstr(const std::string op0
    , const int op1
    , ap_constyp_t cons) {
    ap_environment_t *env = abs_vals_.env;
    ap_linexpr1_t expr = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, 2);
    ap_lincons1_t consExpr = ap_lincons1_make(cons, &expr, NULL);
    ap_lincons1_set_list(&consExpr
                    , AP_COEFF_S_INT, 1, (ap_var_t) op0.c_str()
                    , AP_CST_S_INT, (-1 * op1)
                    , AP_END);
    return consExpr;
}

LatticeFact LatticeFact::meetAndFreeConstr(ap_lincons1_t *consExpr) {
  ap_environment_t *env = abs_vals_.env;
  // Put the constraints into an array and turn it into an abstract value
  ap_lincons1_array_t array = ap_lincons1_array_make(env, 1);
  ap_lincons1_array_set(&array, 0, consExpr);
  //ap_abstract1_t temp = ap_abstract1_of_lincons_array(manG, env, &array);
  ap_abstract1_t meetVals = ap_abstract1_meet_lincons_array(manG, false
      , &abs_vals_, &array);

  ///ap_abstract1_t meetVals = ap_abstract1_meet(manG, false, &abs_vals_, &temp);
  LatticeFact ret(&meetVals);

  // Free the array: this also frees the internal constraints
  ap_lincons1_array_clear(&array);
  // Free the two temporary abstract values
  //ap_abstract1_clear(manG, &temp);
  ap_abstract1_clear(manG, &meetVals);
  return ret;
}

LatticeFact LatticeFact::meetEq(std::string op0, const int cv1) {
  if (useOctG) {
    // Octagons cannot directly test equality
    LatticeFact ret = this->meetLeq(op0, cv1);
    return ret.meetGeq(op0, cv1);
  }
  // For non-octagons, just directly apply the constraint
  ap_lincons1_t cons = createSubConstr(op0, cv1, AP_CONS_EQ);
  return meetAndFreeConstr(&cons);
}

LatticeFact LatticeFact::meetEq(std::string op0, std::string op1) {
  if (useOctG) {
    // Octagons cannot directly test equality
    LatticeFact ret = this->meetLeq(op0, op1);
    return ret.meetGeq(op0, op1);
  }
  // For non-octagons, just directly apply the constraint
  ap_lincons1_t cons = createSubConstr(op0, op1, AP_CONS_EQ);
  return meetAndFreeConstr(&cons);
}

LatticeFact LatticeFact::meetEq(const int cv0, std::string op1) {
  if (useOctG) {
    // Octagons cannot directly test equality. Meet on >= && <=
    LatticeFact ret = this->meetLeq(cv0, op1);
    return ret.meetGeq(cv0, op1);
  }
  // For non-octagons, just directly apply the constraint
  ap_lincons1_t cons = createSubConstr(cv0, op1, AP_CONS_EQ);
  return meetAndFreeConstr(&cons);
}


LatticeFact LatticeFact::meetNEq(std::string op0, std::string op1) {
  if (useOctG) {
    // Octagons cannot directly meet disequality. Join the results of meeting <
    // or >
    LatticeFact lt = this->meetLt(op0, op1);
    LatticeFact gt = this->meetGt(op0, op1);
    return LatticeFact::factJoin(lt, gt);
  }
  ap_lincons1_t cons = createSubConstr(op0, op1, AP_CONS_DISEQ);
  return meetAndFreeConstr(&cons);
}

LatticeFact LatticeFact::meetNEq(std::string op0, const int op1) {
  if (useOctG) {
    // Octagons cannot directly meet disequality. Join the results of meeting <
    // or >
    LatticeFact lt = this->meetLt(op0, op1);
    LatticeFact gt = this->meetGt(op0, op1);
    return LatticeFact::factJoin(lt, gt);
  }
  ap_lincons1_t cons = createSubConstr(op0, op1, AP_CONS_DISEQ);
  return meetAndFreeConstr(&cons);
}

LatticeFact LatticeFact::meetNEq(const int op0, std::string op1) {
  if (useOctG) {
    // Octagons cannot directly meet disequality. Join the results of meeting <
    // or >
    LatticeFact lt = this->meetLt(op0, op1);
    LatticeFact gt = this->meetGt(op0, op1);
    return LatticeFact::factJoin(lt, gt);
  }
  ap_lincons1_t cons = createSubConstr(op0, op1, AP_CONS_DISEQ);
  return meetAndFreeConstr(&cons);
}

LatticeFact LatticeFact::meetGeq(std::string op0, std::string op1) {
  ap_lincons1_t cons = createSubConstr(op0, op1, AP_CONS_SUPEQ);
  return meetAndFreeConstr(&cons);
}

LatticeFact LatticeFact::meetGeq(std::string op0, const int op1) {
  ap_lincons1_t cons = createSubConstr(op0, op1, AP_CONS_SUPEQ);
  return meetAndFreeConstr(&cons);
}

LatticeFact LatticeFact::meetGeq(const int op0, std::string op1) {
  ap_lincons1_t cons = createSubConstr(op0, op1, AP_CONS_SUPEQ);
  return meetAndFreeConstr(&cons);
}

LatticeFact LatticeFact::meetGt(std::string op0, std::string op1) {
  ap_lincons1_t cons = createSubConstr(op0, op1, AP_CONS_SUP);
  return meetAndFreeConstr(&cons);
}

LatticeFact LatticeFact::meetGt(std::string op0, const int op1) {
  ap_lincons1_t cons = createSubConstr(op0, op1, AP_CONS_SUP);
  return meetAndFreeConstr(&cons);
}

LatticeFact LatticeFact::meetGt(const int op0, std::string op1) {
  ap_lincons1_t cons = createSubConstr(op0, op1, AP_CONS_SUP);
  return meetAndFreeConstr(&cons);
}

LatticeFact LatticeFact::meetLt(std::string op0, std::string op1) {
  // op0 < op1 <==> op1 > op0
  ap_lincons1_t cons = createSubConstr(op1, op0, AP_CONS_SUP);
  return meetAndFreeConstr(&cons);
}

LatticeFact LatticeFact::meetLt(std::string op0, const int op1) {
  // op0 < op1 <==> op1 > op0
  ap_lincons1_t cons = createSubConstr(op1, op0, AP_CONS_SUP);
  return meetAndFreeConstr(&cons);
}

LatticeFact LatticeFact::meetLt(const int op0, std::string op1) {
  // op0 < op1 <==> op1 > op0
  ap_lincons1_t cons = createSubConstr(op1, op0, AP_CONS_SUP);
  return meetAndFreeConstr(&cons);
}

LatticeFact LatticeFact::meetLeq(std::string op0, std::string op1) {
  // op0 <= op1 <==> op1 >= op0
  ap_lincons1_t cons = createSubConstr(op1, op0, AP_CONS_SUPEQ);
  return meetAndFreeConstr(&cons);
}

LatticeFact LatticeFact::meetLeq(std::string op0, const int op1) {
  // op0 <= op1 <==> op1 >= op0
  ap_lincons1_t cons = createSubConstr(op1, op0, AP_CONS_SUPEQ);
  return meetAndFreeConstr(&cons);
}

LatticeFact LatticeFact::meetLeq(const int op0, std::string op1) {
  // op0 <= op1 <==> op1 >= op0
  ap_lincons1_t cons = createSubConstr(op1, op0, AP_CONS_SUPEQ);
  return meetAndFreeConstr(&cons);
}

LatticeFact LatticeFact::meetConstrTy(const std::string op0
    , const std::string op1, ap_constyp_t cons) {
  if (cons == AP_CONS_EQ) {
    return meetEq(op0, op1);
  }
  else if (cons == AP_CONS_SUPEQ) {
    return meetGeq(op0, op1);
  }
  else if (cons == AP_CONS_SUP) {
    return meetGt(op0, op1);
  }
  else if (cons == AP_CONS_EQMOD) {
    assert(0 && "eqmod unhandled");
  }
  else if (cons == AP_CONS_DISEQ) {
    return meetNEq(op0, op1);
  }
  else {
    assert(0 && "unreachable");
  }
}

LatticeFact LatticeFact::meetConstrTy(const int op0, const std::string op1
    , ap_constyp_t cons) {
  if (cons == AP_CONS_EQ) {
    return meetEq(op0, op1);
  }
  else if (cons == AP_CONS_SUPEQ) {
    return meetGeq(op0, op1);
  }
  else if (cons == AP_CONS_SUP) {
    return meetGt(op0, op1);
  }
  else if (cons == AP_CONS_EQMOD) {
    assert(0 && "eqmod unhandled");
  }
  else if (cons == AP_CONS_DISEQ) {
    return meetNEq(op0, op1);
  }
  else {
    assert(0 && "unreachable");
  }
}

LatticeFact LatticeFact::meetConstrTy(const std::string op0, const int op1
    , ap_constyp_t cons) {
  if (cons == AP_CONS_EQ) {
    return meetEq(op0, op1);
  }
  else if (cons == AP_CONS_SUPEQ) {
    return meetGeq(op0, op1);
  }
  else if (cons == AP_CONS_SUP) {
    return meetGt(op0, op1);
  }
  else if (cons == AP_CONS_EQMOD) {
    assert(0 && "eqmod unhandled");
  }
  else if (cons == AP_CONS_DISEQ) {
    return meetNEq(op0, op1);
  }
  else {
    assert(0 && "unreachable");
  }
}

bool LatticeFact::isSat(ap_lincons1_t cons) {
  return ap_abstract1_sat_lincons(manG, &abs_vals_, &cons);
}

bool LatticeFact::isBottom() {
  return ap_abstract1_is_bottom(manG, &abs_vals_);
}

ap_interval_t *LatticeFact::getVarInterval(llvm::Value *v
    , std::map<llvm::Value *, std::string> v2n) {
  auto it = v2n.find(v);
  if (it == v2n.end()) {
    return NULL;
  }
  else {
    return getVarInterval(it->second);
  }
}
ap_interval_t *LatticeFact::getVarInterval(std::string v) {
  ap_interval_t *ret = ap_abstract1_bound_variable(manG, &abs_vals_, (ap_var_t)v.c_str());
  assert(ret && "ap_abstract1_bound_variable(): failed to get interval");
  return ret;
}


LatticeFact LatticeFact::assignInterval(llvm::Value *v
    , ap_interval_t *interv
    , std::map<llvm::Value *, std::string> v2n) {
  std::string vStr = Utils::mapAtUnsafe(v2n, v);
  return assignInterval(vStr, interv);
}

LatticeFact LatticeFact::assignInterval(std::string v, ap_interval_t *interv) {
  // Copy constructor
  LatticeFact ret(*this);
  ap_environment_t *env = ret.abs_vals_.env;

  ap_linexpr1_t expr = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, 2);
  ap_linexpr1_set_list(&expr
      , AP_CST_I, interv
      , AP_END);

  ret.assignLinExprAndClearDestr((ap_var_t)v.c_str(), &expr);
  return ret;
}

bool LatticeFact::areValsLEq(LatticeFact f1, LatticeFact f2) {
  bool res;
  //res = ap_abstract1_is_leq(f1.man_, &f1.abs_vals_, &f2.abs_vals_);
  res = ap_abstract1_is_leq(manG, &f1.abs_vals_, &f2.abs_vals_);
  return res;
}

bool LatticeFact::areValsGEq(LatticeFact f1, LatticeFact f2) {
  bool res;
  // f1 >= f2 === f2 <= f1
  //res = ap_abstract1_is_leq(f1.manG, &f2.abs_vals_, &f1.abs_vals_);
  res = ap_abstract1_is_leq(manG, &f2.abs_vals_, &f1.abs_vals_);
  return res;
}

bool LatticeFact::areValsEq(LatticeFact f1, LatticeFact f2) {
  bool res;
  //res = ap_abstract1_is_eq(f1.man_, &f1.abs_vals_, &f2.abs_vals_);
  res = ap_abstract1_is_eq(manG, &f1.abs_vals_, &f2.abs_vals_);
  return res;
}
