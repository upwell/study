CPP = g++

PQ_PATH = $(HOME)/install/postgre
PQ_INCLUDE = $(PQ_PATH)/include

PQ_LDFLAGS = -L$(PQ_PATH)/lib -lpq

BIN = connect.out init.out bench_add.out bench_search.out bench_batch_add.out bench_batch_add2.out bench_batch_add3.out

THREAD_BIN = thread_add.out thread_search.out

all: $(BIN) $(THREAD_BIN)

$(BIN) : %.out: %.cpp
	$(CPP) -I$(PQ_INCLUDE) $^ -o $@ $(PQ_LDFLAGS)

$(THREAD_BIN) : %.out: %.cpp
	$(CPP) -I$(PQ_INCLUDE) $^ -o $@ $(PQ_LDFLAGS) -lpthread

clean:
	- rm -rf *.out
