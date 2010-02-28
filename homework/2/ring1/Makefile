PROJECT = ring
OBJS = $(PROJECT).o
CC = mpicc
CFLAGS = -c -g
OUTPUT = results_ring1.txt

$(PROJECT) : $(OBJS)
	$(CC) $(OBJS) -o $(PROJECT)

$(PROJECT).o : $(PROJECT).c
	$(CC) $(CFLAGS) $(PROJECT).c

all : $(PROJECT)

run : all
	./$(PROJECT) > $(OUTPUT)

clean :
	rm -f *.o
	rm -f $(PROJECT)
	rm -f $(OUTPUT)
