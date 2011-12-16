CPP = g++

BOOSTLDFLAGS = -lboost_thread -lboost_filesystem -lboost_program_options
BOOSTLDFLAGS += -lboost_system

MONGOLDFLAGS = -lmongoclient

BIN = conn.out insert.out bench_add.out bench_search.out
THREAD_BIN = thread_add.out thread_search.out

all: $(BIN) $(THREAD_BIN)

$(BIN): %.out: %.cpp
	$(CPP) $^ -o $@ $(MONGOLDFLAGS) $(BOOSTLDFLAGS)

$(THREAD_BIN): %.out: %.cpp
	$(CPP) $^ -o $@ $(MONGOLDFLAGS) $(BOOSTLDFLAGS) -lpthread
