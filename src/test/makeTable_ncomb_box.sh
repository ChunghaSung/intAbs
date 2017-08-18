#!/bin/bash
set -u

# Import the generic commands to run tests and stuff
source ./funcs.sh
source ./testdirs.sh

TMPPATH=/tmp/table_box.out
echo "Name & LOC & \\# of Inter. & Verif. & Time (s) & Verif. & Time (s) \\\\" > $TMPPATH
echo "\\midrule" >> $TMPPATH

for name in ${TEST_DIRS}
do
    cd $name

    # name of directory
    echo "$name" > /tmp/name.out
    # lines of code
    wc -l < main.c > /tmp/line.out
    # number of interrupt
    wc -l < priority.info > /tmp/interrupt.out
    # non priority box error
    cat box_ncomb_nonpri.errs > /tmp/nonpri.errs
    # non priority box time
    cat box_ncomb_nonpri.time > /tmp/nonpri.time
    # priority box error
    cat box_ncomb_pri.errs > /tmp/pri.errs
    # priority box time
    cat box_ncomb_pri.time > /tmp/pri.time
    # add them to one column
    paste -d, /tmp/name.out /tmp/line.out /tmp/interrupt.out /tmp/nonpri.errs /tmp/nonpri.time /tmp/pri.errs /tmp/pri.time | sed 's/,/\&/g' | tr '\n' '@' | sed 's/@/ \\\\\n/g' >>$TMPPATH
    cd ..
done

LOC=`awk -F\& '{sum += $2} END {print sum}' $TMPPATH`
INTTOTAL=`awk -F\& '{sum += $3} END {print sum}' $TMPPATH`
ERR1=`awk -F\& '{sum += $4} END {print sum}' $TMPPATH`
TIME1=`awk -F\& '{sum += $5} END {print sum}' $TMPPATH`
ERR2=`awk -F\& '{sum += $6} END {print sum}' $TMPPATH`
TIME2=`awk -F\& '{sum += $7} END {print sum}' $TMPPATH`

NUM_LINES=`wc -l < $TMPPATH`
NUM_LINES=$(($NUM_LINES))

echo "\\midrule" >> $TMPPATH
echo "\\textbf{Total} & $LOC & $INTTOTAL & $ERR1 & $TIME1 & $ERR2 & $TIME2 \\\\" >>$TMPPATH

cat $TMPPATH 
