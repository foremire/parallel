PROJECT = mm
OBJS = $(PROJECT).o
CC = cc
CFLAGS = -c -g -Wall -fopenmp
LD_FLAG = -lgomp
OUTPUT = results_mm.txt
RUN = ./$(PROJECT) 8 543

$(PROJECT) : $(OBJS)
	$(CC) $(OBJS) $(LD_FLAG) -o $(PROJECT)

$(PROJECT).o : $(PROJECT).c
	$(CC) $(CFLAGS) $(PROJECT).c

all : $(PROJECT)

run : all
	$(RUN) | tee $(OUTPUT)

clean :
	rm -f *.o
	rm -f $(PROJECT)
	rm -f $(OUTPUT)
