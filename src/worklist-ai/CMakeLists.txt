add_library(worklistAI MODULE 
  worklistai.cpp 
  latticefact.cpp 
  bbops.cpp
  utils.cpp
  ../utils/z3_fp_helpers.cpp
  varvisit.cpp) 

find_library(Z3_LIB z3)
message(STATUS ${Z3_LIB})

target_link_libraries(worklistAI -L${APRON_LIB} -lapron_debug -lboxMPQ_debug
  -loctMPQ_debug ${Z3_LIB})
