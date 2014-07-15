PROGRAMS=metropolis metropolis_rejection_free observables signal_handlers analysis accumulator parallel_tempering inverse_temperature_optimization entropic_sampling wang_landau metropolis_hastings

all: $(PROGRAMS)

metropolis: simple_ising.hpp metropolis.cpp
	g++ -std=c++11 -I../include metropolis.cpp -lboost_serialization -o metropolis

metropolis_rejection_free: simple_ising_rejection_free.hpp metropolis_rejection_free.cpp
	g++ -std=c++11 -I../include metropolis_rejection_free.cpp -lboost_serialization -o metropolis_rejection_free

observables: simple_ising.hpp observables.cpp
	g++ -std=c++11 -I../include observables.cpp -lboost_serialization -o observables

signal_handlers: simple_ising.hpp signal_handlers.cpp
	g++ -std=c++11 -I../include signal_handlers.cpp -lboost_serialization -o signal_handlers

analysis: simple_ising.hpp analysis.cpp
	g++ -std=c++11 -I../include analysis.cpp -lboost_serialization -o analysis

accumulator: simple_ising.hpp accumulator.cpp
	g++ -std=c++11 -I../include accumulator.cpp -lboost_serialization -o accumulator

parallel_tempering: simple_ising.hpp parallel_tempering.cpp
	g++ -std=c++11 -I../include parallel_tempering.cpp -lboost_serialization -fopenmp -o parallel_tempering

inverse_temperature_optimization: simple_ising.hpp inverse_temperature_optimization.cpp
	g++ -std=c++11 -I../include inverse_temperature_optimization.cpp -lboost_serialization -fopenmp -o inverse_temperature_optimization

entropic_sampling: simple_ising.hpp entropic_sampling.cpp
	g++ -std=c++11 -O3 -I../include entropic_sampling.cpp -lboost_serialization -o entropic_sampling

wang_landau: simple_ising.hpp wang_landau.cpp
	g++ -std=c++11 -I../include wang_landau.cpp -lboost_serialization -o wang_landau

metropolis_hastings: simple_ising.hpp metropolis_hastings.cpp
	g++ -std=c++11 -I../include metropolis_hastings.cpp -lboost_serialization -o metropolis_hastings

clean:
	rm $(PROGRAMS)