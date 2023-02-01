CC = gcc
SRC = $(wildcard src/*/*.c) $(wildcard src/*/*/*.c)
OBJ = $(SRC:.c=.o)
LIB = lib/libhttpheader.so
BIN = test
CFLAGS = -Wall -Wextra -Werror -pedantic -fPIC

%.o: %.c
	$(CC) $(CFLAGS) -g -c $< -o $@

$(LIB): $(OBJ)
	$(CC) $(OBJ) -shared -o $(LIB)
	@cp src/http_header/http_header.h include/http_util
	@cp src/util/errcode.h include/http_util

$(BIN): $(LIB)
	$(CC) src/test.c -o $(BIN) -Llib -lhttpheader -lcontainers -Iinclude -Wl,-rpath,lib

.PHONY: clean install
clean:
	@rm $(OBJ)
	@rm $(LIB)
	@rm $(BIN)
install:
	@cp -r include/http_util ~/.local/include
	@cp $(LIB) ~/.local/lib
