#!/bin/bash

# Definition of a bunch of functions which perform different analyses

set -u
source ./exports.sh

##### Begin global definitions
# Make sure we use the time binary and not the builtin shell command
TIME="`which time` -f %e"

# Max time, passed to timeout command. Can be something like 1s, 30m, or 2h (seconds, minutes, hours)
MAX_TIME=30m

TIMEOUT="timeout $MAX_TIME"

# arguments to pass to every abstract interpreter pass
#AI_ARGS="-nodebug"
AI_ARGS=""

# Used by error checking functions
numErrs=X
TEST_FAILED=0
##### End global definitions


# $1 should be the file containing the test output (e.g., box_ncomb.out)
# output is stored in numErrs global
get_num_errs() {

  errString=$(grep 'Errors found: ' ${1}.out)

  if [ "$?" -ne "0" ]
  then
    die 'ERROR: get_num_errs(): error string not found in output'
  fi

  # chop off the "Errors found: " in the output
  numErrs=$(echo $errString | sed 's/Errors found: //')
  echo "$numErrs" > $1.errs
}

# given the prefix of a test name (e.g., box_ncomb) if its output and expected
# number of errors exists then perform the check, otherwise, do nothing.
check_results() {
  if [ -z "${1:-}" ]
  then
    die 'ERROR: check_results(): $1 is empty'
  fi

if [ -f "${1}.out" ]
then
  get_num_errs ${1}
  if [ -f "${1}.errs" ]
  then 
    expErrs=$(cat ${1}.errs)
    if [ "$numErrs" -ne "$expErrs" ]
    then 
      echo "TEST FAILED: $1"
      TEST_FAILED=1
    else 
      echo "TEST PASSED: $1"
      TEST_FAILED=0
    fi
  else 
    echo "WARNING: Errs file missing (${1}.errs)"
    TEST_FAILED=2
  fi
else 
  echo "WARNING: Output file missing (${1}.out)"
  TEST_FAILED=2
fi
}

# $1 should be the return code of a process run with timeout. $2
# should be a log file to write the timeout information to. $3 should be an
# error message associated with the command being checked. This will be
# displayed if the command exited with an error code
#
# If the return code is 124, then the process timedout. Write this to the log
# file ($2). If the return code is zero, then sucess. Otherwise, error; this
# function will call die and close the script
check_timeout() {
  if [ -z "${1:-}" ]
  then
    die "check_timeout(): no first argument"
  fi

  if [ -z "${2:-}" ]
  then
    die "check_timeout(): no second argument"
  fi

  if [ -z "${3:-}" ]
  then
    die "check_timeout(): no third argument"
  fi

  if [ "$1" -eq "124" ]
  then 
    echo "TIMEOUT"
  elif [ "$1" -ne "0" ]
  then
    die "$3"
  fi
  # passed :)
}

# Build the .bc file
build_main() {
  echo "$CLANG -emit-llvm -S -c main.c"
  $CLANG -emit-llvm -S -c main.c  || die "error: clang"
  # Use mem2reg in hopes that this simplifies things enough that we don't have
  # to worry about weird features
  $OPT -mem2reg main.ll >main2.bc || die "error: opt mem2reg"
  $LLVM_DIS main2.bc  || die "error: llvm-dis main2.bc"
  mv main2.ll main.ll
  rm main2.bc
}

# run box domain non-combinational interference exploration
run_box_ncomb_nonpri() {
  echo "$TIME -o box_ncomb_nonpri.time $OPT -load $WORKLIST_SO -worklist-ai  -nocombs -box main.ll >main_out.bc 2>box_ncomb_nonpri.out"
  date
  $TIMEOUT $TIME -o box_ncomb_nonpri.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -nocombs -box main.ll >main_out.bc 2>box_ncomb_nonpri.out 
  ret=$?
  check_timeout $ret box_ncomb_nonpri.out "[ERROR]: opt (box ncomb)" 
  check_results box_ncomb_nonpri
  #echo "Box Non-combinational Passed"
}

run_box_ncomb_pri() {
  echo "$TIME -o box_ncomb_pri.time $OPT -load $WORKLIST_SO -worklist-ai  -nocombs -priority -box main.ll >main_out.bc 2>box_ncomb_pri.out"
  date
  $TIMEOUT $TIME -o box_ncomb_pri.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -nocombs -priority -box main.ll >main_out.bc 2>box_ncomb_pri.out 
  ret=$?
  check_timeout $ret box_ncomb_pri.out "[ERROR]: opt (box ncomb)" 
  check_results box_ncomb_pri
  #echo "Box Non-combinational Passed"
}

# Combinational exploration without constraints
# run opt box domain with combinaional exploration (no constraints)
# Also, run with priority information
run_box_comb_pri() {
  date
  echo "$OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -box -priority main.ll >main_out.bc 2>box_comb_pri.out"
  $TIMEOUT $TIME -o box_comb_pri.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -box -priority main.ll >main_out.bc 2>box_comb_pri.out
  ret=$?
  check_timeout $ret box_comb_pri.out "[ERROR]: opt (box comb)"
  #cmpOrWarn box_comb.out box_comb.exp
  check_results box_comb_pri
  #echo "Box Combinational Passed"
}

# Combinational exploration without constraints
# run opt box domain with combinaional exploration (no constraints)
run_box_comb_nonpri() {
  date
  echo "$OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -box main.ll >main_out.bc 2>box_comb_nonpri.out"
  $TIMEOUT $TIME -o box_comb_nonpri.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -box main.ll >main_out.bc 2>box_comb_nonpri.out
  ret=$?
  check_timeout $ret box_comb_nonpri.out "[ERROR]: opt (box comb)"
  #cmpOrWarn box_comb.out box_comb.exp
  check_results box_comb_nonpri
  #echo "Box Combinational Passed"
}

# run opt oct domain with combinaional exploration (no constraints)
# run with priority information
run_oct_comb_pri() {
    date
    echo "$OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -oct -priority main.ll >main_out.bc 2>oct_comb_pri.out"
    $TIMEOUT $TIME -o oct_comb_pri.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -oct -priority main.ll >main_out.bc 2>oct_comb_pri.out 
    ret=$?
    check_timeout $ret oct_comb_pri.out "[ERROR]: opt (oct comb)"
    #cmpOrWarn oct_comb.out oct_comb.exp
    check_results oct_comb_pri
    #echo "Oct Combinational Passed"
}

# run opt oct domain with combinaional exploration (no constraints)
run_oct_comb_nonpri() {
    date
    echo "$OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -oct main.ll >main_out.bc 2>oct_comb_nonpri.out"
    $TIMEOUT $TIME -o oct_comb_nonpri.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -oct main.ll >main_out.bc 2>oct_comb_nonpri.out 
    ret=$?
    check_timeout $ret oct_comb_nonpri.out "[ERROR]: opt (oct comb)"
    #cmpOrWarn oct_comb.out oct_comb.exp
    check_results oct_comb_nonpri
    #echo "Oct Combinational Passed"
}

die () {
  if [ ! -z "${@:-}" ]
  then
    echo ${@}
  fi
  return 1
}

# Compare the two passed file names. Crash if they are different
cmpOrDie () {
  if [ -z ${1:-} ]
  then
    die "cmpOrDie(): no first argument"
  fi

  if [ -z ${2:-} ]
  then
    die "cmpOrDie(): no second argument"
  fi

  if [ ! -f "$1" ]
  then
    die "cmpOrDie(): $1: file not found"
  fi
  if [ ! -f "$2" ]
  then
    die "cmpOrDie(): $2: file not found"
  fi

  diff $1 $2 
  res=$?

  if [ "$res" -ne "0" ]
  then
    die "$1 does not match $2"
  fi
}

# Compare the two passed file names. Warn if they are different
cmpOrWarn () {
  if [ -z "${1:-}" ]
  then
    die "cmpOrDie(): no first argument"
  fi

  if [ -z "${2:-}" ]
  then
    die "cmpOrDie(): no second argument"
  fi

  if [ ! -f "$1" ]
  then
    die "cmpOrDie(): $1: file not found"
  fi
  if [ ! -f "$2" ]
  then
    #die "cmpOrDie(): $2: file not found"
    echo "[WARNING] cmpOrDie(): $2: file not found"
  fi

  diffOut="${1}_${2}.diff" 
  diff $1 $2 >$diffOut
  res=$?

  if [ "$res" -ne "0" ]
  then
    echo "[WARNING] $1 does not match $2 (see: $diffOut)"
  fi
}

# Begin functions performing different types of analyses

# run box domain non-combinational interference exploration
run_box_ncomb() {
  echo "$TIME -o box_ncomb.time $OPT -load $WORKLIST_SO -worklist-ai  -nocombs -box main.ll >main_out.bc 2>box_ncomb.out"
  date
  #$TIME -o box_ncomb.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -nocombs -box main.ll >main_out.bc 2>box_ncomb.out || die "[ERROR]: opt (box ncomb)" 
  $TIMEOUT $TIME -o box_ncomb.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -nocombs -priority -box main.ll >main_out.bc 2>box_ncomb.out 
  ret=$?
  check_timeout $ret box_ncomb.out "[ERROR]: opt (box ncomb)" 
  #cmpOrWarn box_ncomb.out box_ncomb.exp
  #cmpOrWarn box_ncomb.out box_ncomb.exp
  check_results box_ncomb
  #echo "Box Non-combinational Passed"
}

run_box_slice() {
  echo "  $TIMEOUT $TIME -o box_ncomb.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -nocombs -aslice -box main.ll >main_out.bc 2>box_ncomb_slice.out"
  date
  $TIMEOUT $TIME -o box_ncomb_slice.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -nocombs -aslice -box main_pdg.ll >main_out.bc 2>box_ncomb_slice.out 
  ret=$?
  check_timeout $ret box_ncomb_slice.out "[ERROR]: opt (box ncomb slice)" 
}

run_oct_ncomb() {
  # run oct no combinations. the special file allows the test to be skipped
  if [ ! -f "no_oct_ncomb" ]
  then
    date
    #$TIME -o oct_ncomb.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -nocombs -oct main.ll >main_out.bc 2>oct_ncomb.out || die "[ERROR]: opt (oct ncomb)" 
    $TIMEOUT $TIME -o oct_ncomb.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -nocombs -oct main.ll >main_out.bc 2>oct_ncomb.out
    ret=$?
    check_timeout $ret cot_ncomb.out "[ERROR]: opt (oct ncomb)" 
    #cmpOrWarn oct_ncomb.out oct_ncomb.exp
    check_results oct_ncomb
    #echo "Oct Non-combinational Passed"
  else
    echo "Skipping Oct Non-combinational"
  fi
}




# run opt box domain with combinaional exploration with constraints
run_box_constr() {
  date
  #$TIME -o box_constr.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -constraints -z3 $Z3_BIN -box main.ll >main_out.bc 2>box_constr.out || die "[ERROR]: opt (box constr)" 
  echo "$TIMEOUT $TIME -o box_constr.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -constraints -z3 $Z3_BIN -box main.ll >main_out.bc 2>box_constr.out"
  $TIMEOUT $TIME -o box_constr.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -constraints -z3 $Z3_BIN -box main.ll >main_out.bc 2>box_constr.out 
  ret=$?
  check_timeout $ret box_constr.out "[ERROR]: opt (box constr)" 
  #cmpOrWarn box_constr.out box_constr.exp
  check_results box_constr
  #echo "Box Constraints Passed"
}

# run opt oct domain with combinaional exploration with constraints
run_oct_constr() {
  if [ ! -f "no_oct_constr" ]
  then
    date
    #$TIME -o oct_constr.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -constraints -z3 $Z3_BIN -oct main.ll >main_out.bc 2>oct_constr.out || die "[ERROR]: opt (oct constr)" 
    $TIMEOUT $TIME -o oct_constr.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -constraints -z3 $Z3_BIN -oct main.ll >main_out.bc 2>oct_constr.out 
    ret=$?
    check_timeout $ret oct_constr.out "[ERROR]: opt (oct constr)" 
    #cmpOrWarn oct_constr.out oct_constr.exp
    check_results oct_constr
    #echo "Oct Constraints Passed"
  else 
    echo "Oct constraints skipped"
  fi
}

# Generate the PDG
gen_pdg() {
  echo "$OPT -load $DYN_PDG_SO -contextinsen-dynpdg -mdassert -assert -z3 $Z3_OLD_BIN main.ll >main_pdg.bc"
  #$TIME -o pdg.time $OPT -load $DYN_PDG_SO -contextinsen-dynpdg -mdassert -assert -z3 $Z3_BIN main.ll >main_pdg.bc 2>pdg.out || die "[ERROR]: opt (pdg)" 
  $TIMEOUT $TIME -o pdg.time $OPT -load $DYN_PDG_SO -contextinsen-dynpdg -mdassert -assert -z3 $Z3_OLD_BIN main.ll >main_pdg.bc 2>pdg.out
  ret=$?
  check_timeout $ret pdg.out "[ERROR]: opt (pdg)" 
  echo "PDG Generated"
  llvm-dis main_pdg.bc
}

gen_impact() {
  gen_pdg
  if [ ! -f "impacted_insts" ]
  then
    die "[ERROR] file with impacted instructions (impacted_insts) not found"
  fi

  IMPACTED=$(cat impacted_insts)

  #$TIME -o pdg.time $OPT -load $DYN_PDG_SO -contextinsen-dynpdg -mdassert -assert -z3 $Z3_BIN main.ll >main_pdg.bc 2>pdg.out || die "[ERROR]: opt (pdg)" 
  echo "$TIMEOUT $TIME -o impact.time $OPT -load $DYN_PDG_SO -contextinsen-dynpdg -impact $IMPACTED -z3 $Z3_OLD_BIN main_pdg.ll >main_impact.bc 2>impact.out"
  $TIMEOUT $TIME -o impact.time $OPT -load $DYN_PDG_SO -contextinsen-dynpdg -impact $IMPACTED -z3 $Z3_OLD_BIN main_pdg.ll >main_impact.bc 2>impact.out
  $LLVM_DIS main_impact.bc
  ret=$?
  check_timeout $ret impact.out "[ERROR]: opt (impact)" 
  echo "impact generated"
  llvm-dis main_impact.bc
}

get_impact_stats() {
  if [ ! -f "main_impact.bc" ]
  then
    gen_impact
  fi
  $TIMEOUT $TIME -o impact.time $OPT -load $DYN_PDG_SO -contextinsen-dynpdg -impact-stats main_impact.bc >/dev/null 2>impact_stats.out
}

# Run box with PDG
run_box_pdg() {
  # If the file "no_slice" exists then the slice tests will not be run
  if [ ! -f "no_slice" ]
  then
    # run box with assert slice
    date
    #$TIME -o box_slice.time $OPT -load $WORKLIST_SO -worklist-ai -aslice $AI_ARGS -constraints -z3 $Z3_BIN -box main_pdg.ll >main_out.bc 2>box_slice.out || die "[ERROR]: opt (box slice)" 
    echo "$TIMEOUT $TIME -o box_slice.time $OPT -load $WORKLIST_SO -worklist-ai -aslice $AI_ARGS -constraints -z3 $Z3_BIN -box main_pdg.ll >main_out.bc 2>box_slice.out"
    $TIMEOUT $TIME -o box_slice.time $OPT -load $WORKLIST_SO -worklist-ai -aslice $AI_ARGS -constraints -z3 $Z3_BIN -box main_pdg.ll >main_out.bc 2>box_slice.out 
    ret=$?
    check_timeout $ret box_slice.out "[ERROR]: opt (box slice)" 
    #cmpOrWarn box_slice.out box_slice.exp
    check_results box_slice
    #echo "Box Slice passed"
    # Calculate the PDG creation time and the abstract interpretation time
    echo `cat box_slice.time` + `cat pdg.time` | bc >box_slice_pdg.time
  fi
}

# Run box with change impact and constraints
run_box_imp_constr() {
  # If the file "no_impact" exists then the impact tests will not be run
  if [ ! -f "no_impact" ]
  then
    date
    echo "$TIMEOUT $TIME -o box_imp_constr.time $OPT -load $WORKLIST_SO -worklist-ai -impact -aslice $AI_ARGS -constraints -z3 $Z3_BIN -box main_impact.ll >main_out.bc 2>box_imp_constr.out"
    $TIMEOUT $TIME -o box_imp_constr.time $OPT -load $WORKLIST_SO -worklist-ai -impact -aslice $AI_ARGS -constraints -z3 $Z3_BIN -box main_impact.ll >main_impact_out.bc 2>box_imp_constr.out 
    ret=$?
    check_timeout $ret box_imp_constr.out "[ERROR]: opt (box impact constrants)" 
    check_results box_imp_constr
    # Calculate the PDG creation time and the abstract interpretation time
    #echo "`cat box_imp_constr.time` + `cat impact.time`" | bc >box_imp_constr.time
    echo "`cat box_imp_constr.time` + `cat impact.time`" | bc >box_imp_constr2.time
  fi
}

# Run box impact withouut constrants or combinational exploration
run_box_imp() {
  # If the file "no_impact" exists then the impact tests will not be run
  if [ ! -f "no_impact" ]
  then
    date
    $TIMEOUT $TIME -o box_imp.time $OPT -load $WORKLIST_SO -worklist-ai -impact -aslice $AI_ARGS -nocombs -z3 $Z3_BIN -box main_impact.ll >main_impact_out.bc 2>box_imp.out 
    ret=$?
    check_timeout $ret box_imp.out "[ERROR]: opt (box impact)" 
    check_results box_imp
    # Calculate the PDG creation time and the abstract interpretation time
    #echo "`cat box_imp_constr.time` + `cat impact.time`" | bc >box_imp_constr.time
    echo "`cat box_imp.time` + `cat impact.time`" | bc >box_imp2.time
  fi
}

# run oct with PDG
run_oct_pdg() {
  if [ ! -f "no_slice" ]
  then
    if [ ! -f "no_oct_slice" ]
    then
      # run oct with assert slice
      date
      #$TIME -o oct_slice.time $OPT -load $WORKLIST_SO -worklist-ai -aslice $AI_ARGS -constraints -z3 $Z3_BIN -oct main_pdg.ll >main_out.bc 2>oct_slice.out || die "[ERROR]: opt (oct slice)" 
      $TIMEOUT $TIME -o oct_slice.time $OPT -load $WORKLIST_SO -worklist-ai -aslice $AI_ARGS -constraints -z3 $Z3_BIN -oct main_pdg.ll >main_out.bc 2>oct_slice.out 
      ret=$?
      check_timeout $ret oct_slice.out "[ERROR]: opt (oct slice)" 
      check_results oct_slice
      #cmpOrWarn oct_slice.out oct_slice.exp
      #echo "Oct Slice passed"
      # Calculate the PDG creation time and the abstract interpretation time
      echo `cat oct_slice.time` + `cat pdg.time` | bc >oct_slice_pdg.time
    else 
      echo "Oct slice skipped"
    fi
  fi
}


run_box_tso() {
  date
  echo "$TIMEOUT $TIME -o box_tso.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -tso -constraints -z3 $Z3_BIN -box main.ll >main_out.bc 2>box_tso.out"
  $TIMEOUT $TIME -o box_tso.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -tso -constraints -z3 $Z3_BIN -box main.ll >main_out.bc 2>box_tso.out 
  ret=$?
  check_timeout $ret box_tso.out "[ERROR]: opt (box tso)" 
  check_results box_tso
}

run_box_pso() {
  date
  echo "$TIMEOUT $TIME -o box_pso.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -pso -constraints -z3 $Z3_BIN -box main.ll >main_out.bc 2>box_pso.out"
  $TIMEOUT $TIME -o box_pso.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -pso -constraints -z3 $Z3_BIN -box main.ll >main_out.bc 2>box_pso.out 
  ret=$?
  check_timeout $ret box_pso.out "[ERROR]: opt (box pso)" 
  check_results box_pso
}

run_box_rmo() {
  date
  echo "$TIMEOUT $TIME -o box_rmo.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -rmo -constraints -z3 $Z3_BIN -box main.ll >main_out.bc 2>box_rmo.out"
  $TIMEOUT $TIME -o box_rmo.time $OPT -load $WORKLIST_SO -worklist-ai $AI_ARGS -rmo -constraints -z3 $Z3_BIN -box main.ll >main_out.bc 2>box_rmo.out 
  ret=$?
  check_timeout $ret box_rmo.out "[ERROR]: opt (box rmo)" 
  check_results box_rmo
}

run_duet() {
  DUET_LOC=/home/markus/src/duet/duet/duet
  date
  echo $DUET_LOC -coarsen main.c
  $TIMEOUT $TIME -o duet.time $DUET_LOC -coarsen main.c >duet.out
  ret=$?
  check_timeout $ret duet.out "[ERROR]: duet"
}

run_duet_param() {
  DUET_LOC=/home/markus/src/duet/duet/duet
  date
  $TIMEOUT $TIME -o duetp.time $DUET_LOC -coarsen -parameterized main_duet.c >duet_pr.out
  ret=$?
  check_timeout $ret duet.out "[ERROR]: duet"
}
