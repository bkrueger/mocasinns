PROGRAMS=metropolis observables analysis parallel_tempering inverse_temperature_optimization entropic_sampling wang_landau

all: $(PROGRAMS)

metropolis: simple_ising.hpp metropolis.cpp
	g++ -std=c++11 -I../include metropolis.cpp -lboost_serialization -o metropolis

observables:
	g++ -std=c++11 -I../include observables.cpp -lboost_serialization -o observables

analysis: simple_ising.hpp analysis.cpp
	g++ -std=c++11 -I../include analysis.cpp -lboost_serialization -o analysis

parallel_tempering: simple_ising.hpp parallel_tempering.cpp
	g++ -std=c++11 -I../include parallel_tempering.cpp -lboost_serialization -fopenmp -o parallel_tempering

inverse_temperature_optimization: simple_ising.hpp inverse_temperature_optimization.cpp
	g++ -std=c++11 -I../include inverse_temperature_optimization.cpp -lboost_serialization -fopenmp -o inverse_temperature_optimization

entropic_sampling: simple_ising.hpp entropic_sampling.cpp
	g++ -std=c++11 -O3 -I../include entropic_sampling.cpp -lboost_serialization -o entropic_sampling

wang_landau: simple_ising.hpp wang_landau.cpp
	g++ -std=c++11 -I../include wang_landau.cpp -lboost_serialization -o wang_landau

clean:
	rm $(PROGRAMS)