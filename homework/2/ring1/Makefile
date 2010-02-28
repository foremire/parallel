PROJECT = ring
OBJS = $(PROJECT).o
CC = mpicc
CFLAGS = -c -g
OUTPUT = results_ring1.txt
RUN = mpirun -np 16

$(PROJECT) : $(OBJS)
	$(CC) $(OBJS) -o $(PROJECT)

$(PROJECT).o : $(PROJECT).c
	$(CC) $(CFLAGS) $(PROJECT).c

all : $(PROJECT)

run : all
#	$(RUN) $(PROJECT)
	$(RUN) $(PROJECT) | tee $(OUTPUT)

clean :
	rm -f *.o
	rm -f $(PROJECT)
	rm -f $(OUTPUT)
