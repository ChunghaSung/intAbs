/**
 * Author: Markus Kusano
 *
 * Debug printing macros.
 *
 * Enable/Disable by defining/not defining MK_DEBUG
 *
 * Also can be disabled with the -nodebug option (see worklistai.cpp)
 */

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
extern llvm::cl::opt<bool> nodebugG;

// Debug print
#ifdef MK_DEBUG
#define DEBUG_MSG(str) do { if (!nodebugG) llvm::errs() << str; } while(false)
#else
#define DEBUG_MSG(str) do { } while (false)
#endif

// Debug print in red
#ifdef MK_DEBUG
#define DEBUG_MSG_RED(str) \
do { \
  if (!nodebugG) { \
    if (llvm::errs().has_colors()) {  \
      llvm::errs().changeColor(raw_ostream::RED); \
    } \
    llvm::errs() << str;  \
    llvm::errs().resetColor(); \
  } \
} while (false)
#else
#define DEBUG_MSG_RED(str) do { } while (false)
#endif

// Debug execute arbitrary code
#ifdef MK_DEBUG
#define DEBUG(cmds) \
  do { \
    if (!nodebugG) { \
      cmds \
    } \
  } while(false)
#else
#define DEBUG(cmds) do { } while (false)
#endif
