#!/bin/bash

#CBMC=~/i-cbmc/src-r3116/cbmc/cbmc
#CBMC2=~/i-cbmc/src-r4603/cbmc/cbmc
CBMC3=~/i-cbmc/src/cbmc/cbmc

fileName=$1
exptName=$2

currFolder=.
dataFolder=$currFolder/'data-'$exptName
dataName=$dataFolder/'data_'$exptName
timeName=$dataFolder/'time_'$exptName

if [ -d $dataFolder ]
then
  echo "Folder " + $dataFolder + " already exists. "
  exit
fi

mkdir $dataFolder

#(time $CBMC2 --mm irq $fileName -DCBMC -DREAL_T=float -DUSE_FLOAT &> $dataName'2.txt') &> $timeName'2.txt'
(time $CBMC3 --mm irq $fileName -DCBMC -DREAL_T=float -DUSE_FLOAT &> $dataName'3.txt') &> $timeName'3.txt'
#(time $CBMC --mm irq $fileName -DCBMC -DREAL_T=float -DUSE_FLOAT &> $dataName'.txt') &> $timeName'.txt'

