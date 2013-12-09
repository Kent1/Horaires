CC      = gcc
CFLAGS  = -Wall -O
LDFLAGS =
EXEC    = horaire
SRC     = $(wildcard *.c)
OBJ     = $(SRC:.c=.o)

all: $(EXEC)

horaire: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

conflict.o: util.h structs/exam.h structs/room.h

heuristics.o: util.h structs/exam.h structs/room.h

main.o: util.h structs/exam.h structs/room.h heuristics.h conflict.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o

mrproper: clean
	rm -f $(EXEC)
