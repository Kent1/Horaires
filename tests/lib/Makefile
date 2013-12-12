CC          = gcc
CFLAGS      = -Wall -O -std=c99 -I$(LIB_DIR)
LDFLAGS     = -L$(LIB_DIR) -l$(LIB_HORAIRE) -lcunit
SRC         = $(filter-out $(MAIN), $(wildcard *.c))
BASEDIR     = ../..
LIB_DIR     = $(BASEDIR)/lib
LIB_HORAIRE = horaire
MAIN        = main.c
EXEC        = test
OBJ_DIR     = obj
OBJ         = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC))



lib:
	$(MAKE) -C $(LIB_DIR)

compile_tests: lib $(OBJ)
	$(CC) -o $(EXEC) $(MAIN) $(OBJ) $(CFLAGS) $(LDFLAGS)

test: compile_tests
	LD_LIBRARY_PATH=$(LIB_DIR) ./$(EXEC)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(OBJ)

mrproper: clean
	rm -f $(EXEC)
	rm -rf $(OBJ_DIR)