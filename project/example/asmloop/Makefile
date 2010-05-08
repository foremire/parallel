############################
#
# Makefile written by Daniel Orozco
# May 4th, 2010
#
# Permission to copy this file is granted as long as this message is
# preserved.
#
#############################

##################################################################
#						Makefile Variables
##################################################################


CC = /opt/intel/Compiler/11.1/069/bin/intel64/icc

OBJS = main.o

LIBPATH = /opt/intel/Compiler/11.1/069/lib/intel64

LIBRARYPATH = -L$(LIBPATH)

LIBS = $(LIBRARYPATH) -lintlc

CFLAGS = 


##################################################################
#						Default Rules
#
#	The following default rule will look for a .c to produce a .o
#	This is useful because the OBJS variable say what objects
#	form the program.
#
##################################################################

.c.o:
	$(CC) $(CFLAGS) -c $<


##################################################################
#						Rules
##################################################################


all: $(OBJS)
	gcc -o myasm $(OBJS) $(LIBS)

toasm:
	$(CC) -S main.c
	$(CC) -S asmloop.c

build: asmloop.s c_loop.c main.c
	$(CC) -o myasm gettime.c main.c c_loop.c asmloop.s $(LIBS)

testasm:
	$(CC) -O0 -S asmgenerator.c 	
	
run:
	export LD_LIBRARY_PATH=$(LIBPATH) && ./myasm
	
clean:
	rm -rf $(OBJS)
	rm -rf myasm
	
	
