CC=g++
CFLAGS=-std=c++17 -Wall -Werror -Wextra 
SRC=*.cpp ../include/common.cpp
TARGET=asm

.PHONY: clean

all: asm

asm: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean: 
	rm -f $(TARGET) bytecode_file.txt
