#######################################
#
# 	Makefile written by Daniel Orozco
#
#	Permission to copy or redistribute 
#	this file  in part or as a whole is granted as long as
#	this message is preserved along with any copies.
#
########################################

########################################
#		Makefile Variables
########################################

CC = gcc
INCLUDED = -I./
CCFLAGS = -Wall -Wextra -O3 $(INCLUDED)

OBJS = primitives.o my_sync.o

########################################
#		Default rules
########################################

# Dependencies	
.c.o:
	$(CC) $(CCFLAGS) -c $<

########################################
#		Makefile Rules
########################################


# Default build rule
all: $(OBJS)
	$(CC) $(CCFLAGS) $(OBJS) -o performance -lpthread

run:
	./performance 3500 8 | tee results.txt
#	./performance 3500 16 | tee -a results.txt
#	./performance 2 4
	
clean:
	rm -rf performance
	rm -rf *.o
