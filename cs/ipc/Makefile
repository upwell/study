CC = gcc

LDFLAGS = -lpthread

BIN = mmap_fork.out mmap_fork_sem.out

all: $(BIN)

$(BIN): %.out: %.c
	$(CC) -o $@ $^ $(LDFLAGS)
