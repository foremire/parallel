OBJS = hello.o
CC = cc
CFLAGS = -c -g
PROJECT = hello
OUTPUT = out.txt

$(PROJECT) : $(OBJS)
	$(CC) $(OBJS) -o $(PROJECT)

hello.o : hello.c
	$(CC) $(CFLAGS) hello.c

all : $(PROJECT)

run : all
	./$(PROJECT) > $(OUTPUT)

clean :
	rm -f *.o
	rm -f $(PROJECT)
	rm -f $(OUTPUT)
