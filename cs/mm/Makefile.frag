
LINK_LIB = -lpthread
LINK_TCMALLOC = -ltcmalloc_minimal

BIN = frag.out tcmalloc_frag.out

all: $(BIN)

frag.out: frag_thread.cpp
	g++ -o $@ $^ $(LINK_LIB)

tcmalloc_frag.out: frag_thread.cpp
	g++ -o $@ $^ $(LINK_LIB) $(LINK_TCMALLOC)

clean:
	rm -rf $(BIN)

