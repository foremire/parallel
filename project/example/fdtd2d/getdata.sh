#!/bin/bash

################################################################################
# Script written by Daniel Orozco to automate obtaining data.
# Writen on 04/12/2010
# Permission to use this file is granted as long as this message is preserved.
#
################################################################################

################################################################################
# Functions
################################################################################

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


################################################################################
# "Main"
################################################################################

# Remove the results file
rm -rf ./summary.txt
touch ./summary.txt

# Compile the program
make -B EXTRADEFS="-DGENERIC_COUNTER"

# Run with each number requested
ReadFile < ./counters.input

#Report the experiments
echo $experimentcount experiments have taken $sumtime seconds | tee -a ./summary.txt
meantime=`echo "scale=4; $sumtime / $experimentcount" | bc`
echo $meantime seconds average | tee -a ./summary.txt


