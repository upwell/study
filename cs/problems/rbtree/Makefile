
CC = gcc
CFLAGS = -Wall -O2

SRC = test_rbtree.c ngx_rbtree.c
OBJ = $(patsubst %.c,%.o,$(notdir $(SRC)))

BIN = runtest.out

all: $(BIN)

$(OBJ): %.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN): $(OBJ)
	$(CC) -o $@ $^

clean:
	rm -rf $(OBJ) $(BIN)
