############################
#
# Makefile written by Xitong Liu
# April 9th, 2010
#
#############################

##################################################################
#						Makefile Variables
##################################################################

PROJECT = sse
PAPISRC = /usr/local/papi-3.7.1
OBJS = $(PROJECT).o

INCLUDEPATH = $(PAPISRC)/include
INCLUDES = -I$(INCLUDEPATH) -I.
LIBPATH = $(PAPISRC)/lib
LIBS = -lpapi -lgomp -lm
LIBRARIES = -L$(LIBPATH) $(LIBS)

CC = cc
CFLAGS = -c -g -Wall -fopenmp -msse $(INCLUDES)
OUTPUT = results_mm.txt
RUN = ./$(PROJECT)

$(PROJECT) : $(OBJS)
	$(CC) $(OBJS) $(LIBRARIES) -o $(PROJECT)

sse.o : sse.c
	$(CC) $(CFLAGS) sse.c

all : $(PROJECT)

run : all
	$(RUN)

clean :
	rm -f *.o
	rm -f $(PROJECT)
	rm -f $(OUTPUT)
