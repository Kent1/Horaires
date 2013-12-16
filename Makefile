CC            = gcc
CFLAGS        = -Wall -O -std=c99
LDFLAGS       = -I$(LIB_DIR) -L$(LIB_DIR) -l$(LIB_TIMETABLE)
EXEC          = timetable
MAIN          = main.c
LIB_DIR       = lib
LIB_TIMETABLE = timetable


all:
	$(MAKE) -C $(LIB_DIR)

main: all
	$(CC) -o $(EXEC) $(MAIN) $(CFLAGS) $(LDFLAGS)

launch: main
	LD_LIBRARY_PATH=$(LIB_DIR) ./$(EXEC)

clean:
	$(MAKE) -C $(LIB_DIR) clean

mrproper:
	rm -f $(EXEC)
	$(MAKE) -C $(LIB_DIR) mrproper
