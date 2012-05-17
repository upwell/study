
BIN = tst_srv.out client.out

all: $(BIN)

$(BIN): %.out: %.c
	gcc $^ -o $@

clean:
	rm -rf $(BIN)
