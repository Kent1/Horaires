CC       = gcc
CFLAGS   = -Wall -O -std=c99 -fPIC
LDFLAGS  = -lm
LIB_NAME = libtimetable.so
OBJ_DIR  = obj
SRC      = $(wildcard *.c)
OBJ      = $(patsubst %.c,obj/%.o,$(SRC))


all: obj_dir lib

obj_dir:
	@mkdir -p $(OBJ_DIR)

lib: $(OBJ)
	$(CC) -o $(LIB_NAME) -shared $(OBJ) $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c util.h structs.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f $(OBJ)

mrproper: clean
	rm -rf $(OBJ_DIR)
	rm -f $(LIB_NAME)
