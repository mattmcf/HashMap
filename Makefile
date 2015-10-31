# Matt McFarland
# Makefile for Hashmap and its test components

# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -pedantic -std=c11
#CFLAGS = -Wall -pedantic -std=c11 -m32

# Source directory files
SRC_DIR = ./src/

# Source .c files
SRC_FILES = $(SRC_DIR)hashmap.c
SRC_INCS = $(SRC_DIR)hashmap.h

# Should be SRC_FILES with .o
SRC_OBJS = hashmap.o

# Test files
TEST_DIR = ./test/
TEST_SRC = $(TEST_DIR)TestHashMap.c

# Test object files
TEST_OBJS = TestHashMap.o

# Test executable
TEST_EXEC = TestHashMap

$(TEST_EXEC): $(TEST_OBJS) $(SRC_OBJS) 
	$(CC) $(CFLAGS) $(SRC_OBJS) $(TEST_OBJS) -o $(TEST_EXEC) 

$(TEST_OBJS): $(SRC_OBJS)
	$(CC) $(CFLAGS) -c $(TEST_SRC)

$(SRC_OBJS): $(SRC_FILES) $(SRC_INCS)
	$(CC) $(CFLAGS) -c $(SRC_FILES) $(SRC_INCS)

clean:
	rm -f $(SRC_OBJS)
	rm -f $(TEST_EXEC)
	rm -f $(TEST_OBJS)
	rm -f $(SRC_DIR)*.gch
