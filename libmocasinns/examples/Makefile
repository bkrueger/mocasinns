PROGRAMS=simple_ising_metropolis simple_ising_metropolis_observator simple_ising_analysis simple_ising_parallel_tempering simple_ising_parallel_tempering_temp_optimization simple_ising_wang_landau

all: $(PROGRAMS)

simple_ising_metropolis: simple_ising.hpp simple_ising_metropolis.cpp
	g++ -std=c++11 -I../include simple_ising_metropolis.cpp -lboost_serialization -o simple_ising_metropolis

simple_ising_metropolis_observator:
	g++ -std=c++11 -I../include simple_ising_metropolis_observator.cpp -lboost_serialization -o simple_ising_metropolis_observator

simple_ising_analysis: simple_ising.hpp simple_ising_analysis.cpp
	g++ -std=c++11 -I../include simple_ising_analysis.cpp -lboost_serialization -o simple_ising_analysis

simple_ising_parallel_tempering: simple_ising.hpp simple_ising_parallel_tempering.cpp
	g++ -std=c++11 -I../include simple_ising_parallel_tempering.cpp -lboost_serialization -fopenmp -o simple_ising_parallel_tempering

simple_ising_parallel_tempering_temp_optimization: simple_ising.hpp simple_ising_parallel_tempering_temp_optimization.cpp
	g++ -std=c++11 -I../include simple_ising_parallel_tempering_temp_optimization.cpp -lboost_serialization -fopenmp -o simple_ising_parallel_tempering_temp_optimization

simple_ising_wang_landau: simple_ising.hpp simple_ising_wang_landau.cpp
	g++ -std=c++11 -I../include simple_ising_wang_landau.cpp -lboost_serialization -o simple_ising_wang_landau

clean:
	rm $(PROGRAMS)