CPP = g++

BOOSTLDFLAGS = -lboost_thread -lboost_filesystem -lboost_program_options
BOOSTLDFLAGS += -lboost_system

MONGOLDFLAGS = -lmongoclient

BIN = conn.out insert.out

all: $(BIN)

conn.out: conn.cpp
	g++ $^ -o $@ $(MONGOLDFLAGS) $(BOOSTLDFLAGS)

insert.out: insert.cpp
	g++ $^ -o $@ $(MONGOLDFLAGS) $(BOOSTLDFLAGS)
