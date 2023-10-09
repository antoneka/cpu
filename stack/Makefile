CC=g++
CFLAGS=-std=c++17 -Wall -Werror -Wextra
SRC=*.cpp
TARGET=stack

.PHONY: clean

all: stack

stack: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean: 
	rm -f $(TARGET) stack_log.txt
