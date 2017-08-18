#!/bin/bash

foldername=$1                                            
filename='brake2_2_po.c'        

if [ -d $foldername ]                                    
then
  echo "Folder " + $dataFolder+ " already exists. "      
  exit
fi
  
mkdir $foldername

BENCHMARKS=
PWD=`pwd`
PROGRESSLOG=${PWD}/${foldername}/progress.log                       
TIMELOG=${PWD}/${foldername}/runtime.log
FLAGS="--mm irq -DCBMC -DREAL_T=float -DUSE_FLOAT"       
EXEC=~/i-cbmc/src/cbmc/cbmc                              
RUNSOLVER=~/runsolver/src/runsolver                      
CTIMEOUT=3600
WTIMEOUT=7200                                            
MEM=16384
SUFFIX=""                                                

echo "Starting experiments at `date` on `hostname`" > ${PROGRESSLOG}${SUFFIX}
                                                         
#for filename in `ls brake2_*_po.c`
#for filename  in `ls ${BENCHMARKS}/*.c`;
for i in 1 2 3
do
rm -f ${foldername}/${filename}.watch${SUFFIX}$i
rm -f ${foldername}/${filename}.out${SUFFIX}$i
rm -f ${foldername}/${filename}.time${SUFFIX}$i
echo -n "Starting to processing  $filename at `date`" >> ${PROGRESSLOG}${SUFFIX}
${RUNSOLVER} -w ${foldername}/${filename}.watch${SUFFIX}$i -v ${foldername}/${filename}.time${SUFFIX}$i -o ${foldername}/${filename}.out${SUFFIX}$i -C ${CTIMEOUT} -W ${WTIMEOUT} -d 1 -M ${MEM} ${EXEC} ${FLAGS} $filename
grep -e "CPUTIME=" ${foldername}/${filename}.time${SUFFIX}$i >> ${PROGRESSLOG}${SUFFIX}
done
