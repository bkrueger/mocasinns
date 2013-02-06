/*!
 * \file metropolis.cpp
 * \brief Implementation of the libMoCaSinns template interface
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Johannes F. Knauf
 * \author Benedikt Krüger
 */

#ifdef METROPOLIS_HPP

#include <cmath>

namespace Mocasinns
{

/*!
 * \param num_steps Number of Metropolis steps that will be performed
 * \param beta Inverse temperature that will be used for calculation of the acceptance probability of the Metropolis steps.
 */
template <class ConfigurationType, class Step, class RandomNumberGenerator>
template <class TemperatureType>
void Metropolis<ConfigurationType, Step, RandomNumberGenerator>::do_metropolis_steps(const uint32_t& num_steps, const TemperatureType& beta = 0)
{
  for (uint32_t i = 0; i < num_steps; i++)
  {
    Step next_step = this->configuration_space->propose_step(this->rng);

    if (next_step.is_executable())
    {
      // One does need to define this in advance, because the Temperature Type need not to be comparable to an double 0.0
      double beta_times_delta_E = beta*next_step.delta_E();
      double selection_probability_factor = next_step.selection_probability_factor();
      double random_accept = this->rng->random_double();
      if (beta_times_delta_E <= -log(selection_probability_factor) || random_accept < (1.0/selection_probability_factor)*exp(-beta_times_delta_E))
      {
	next_step.execute();
      }
    }
  }
}

/*!
  \tparam ObservableFunctor Class with operator() taking a pointer to the simulation and returning the value of a arbitrary observable. The class must contain a typedef ::observable_type classifying the return type of the functor.
  \tparam TemperatureType Type of the inverse temperature, there must be an operator* defined this class and the energy type of the configuration.
  \param parameters Parameters to use for the simulation
  \param beta Inverse temperature at which the simulation is performed.
  \returns Vector containing the single measurements performed
*/
template<class ConfigurationType, class Step, class RandomNumberGenerator>
template<class ObservableFunctor, class TemperatureType>
std::vector<typename ObservableFunctor::observable_type> Metropolis<ConfigurationType, Step, RandomNumberGenerator>::do_metropolis_simulation(const Parameters& parameters, const TemperatureType& beta)
{
  // Perform the relaxation steps
  do_metropolis_steps(parameters.relaxation_steps, beta);
  
  // Create a vector for storing the measurements
  std::vector<typename ObservableFunctor::observable_type> measurements;
  
  // For each measurement, perform the steps and take the measurement
  for (unsigned int m = 0; m < parameters.measurement_number; ++m)
  {
    do_metropolis_steps(parameters.steps_between_measurement, beta);
    measurements.push_back(ObservableFunctor(this->configuration_space));
  }
  
  // Return the measurments
  return measurments;
}

/*!
  \tparam ObservableFunctor Class with operator() taking a pointer to the simulation and returning the value of a arbitrary observable. The class must contain a typedef ::observable_type classifying the return type of the functor.
  \tparam InputIterator Type of the iterator that iterates the different temperatures that will be considered
  \param parameters Parameters to use for the simulation
  \param beta_begin Iterator pointing to the first inverse temperature that is calculated
  \param beta_end Iterator pointing on position after the last inverse temperature that is calculated
  \returns Vector containing the vectors of measurments performed for each temperature. (First index: inverse temperature, second index: measurment number)
*/
template<class ConfigurationType, class Step, class RandomNumberGenerator>
template<class ObservableFunctor, class InputIterator>
std::vector<std::vector<typename ObservableFunctor::observable_type> > Metropolis<ConfigurationType, Step, RandomNumberGenerator>::do_metropolis_simulation(const Parameters& parameters, InputIterator first_beta, InputIterator last_beta)
{
  std::vector<std::vector<typename ObservableFunctor::observable_type> > results;
  for (InputIterator beta = first_beta; beta != last_beta; ++beta)
  {
    results.push_back(do_metropolis_simulation(parameters, *beta));
  }
}  

template <class ConfigurationType, class Step, class RandomNumberGenerator>
void Metropolis<ConfigurationType, Step, RandomNumberGenerator>::load_serialize(std::istream& input_stream)
{
  boost::archive::text_iarchive input_archive(input_stream);
  input_archive >> (*this);
}
template <class ConfigurationType, class Step, class RandomNumberGenerator>
void Metropolis<ConfigurationType, Step, RandomNumberGenerator>::load_serialize(const char* filename)
{
  std::ifstream input_filestream(filename);
  load_serialize(input_filestream);
  input_filestream.close();
}
template <class ConfigurationType, class Step, class RandomNumberGenerator>
void Metropolis<ConfigurationType, Step, RandomNumberGenerator>::save_serialize(std::ostream& output_stream) const
{
  boost::archive::text_oarchive output_archive(output_stream);
  output_archive << (*this);
}
template <class ConfigurationType, class Step, class RandomNumberGenerator>
void Metropolis<ConfigurationType, Step, RandomNumberGenerator>::save_serialize(const char* filename) const
{
  std::ofstream output_filestream(filename);
  save_serialize(output_filestream);
  output_filestream.close();
}

} // of namespace Mocasinns

#endif
