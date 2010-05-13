#!/bin/bash

# Compile the program
make -B

sum=0
run_time=10

for i in $(seq 1 $run_time)
do
  result=`make run | grep Serial | cut -d':' -f2`;
  sum=`echo "$sum+$result" | bc`
  #printf "\b->"
  printf "\b->"
done

printf "\n"
ave=`echo "scale=4;$sum/$run_time" | bc`
echo Average Serial Time $ave
