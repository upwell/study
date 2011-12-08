CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lebtree

EBTREE_PATH = ./ebtree-6.0.6
EBTREE_INCLUDE = -I$(EBTREE_PATH)
EBTREE_LIB = $(EBTREE_PATH)/libebtree.a

SRC = test_ebtree.c

OBJ = $(patsubst %.c,%.o,$(notdir $(SRC)))

BIN = runtest.out

all: $(BIN)

$(OBJ): %.o : %.c
	$(CC) $(CFLAGS) $(EBTREE_INCLUDE) -c -o $@ $<

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(EBTREE_LIB)

clean:
	rm -rf $(OBJ) $(BIN)
