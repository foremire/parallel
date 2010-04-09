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
LIBS = -lpapi -lgomp
LIBRARIES = -L$(LIBPATH) $(LIBS)

CC = cc
CFLAGS = -c -g -Wall -fopenmp $(INCLUDES)
OUTPUT = results_mm.txt
RUN = ./$(PROJECT) 8 543

.c.o:
	$(CC) $(CFLAGS) -c $<

$(PROJECT) : $(OBJS)
	$(CC) $(OBJS) $(LIBRARIES) -o $(PROJECT)

all : $(PROJECT)

run : all
	export LD_LIBRARY_PATH=$(LIBPATH) && $(RUN) | tee $(OUTPUT)

clean :
	rm -f *.o
	rm -f $(PROJECT)
	rm -f $(OUTPUT)
