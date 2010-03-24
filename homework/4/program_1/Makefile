PROJECT = multiqueue
OBJS = $(PROJECT).o
CC = cc
CFLAGS = -c -g
OUTPUT = program_1_out.txt
RUN = ./$(PROJECT) 2 1 4

$(PROJECT) : $(OBJS)
	$(CC) $(OBJS) -o $(PROJECT)

$(PROJECT).o : $(PROJECT).c $(PROJECT).h
	$(CC) $(CFLAGS) $(PROJECT).c

all : $(PROJECT)

run : all
	$(RUN) | tee $(OUTPUT)

clean :
	rm -f *.o
	rm -f $(PROJECT)
	rm -f $(OUTPUT)
