#!/bin/bash

# ProcessLine
# This function takes a line with a number and an instruction and it compiles
# and runs the program to get the PAPI COUNT
#
sumtime=0
experimentcount=0

ProcessLine()
{
  # Find the value to measure and the text.
  value=$1
  if [ $1 != "#" ]
  then
    msg=`echo $@ | cut -d'[' -f2 | cut -d']' -f1`
	
    #Run the program
    make run COUNTERNUM="$value"
	
    # Collect result
    result=`grep XxX ./out.txt | cut -d' ' -f2`
    echo $msg = $result | tee -a ./summary.txt
    localtime=`grep TtT ./out.txt | cut -d' ' -f2`
    sumtime=`echo $sumtime + $localtime | bc`
    experimentcount=`echo 1 + $experimentcount | bc`
    echo So far $experimentcount experiments have taken $sumtime seconds
  fi
}

# Read File: Reads a file from the command line and passes each line to the 
# Process Line function
ReadFile()
{
  # Sequentially read each Line
  while read line
  do
    # Process Each line
    ProcessLine $line
  done
}

# Compile the program
make -B

sum=0
run_time=20

for i in $(seq 1 $run_time)
do
  result=`make run | grep Parallel | cut -d':' -f2`;
  sum=`echo "$sum+$result" | bc`
  #printf "\b->"
  printf "\b->"
done

printf "\n"
ave=`echo "scale=4;$sum/$run_time" | bc`
echo Average Parallel Time $ave
