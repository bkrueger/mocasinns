CXX = g++
OPT=-O2
CFLAGS = $(OPT) -Wall -Wextra -pedantic -std=c++0x -fopenmp

INCLUDE = -I../libgespinst/include -I../librandom/include -I../libmocasinns/include

LIBRARIES = -lboost_serialization -lboost_signals

all: metropolis wang_landau simple_ising_metropolis

metropolis: metropolis_simple metropolis_accumulator metropolis_histogram
wang_landau: wang_landau_simple

metropolis_%: metropolis_%.o
	$(CXX) $(CFLAGS) $(INCLUDE) $< $(LIBRARIES) -o $@

wang_landau_%: wang_landau_%.o
	$(CXX) $(CFLAGS) $(INCLUDE) $< $(LIBRARIES) -o $@

simple_ising_metropolis: simple_ising_metropolis.o
	$(CXX) $(CFLAGS) $(INCLUDE) $< $(LIBRARIES) -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) -o $@ -c $<

clean:
	rm *.o metropolis_simple metropolis_accumulator metropolis_histogram wang_landau_simple simple_ising_metropolis