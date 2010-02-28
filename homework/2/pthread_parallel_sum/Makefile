PROJECT = sum
OBJS = $(PROJECT).o
CC = cc
CFLAGS = -c -g -Wall
LD_FLAG = -lpthread
OUTPUT = results_parallel_sum.txt
RUN = ./$(PROJECT) 3

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
