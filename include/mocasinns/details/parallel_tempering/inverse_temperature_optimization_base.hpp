#ifndef MOCASINNS_DETAILS_PARALLEL_TEMPERING_INVERSE_TEMPERATURE_OPTIMIZATION_BASE_HPP
#define MOCASINNS_DETAILS_PARALLEL_TEMPERING_INVERSE_TEMPERATURE_OPTIMIZATION_BASE_HPP

namespace Mocasinns
{
  namespace Details
  {
    namespace ParallelTempering
    {
      template <class ParallelTemperingType, class InverseTemperatureType>
      class InverseTemperatureOptimizationBase
      {
      public:
	typedef typename ParallelTemperingType::ReplicaExchangeLog ReplicaExchangeLog;
	typedef typename ParallelTemperingType::InverseTemperatureHistogram InverseTemperatureHistogram;
	
	InverseTemperatureOptimizationBase(ParallelTemperingType* optimize_simulation, unsigned int opt_steps)
	  : simulation(optimize_simulation), 
	    original_parameters(optimize_simulation->get_simulation_parameters()),
	    replica_number(optimize_simulation->get_config_spaces().size()),
	    optimization_steps(opt_steps)
	{ }
      
	//! Vector that stores the inverse temperatures of the replicas for each recursion step
	const std::vector<std::vector<InverseTemperatureType> >& get_inverse_temperatures() const { return inverse_temperatures; }
	//! Vector that stores the acceptance probabilities of the replica exchanges for each recursion step
	const std::vector<std::vector<double> >& get_acceptance_probabilities() const { return acceptance_probabilities;}

	//! Get-accessor for the number of optimization steps
	unsigned int get_optimization_steps() const { return optimization_steps; }
	//! Set-accessor for the number of optimization steps
	void set_optimization_steps(unsigned int value) { optimization_steps = value; }
	
      protected:
	//! Pointer to the parallel tempering simulation
	ParallelTemperingType* simulation;
	//! Parameters of the original simulation
	typename ParallelTemperingType::Parameters original_parameters;
	//! Number of replicas in the simulations
	unsigned int replica_number;
	//! Number of optimization steps that should be executed
	unsigned int optimization_steps;

	//! Vector that stores the inverse temperatures of the replicas for each recursion step
	std::vector<std::vector<InverseTemperatureType> > inverse_temperatures;
	//! Vector that stores the acceptance probabilities of the replica exchanges for each recursion step
	std::vector<std::vector<double> > acceptance_probabilities;
	
	//! Alter the parameters to perform inverse temperature optimization
	void set_parameters_optimize()
	{
	  typename ParallelTemperingType::Parameters optimization_parameters(original_parameters);
	  optimization_parameters.measurement_number = 1;
	  optimization_parameters.relaxation_steps = 0;
	  simulation->set_simulation_parameters(optimization_parameters);
	}
	//! Undo the change in the parameters
	void unset_parameters_optimize() { simulation->set_simulation_parameters(original_parameters); }

	//! Clear the inverse temperatures and the acceptance probabilities and store the given temperature range in the vector
	template <class InverseTemperatureIterator>
	void initialize_inverse_temperatures(InverseTemperatureIterator inverse_temperatures_begin,
					     InverseTemperatureIterator inverse_temperatures_end)
	{
	  // Clear the replica exchanges, the acceptance probabilities and the inverse temperatures
	  simulation->replica_exchanges_reset();
	  inverse_temperatures.clear();
	  acceptance_probabilities.clear();

	  // Store the given range of inverse temperatures
	  inverse_temperatures.push_back(std::vector<InverseTemperatureType>(inverse_temperatures_begin, inverse_temperatures_end));
	}
	
	//! Mesure the acceptance probabilities. Does simulations until each replica exchange was accepted at least once
	void measure_acceptance_probabilities()
	{
	  unsigned int current_beta = inverse_temperatures.size() - 1;
	
	  while (*std::min_element(simulation->get_replica_exchanges_executed().begin(), simulation->get_replica_exchanges_executed().end()) == 0)
	  {
	    // Do a parallel tempering simulation
	    set_parameters_optimize();
	    simulation->do_parallel_tempering_simulation(inverse_temperatures[current_beta].begin(), inverse_temperatures[current_beta].end());
	    unset_parameters_optimize();
	  }
	}
	//! Logs the acceptance probabilities and resets the replica exchange logs
	void log_acceptance_probabilities()
	{
	  // Log the acceptance probability
	  acceptance_probabilities.push_back(std::vector<double>(replica_number - 1, 0.0));							    
	  ReplicaExchangeLog replica_exchanges_executed = simulation->get_replica_exchanges_executed();
	  ReplicaExchangeLog replica_exchanges_rejected = simulation->get_replica_exchanges_rejected();
	  for (unsigned int r = 0; r < replica_number - 1; ++r)
	  {
	    acceptance_probabilities[acceptance_probabilities.size() - 1][r] = 
	      static_cast<double>(replica_exchanges_executed[r]) / static_cast<double>(replica_exchanges_executed[r] + replica_exchanges_rejected[r]);
	  }
	  
	  // Reset the replica exchanges
	  simulation->replica_exchanges_reset();
	}
      };
    }
  }
}

#endif
