CC = gcc
CXX = g++
OPT=-O2
CFLAGS = -g -pg $(OPT) -Wall -Wextra -pedantic -std=c++0x -fopenmp
LDFLAGS = -L../../libising/lib -L../../librandom/lib
INCLUDE = -I../include -I../../libising/src -I../../librandom/include

TEST_LIBS = -lising -lrandom -lboost_serialization -lboost_signals -lcppunit -ldl
TEST_OBJECTS_MAIN = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
TEST_OBJECTS_HISTOGRAMS = $(patsubst %.cpp,%.o,$(wildcard test_histograms/*.cpp))
TEST_OBJECTS_OBSERVABLES = $(patsubst %.cpp,%.o,$(wildcard test_observables/*.cpp))
TEST_OBJECTS = $(TEST_OBJECTS_MAIN) $(TEST_OBJECTS_HISTOGRAMS) $(TEST_OBJECTS_OBSERVABLES)

all: test

test: $(TEST_OBJECTS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(INCLUDE) $(TEST_OBJECTS) $(TEST_LIBS) -o test

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) -o $@ -c $<

clean:
	rm -f *.o test
