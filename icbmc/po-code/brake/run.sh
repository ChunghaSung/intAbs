#!/bin/bash

#CBMC=~/cbmc-pty/src/cbmc/cbmc
#CBMC=~/i-cbmc/src-r3116/cbmc/cbmc
#CBMC=~/i-cbmc/src-4.9/cbmc/cbmc
CBMC=~/i-cbmc/src/cbmc/cbmc

dataFolder='data-'$1

if [ -d $dataFolder ]
then
  echo "Folder " + $dataFolder+ " already exists. "
  exit
fi

mkdir $dataFolder

# start to rock
for f in `ls brake2_*_po.c`
do
  exptName=`echo $f | cut -d . -f 1`
  dataName=$dataFolder/'data_'$exptName
  timeName=$dataFolder/'time_'$exptName
  (time $CBMC --mm irq $f -DCBMC -DREAL_T=float -DUSE_FLOAT &> $dataName'.txt') &> $timeName'.txt'
done

