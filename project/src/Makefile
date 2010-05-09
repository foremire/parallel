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
CFLAGS = -c -Wall -fopenmp -O0 -msse3 $(INCLUDES)
OUTPUT = results_mm.txt
RUN = ./$(PROJECT)

ASMS = omp_mat_mul_sse_ppl_asm.s

$(PROJECT) : $(OBJS) $(ASMS)
	$(CC) $(OBJS) $(ASMS) $(LIBRARIES) -o $(PROJECT)

mm.o : mm.c mm.h
	$(CC) $(CFLAGS) mm.c

omp_mat_mul_sse_ppl_asm.s: omp_mat_mul_sse_ppl_asm.c
	$(CC) -S $(CFLAGS) omp_mat_mul_sse_ppl_asm.c

all : $(PROJECT)

run : all
	$(RUN) | tee $(OUTPUT)

clean :
	rm -f *.o
	rm -f $(PROJECT)
	rm -f $(OUTPUT)
	rm -f *.s
