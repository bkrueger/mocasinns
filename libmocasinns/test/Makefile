CC = gcc
CXX = g++
OPT=-O2
CFLAGS = -g -pg $(OPT) -Wall -Wextra -pedantic -std=c++0x
LDFLAGS = -L../../libising/lib -L../../librandom/lib
INCLUDE = -I../include -I../../libising/src -I../../librandom/include

TEST_LIBS = -lising -lrandom -lboost_serialization -lboost_signals -lcppunit -ldl
TEST_OBJECTS = test_simulation.o test_metropolis.o test_wang_landau.o test_binnings.o test_histobase.o test_histocrete.o test_histogram.o test_histogram_number.o test.o

all: test

test: $(TEST_OBJECTS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(INCLUDE) $(TEST_OBJECTS) $(TEST_LIBS) -o test

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) -c $<

clean:
	rm -f *.o test
