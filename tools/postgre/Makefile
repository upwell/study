CPP = g++

<<<<<<< HEAD
PQ_PATH = /root/install/postgre
=======
PQ_PATH = $(HOME)/postgre
>>>>>>> 1ee6351c58b3da84b7ce0870ad8ceb6229b96351
PQ_INCLUDE = $(PQ_PATH)/include

PQ_LDFLAGS = -L$(PQ_PATH)/lib -lpq

BIN = connect.out init.out bench_add.out bench_search.out bench_batch_add.out
THREAD_BIN = thread_add.out thread_search.out

all: $(BIN) $(THREAD_BIN)

$(BIN) : %.out: %.cpp
	$(CPP) -I$(PQ_INCLUDE) $^ -o $@ $(PQ_LDFLAGS)

$(THREAD_BIN) : %.out: %.cpp
	$(CPP) -I$(PQ_INCLUDE) $^ -o $@ $(PQ_LDFLAGS) -lpthread
