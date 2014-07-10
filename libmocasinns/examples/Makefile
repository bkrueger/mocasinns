PROGRAMS=simple_ising_metropolis simple_ising_metropolis_observator simple_ising_analysis

all: $(PROGRAMS)

simple_ising_metropolis: simple_ising.hpp simple_ising_metropolis.cpp
	g++ -std=c++11 -I../include simple_ising_metropolis.cpp -lboost_serialization -o simple_ising_metropolis

simple_ising_metropolis_observator:
	g++ -std=c++11 -I../include simple_ising_metropolis_observator.cpp -lboost_serialization -o simple_ising_metropolis_observator

simple_ising_analysis: simple_ising.hpp simple_ising_analysis.cpp
	g++ -std=c++11 -I../include simple_ising_analysis.cpp -lboost_serialization -o simple_ising_analysis

clean:
	rm $(PROGRAMS)