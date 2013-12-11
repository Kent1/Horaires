CC      = gcc
CFLAGS  = -Wall -O -std=c99 -fPIC
LDFLAGS =
LIB_NAME= libhoraire.so
OBJ_DIR = obj
SRC     = $(wildcard *.c)
OBJ     = $(patsubst %.c,obj/%.o,$(SRC))

all: lib

lib: $(OBJ)
	$(CC) -o $(LIB_NAME) -shared  $(OBJ) $(LDFLAGS)

$(OBJ_DIR)/conflict.o: util.h structs.h

$(OBJ_DIR)/heuristics.o: util.h structs.h

$(OBJ_DIR)/main.o: util.h structs.h heuristics.h conflict.h

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f $(OBJ)

mrproper: clean
	rm -f $(EXEC)
	rm -rf $(OBJ_DIR)
	rm -f $(LIB_NAME)
