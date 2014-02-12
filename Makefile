CC            = gcc
CFLAGS        = -Wall -O -std=c99
LDFLAGS       = -I$(LIB_DIR) -L$(LIB_DIR) -l$(LIB_TIMETABLE)
LIB_DIR       = lib
LIB_TIMETABLE = timetable
TEST_LIB_DIR  = tests/lib


all:
	$(MAKE) -C $(LIB_DIR)

lib_test:
	$(MAKE) -C $(TEST_LIB_DIR) test

clean:
	$(MAKE) -C $(LIB_DIR) clean
	$(MAKE) -C $(TEST_LIB_DIR) clean

mrproper:
	$(MAKE) -C $(LIB_DIR) mrproper
	$(MAKE) -C $(TEST_LIB_DIR) mrproper
