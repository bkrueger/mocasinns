CC = gcc
CXX = g++
OPT=-O2
CFLAGS = -g -pg $(OPT) -Wall -Wextra -pedantic -std=c++0x -fopenmp
INCLUDE = -I../include -I../../libgespinst/include -I../../librandom/include

TEST_LIBS = -lboost_serialization -lboost_signals -lboost_program_options -lcppunit -ldl
#TEST_OBJECTS_MAIN = test.o test_simulation.o test_configuration_test.o test_metropolis.o test_metropolis_parallel.o test_entropic_sampling.o test_wang_landau.o test_optimal_ensemble_sampling.o
TEST_OBJECTS_MAIN = $(patsubst %.cpp,%.o,$(wildcard ./*.cpp))
TEST_OBJECTS_ACCUMULATORS = $(patsubst %.cpp,%.o,$(wildcard test_accumulators/*.cpp))
TEST_OBJECTS_ANALYSIS = $(patsubst %.cpp,%.o,$(wildcard test_analysis/*.cpp))
TEST_OBJECTS_HISTOGRAMS = $(patsubst %.cpp,%.o,$(wildcard test_histograms/*.cpp))
TEST_OBJECTS_OBSERVABLES = $(patsubst %.cpp,%.o,$(wildcard test_observables/*.cpp))
TEST_OBJECTS_ENERGY_TYPES = $(patsubst %.cpp,%.o,$(wildcard test_energy_types/*.cpp))
TEST_OBJECTS_DETAILS_STL_EXTENSIONS = $(patsubst %.cpp,%.o,$(wildcard test_details/test_stl_extensions/*.cpp))
TEST_OBJECTS_DETAILS_PARALLEL_TEMPERING = $(patsubst %.cpp,%.o,$(wildcard test_details/test_parallel_tempering/*.cpp))

TEST_OBJECTS = $(TEST_OBJECTS_MAIN) $(TEST_OBJECTS_ACCUMULATORS) $(TEST_OBJECTS_HISTOGRAMS) $(TEST_OBJECTS_ENERGY_TYPES) $(TEST_OBJECTS_OBSERVABLES) $(TEST_OBJECTS_DETAILS_STL_EXTENSIONS) $(TEST_OBJECTS_DETAILS_PARALLEL_TEMPERING) $(TEST_OBJECTS_ANALYSIS)

all: test

test: $(TEST_OBJECTS)
	$(CXX) $(CFLAGS) $(INCLUDE) $(TEST_OBJECTS) $(TEST_LIBS) -o test

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) -o $@ -c $<

clean:
	rm -f $(TEST_OBJECTS) *.o test

compile_tests: compile_tests/compile_test.o
	$(CXX) $(CFLAGS) $(INCLUDE) -o compile_tests/compile_test.o -c compile_tests/compile_test.cpp
