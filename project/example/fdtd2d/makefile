############################
#
# Makefile written by Daniel Orozco
# April 8th, 2010
#
# Permission to copy this file is granted as long as this message is
# preserved.
#
#############################

##################################################################
#						Makefile Variables
##################################################################

PAPISRC = /usr/local/papi-3.7.1

CC = gcc

OBJS = main.o fdtd2d.o globaldefs.o

EXTRADEFS =

DEFINES = -DNSIZE="2000" -DAMPLITUDE="100" -DTIMESTEPS="100" $(EXTRADEFS)

INCLUDEPATH = $(PAPISRC)/include

INCLUDES = -I$(INCLUDEPATH) -I.

LIBPATH = $(PAPISRC)/lib

LIBS = -lpapi -lpthread

LIBRARIES = -L$(LIBPATH) $(LIBS) -lm

CFLAGS = $(INCLUDES) $(DEFINES)

PROGRAM = fdtd2d

# Counternum is used to run the program with a particular counter.
COUNTERNUM =


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
	gcc -o $(PROGRAM) $(OBJS) $(LIBRARIES)
	
run:
	export LD_LIBRARY_PATH=$(LIBPATH) && ./$(PROGRAM) $(COUNTERNUM) | tee out.txt
	
clean:
	rm -rf $(OBJS)
	rm -rf $(PROGRAM)
	
	

	
	
