CPP = g++

BOOSTLDFLAGS = -lboost_thread -lboost_filesystem -lboost_program_options
BOOSTLDFLAGS += -lboost_system

MONGOLDFLAGS = -lmongoclient

BIN = conn.out insert.out bench_add.out bench_search.out

all: $(BIN)

$(BIN): %.out: %.cpp
	g++ $^ -o $@ $(MONGOLDFLAGS) $(BOOSTLDFLAGS)
