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

#OBJS = do_loops.o dummy.o test_utils.o zero.o
OBJS = main.o


INCLUDEPATH = $(PAPISRC)/include

INCLUDES = -I$(INCLUDEPATH) -I.

LIBPATH = $(PAPISRC)/lib

LIBS = -lpapi

LIBRARIES = -L$(LIBPATH) $(LIBS)

CFLAGS = $(INCLUDES)


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
	gcc -o mytest $(OBJS) $(LIBRARIES)
	
run:
	export LD_LIBRARY_PATH=$(LIBPATH) && ./mytest
	
clean:
	rm -rf $(OBJS)
	rm -rf mytest
	
	
