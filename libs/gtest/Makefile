CXX = g++
CXXFLAG = -g -Wall
LDFLAG = 

TEST = foo_unit

GTEST_LIB = /usr/lib/libgtest_main.a /usr/lib/libgtest.a
GTEST_LINK = -lpthread

all: $(TEST)

foo.o: foo.cpp
	$(CXX) $(CXXFLAG) -c $^

foo_unit.o: foo_unit.cpp
	$(CXX) $(CXXFLAG) -c $^

foo_unit: foo.o foo_unit.o
	$(CXX) $(CXXFLAG) $^ $(GTEST_LIB) -o $@ $(GTEST_LINK)

clean:
	rm -rf $(TEST) *.o
