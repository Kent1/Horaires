CC      = gcc
CFLAGS  = -Wall -O -std=c99
LDFLAGS =
EXEC    = horaire
OBJ_DIR = obj
SRC     = $(wildcard *.c)
OBJ     = $(patsubst %.c,obj/%.o,$(SRC))

all: $(EXEC)

horaire: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

$(OBJ_DIR)/conflict.o: util.h structs/exam.h structs/room.h

$(OBJ_DIR)/heuristics.o: util.h structs/exam.h structs/room.h

$(OBJ_DIR)/main.o: util.h structs/exam.h structs/room.h heuristics.h conflict.h

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f $(OBJ)

mrproper: clean
	rm -f $(EXEC)
	rm -rf $(OBJ_DIR)
