#!/bin/bash
set -u

# run all box and oct related tests for a single passed directory
set -u

# Import the generic commands to run tests and stuff
source ./funcs.sh
source ./testdirs.sh

for name in ${TEST_DIRS}
do
    echo "Testing $name"
    cd $name
    build_main
    run_box_ncomb_nonpri
    run_box_ncomb_pri
#run_box_comb_nonpri
#   run_box_comb_pri
#   run_oct_comb_nonpri
#   run_oct_comb_pri
    cd ..
done
