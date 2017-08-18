#!/bin/bash
set -u

# Import the generic commands to run tests and stuff
source ./funcs.sh
source ./testdirs.sh

TMPPATH=/tmp/table_box.out
echo "Name & LOC & \\# of Pairs & \\# of filtered Pairs \\\\" > $TMPPATH
echo "\\midrule" >> $TMPPATH

for name in ${TEST_DIRS}
do
    cd $name

    # name of directory
    echo "$name" > /tmp/name.out
    # lines of code
    wc -l < main.c > /tmp/line.out
    # number of total pairs
    errsString=$(grep 'Total pairs: ' box_ncomb_pri.out)
    echo $errsString | sed 's/Total pairs: //' > /tmp/totalPairs.out
    # number of filtered pairs
    errsFString=$(grep 'Filtered pairs: ' box_ncomb_pri.out)
    echo $errsFString | sed 's/Filtered pairs: //' > /tmp/filteredPairs.out
    # add them to one column
    paste -d, /tmp/name.out /tmp/line.out /tmp/totalPairs.out /tmp/filteredPairs.out | sed 's/,/\&/g' | tr '\n' '@' | sed 's/@/ \\\\\n/g' >>$TMPPATH
    cd ..
done

LOC=`awk -F\& '{sum += $2} END {print sum}' $TMPPATH`
TOTALPAIR=`awk -F\& '{sum += $3} END {print sum}' $TMPPATH`
FILTERPAIR=`awk -F\& '{sum += $4} END {print sum}' $TMPPATH`

NUM_LINES=`wc -l < $TMPPATH`
NUM_LINES=$(($NUM_LINES))

echo "\\midrule" >> $TMPPATH
echo "\\textbf{Total} & $LOC & $TOTALPAIR & $FILTERPAIR \\\\" >>$TMPPATH

cat $TMPPATH 
