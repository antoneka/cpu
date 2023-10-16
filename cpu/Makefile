CC=g++
CFLAGS=-std=c++17 -Wall -Werror -Wextra 
SRC=*.cpp ../include/common.cpp ../stack/stack.cpp ../stack/stack_debug.cpp ../stack/hash_rot13.cpp
TARGET=cpu

.PHONY: clean

all: cpu

cpu: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean: 
	rm -f $(TARGET) 
