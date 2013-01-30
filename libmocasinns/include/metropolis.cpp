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
      double delta_E = next_step.delta_E();
      double random_accept = this->rng->random_double();
      if (delta_E <= 0 || random_accept < exp(-beta * delta_E))
      {
	next_step.execute();
      }
    }
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
