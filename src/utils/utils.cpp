#include "utils.h"
#include <iomanip>
#include <sstream>
#include "mk_debug.h"

std::string Utils::to_const_bitvec(unsigned i) {
  std::stringstream stream;

  DEBUG_MSG("to_const_bitvec(): " << i << '\n');

  // Ensure that the constant is padded with zeros equal to the number of hex
  // digits in an unsigned (numbers of bytes * 2)
  unsigned num_nibbles = sizeof(unsigned) * 2;
  stream << "#x" << std::setfill('0') << std::setw(num_nibbles) 
         << std::hex << i;

  std::string ret( stream.str() );

  return ret;
}
