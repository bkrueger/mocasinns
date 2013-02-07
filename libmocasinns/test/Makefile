CC = gcc
CXX = g++
OPT=-O2
CFLAGS = -g -pg $(OPT) -Wall -Wextra -pedantic -std=c++0x
LDFLAGS = -L../../libising/lib -L../../librandom/lib
INCLUDE = -I../include -I../../libising/src -I../../librandom/include

TEST_LIBS = -lising -lrandom -lboost_serialization -lboost_signals -lcppunit -ldl
TEST_OBJECTS = test_simulation.o test_configuration_test.o test_metropolis.o test_wang_landau.o test_histograms/test_binnings.o test_histograms/test_histobase.o test_histograms/test_histocrete.o test_histograms/test_histogram.o test_histograms/test_histogram_number.o test_observables/test_vector_observable.o test.o

all: test

test: $(TEST_OBJECTS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(INCLUDE) $(TEST_OBJECTS) $(TEST_LIBS) -o test

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) -o $@ -c $<

clean:
	rm -f *.o test
