CC=g++
CFLAGS=-std=c++17 -Wall -Werror -Wextra
SRC=*.cpp ../include/common.cpp
TARGET=disasm

.PHONY: clean

all: disasm

disasm: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean: 
	rm -f $(TARGET) disasm_file.txt
