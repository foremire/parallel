PROJECT = mm
OBJS = $(PROJECT).o
CC = cc
CFLAGS = -c -g
OUTPUT = results_serial_mm.txt
RUN = ./$(PROJECT) 5

$(PROJECT) : $(OBJS)
	$(CC) $(OBJS) -o $(PROJECT)

$(PROJECT).o : $(PROJECT).c
	$(CC) $(CFLAGS) $(PROJECT).c

all : $(PROJECT)

run : all
	$(RUN) | tee $(OUTPUT)

clean :
	rm -f *.o
	rm -f $(PROJECT)
	rm -f $(OUTPUT)
