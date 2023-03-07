CC = gcc
SRC = $(wildcard src/*/*.c) $(wildcard src/*/*/*.c)
OBJ = $(SRC:.c=.o)
LIB = lib/libhttputil.so
BIN = test
CFLAGS = -Wall -Wextra -Werror -pedantic -fPIC

%.o: %.c
	$(CC) $(CFLAGS) -g -c $< -o $@ -Iinclude

$(LIB): $(OBJ)
	@cp src/http_header/http_header.h include/http_util
	@cp src/url/url.h include/http_util
	@cp src/util/errcode.h include/http_util
	@cp src/util/containers.h include/http_util
	$(CC) $(OBJ) -shared -o $(LIB) -lcontainers


$(BIN): $(LIB)
	$(CC) src/test.c -o $(BIN) -Llib -lhttputil -lcontainers -Iinclude -Wl,-rpath,lib

.PHONY: clean install
clean:
	@rm $(OBJ)
	@rm $(LIB)
	@rm $(BIN)
install:
	@cp -r include/http_util ~/.local/include
	@cp $(LIB) ~/.local/lib
uninstall:
	@rm -rf ~/.local/include/http_util
	@rm -rf ~/.local/$(LIB)
