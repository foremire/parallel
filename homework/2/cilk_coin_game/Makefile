PROJECT = coin
OBJS = $(PROJECT).o
CILKCC = /home/xliu/usr/bin/cilkc
CILKINCLUDE = -I/home/xliu/usr/include/cilk
CILKLIBS = -L/home/xliu/usr/lib/cilk

CC = $(CILKCC)
CFLAGS = -c -g -Wall $(CILKINCLUDE)
LD_FLAG = $(CILKLIBS)
OUTPUT = results_coin_game.txt
RUN = ./$(PROJECT) 3

$(PROJECT) : $(OBJS)
	$(CC) $(OBJS) $(LD_FLAG) -o $(PROJECT)

$(PROJECT).o : $(PROJECT).cilk
	$(CC) $(CFLAGS) $(PROJECT).cilk

all : $(PROJECT)

run : all
	$(RUN) | tee $(OUTPUT)

clean :
	rm -f *.o
	rm -f $(PROJECT)
	rm -f $(OUTPUT)
