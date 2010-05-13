#!/bin/bash

# Compile the program
make -B

sum=0
run_time=20

for i in $(seq 1 $run_time)
do
  result=`make run | grep MFLOPS | cut -d':' -f2`;
  sum=`echo "$sum+$result" | bc`
  #printf "\b->"
  printf "\b->"
done

printf "\n"
ave=`echo "scale=4;$sum/$run_time" | bc`
echo GFLOPS $ave
