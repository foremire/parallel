PROJECT = multiqueue
OBJS = $(PROJECT).o
CC = cc
CFLAGS = -c -g -Wall -O0
LINKFLAGS = -lpthread
OUTPUT = program_1_out.txt
RUN = ./$(PROJECT) 1 2 10

$(PROJECT) : $(OBJS)
	$(CC) $(OBJS) -o $(PROJECT) $(LINKFLAGS)

$(PROJECT).o : $(PROJECT).c $(PROJECT).h
	$(CC) $(CFLAGS) $(PROJECT).c

all : $(PROJECT)

run : all
	$(RUN) | tee $(OUTPUT)

clean :
	rm -f *.o
	rm -f $(PROJECT)
	rm -f $(OUTPUT)
