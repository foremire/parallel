PROJECT = multiqueue
OBJS = $(PROJECT).o my_sync.o
CC = cc
CFLAGS = -c -g -Wall -Wextra -O0
LINKFLAGS = -lpthread
OUTPUT = program_3_out.txt
RUN = ./$(PROJECT) 4 2 10

$(PROJECT) : $(OBJS)
	$(CC) $(OBJS) -o $(PROJECT) $(LINKFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $<

all : $(PROJECT)

run : all
	$(RUN) | tee $(OUTPUT)

clean :
	rm -f *.o
	rm -f $(PROJECT)
	rm -f $(OUTPUT)
