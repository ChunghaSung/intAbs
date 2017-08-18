/*
 * Author: Markus Kusano
 *
 * A class of static utility functions 
 */
#pragma once

#include <string>
#define NUMPRI 9
extern const unsigned GLOBAL_ID = NUMPRI;

struct Utils {
  // Return a SMT-LIB2 const bitvector form of the passed unsigned
  static std::string to_const_bitvec(unsigned i);
};
