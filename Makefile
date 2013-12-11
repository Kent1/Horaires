CC      = gcc
CFLAGS  = -Wall -O -std=c99
EXEC    = horaire
MAIN 	= main.c
LIB_DIR = lib
LIB_HORAIRE = horaire

LDFLAGS = -I$(LIB_DIR) -L$(LIB_DIR) -l$(LIB_HORAIRE)



lib:
	cd $(LIB_DIR) && $(MAKE) lib

main: lib
	$(CC) -o $(EXEC) $(MAIN) $(CFLAGS) $(LDFLAGS)

launch: main
	LD_LIBRARY_PATH=lib ./$(EXEC)

mrproper:
	rm -f $(EXEC)
	cd $(LIB_DIR) && $(MAKE) mrproper