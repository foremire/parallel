PROJECT = hello
OBJS = $(PROJECT).o
CC = cc
CFLAGS = -c -g -Wall
LD_FLAG = -lpthread
OUTPUT = results_hello_world.txt
RUN_1 = ./$(PROJECT) 3
RUN_2 = ./$(PROJECT) 7
RUN_3 = ./$(PROJECT) 5
RUN_4 = ./$(PROJECT) 2

$(PROJECT) : $(OBJS)
	$(CC) $(OBJS) $(LD_FLAG) -o $(PROJECT)

$(PROJECT).o : $(PROJECT).c
	$(CC) $(CFLAGS) $(PROJECT).c

all : $(PROJECT)

run : all
	$(RUN_1) | tee $(OUTPUT)
	echo >> $(OUTPUT)
	$(RUN_2) | tee -a $(OUTPUT)
	echo >> $(OUTPUT)
	$(RUN_3) | tee -a $(OUTPUT)
	echo >> $(OUTPUT)
	$(RUN_4) | tee -a $(OUTPUT)

clean :
	rm -f *.o
	rm -f $(PROJECT)
	rm -f $(OUTPUT)
