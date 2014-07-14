#ifndef SIMPLE_ISING_HPP
#define SIMPLE_ISING_HPP

#include <vector>

class IsingConfiguration;

// Class representing a single step in an Ising chain
class IsingStep
{
public:
  // Pointer to the ising configuration to which the spin belongs
  IsingConfiguration* configuration;
  // Index that specifies the index of the spin that will be flipped by this step
  unsigned int flip_index;

  // Constructor taking the configuration and the spin index of the flip
  IsingStep(IsingConfiguration* config, unsigned int index) : configuration(config), flip_index(index) { }

  // Calculates the energy difference induced by this step
  int delta_E();

  // Execute the flip
  void execute();
};

// Class representing a one-dimensiona chain of ising spins
class IsingConfiguration
{
public:
  // The spins are stored as vectors of -1 and 1
  std::vector<int> spins;

  // Default constructor, needed for serialization
  IsingConfiguration() { }
  // Create an Ising configuration with a certain number of spins
  IsingConfiguration(unsigned int length) : spins(length, 1) { }

  // Apply a given flip to the configuration
  void commit(const IsingStep& step) { spins[step.flip_index] *= -1; }

  // Calculate the energy (sum over products of nearest neighbour spins) of the chain
  int energy()
  {
    int result = 0;
    for (unsigned int i = 0; i < spins.size() - 1; ++i)
      result += spins[i] * spins[i + 1];
    result += spins[0]*spins[spins.size() - 1];
    return result;
  }

  // Create a step (spin flip) using a given random number generator
  template <class RandomNumberGenerator>
  IsingStep propose_step(RandomNumberGenerator* rng) { return IsingStep(this, rng->random_int32(0, spins.size() - 1)); }
};
  
int IsingStep::delta_E()
{
  // Calculate the indices of the neighbouring spins using the periodic boundary conditions
  unsigned int flip_index_lower;
  if (flip_index == 0) flip_index_lower = configuration->spins.size() - 1;
  else flip_index_lower = flip_index - 1;
  unsigned int flip_index_upper;
  if (flip_index == configuration->spins.size() - 1) flip_index_upper = 0;
  else flip_index_upper = flip_index + 1;
  
    // Calculate the energy difference
  return -2*configuration->spins[flip_index]*(configuration->spins[flip_index_lower] + configuration->spins[flip_index_upper]);
}

void IsingStep::execute() { configuration->commit(*this); }

#endif
