
CFLAGS = -std=c++11
LDFLAGS = -lpthread
CPP = g++

SRC_DIR = ./

vpath %.cpp $(SRC_DIR)

CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst %.cpp, %.out, $(CPP_FILES))

all: $(OBJ_FILES)

$(OBJ_FILES): %.out: %.cpp
	$(CPP) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	-rm -rf $(OBJ_FILES)





