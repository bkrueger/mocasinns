CXX = g++
OPT=-O2
CFLAGS = $(OPT) -Wall -Wextra -pedantic -std=c++0x -fopenmp

LDFLAGS = -L../libising/lib -L../librandom/lib
INCLUDE = -I../libising/src -I../librandom/include -I../libmocasinns/include

LIBRARIES = -lising -lrandom -lboost_serialization -lboost_signals

all: metropolis simple_ising_metropolis

metropolis: metropolis_simple metropolis_accumulator metropolis_histogram

metropolis_%: metropolis_%.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $(INCLUDE) $< $(LIBRARIES) -o $@

simple_ising_metropolis: simple_ising_metropolis.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $(INCLUDE) $< $(LIBRARIES) -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) -o $@ -c $<