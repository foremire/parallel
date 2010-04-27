############################
#
# Makefile written by Xitong Liu
# April 9th, 2010
#
#############################

##################################################################
#						Makefile Variables
##################################################################

PROJECT = mm
PAPISRC = /usr/local/papi-3.7.1
OBJS = $(PROJECT).o

INCLUDEPATH = $(PAPISRC)/include
INCLUDES = -I$(INCLUDEPATH) -I.
LIBPATH = $(PAPISRC)/lib
LIBS = -lpapi -lgomp -lm
LIBRARIES = -L$(LIBPATH) $(LIBS)

CC = cc
CFLAGS = -c -g -Wall -fopenmp -msse3 $(INCLUDES)
OUTPUT = results_mm.txt
RUN = ./$(PROJECT)

$(PROJECT) : $(OBJS)
	$(CC) $(OBJS) $(LIBRARIES) -o $(PROJECT)

mm.o : mm.c mm.h
	$(CC) $(CFLAGS) mm.c

all : $(PROJECT)

run : all
	$(RUN) | tee $(OUTPUT)

clean :
	rm -f *.o
	rm -f $(PROJECT)
	rm -f $(OUTPUT)
