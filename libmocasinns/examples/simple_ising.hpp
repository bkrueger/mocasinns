/*! 
 * \file simple_ising.hpp File containing a configuration class and a step class providing all necessary functions for working with Mocasinns
 */

#include <vector>

//! Forward declaration of the system
class SimpleIsingSystem;

class SimpleIsingStep
{
private:
  //! Index where the flip should take place
  unsigned int flip_index;
  //! Energy difference of the flip
  int energy_difference;
  //! System where the step belongs to
  SimpleIsingSystem* step_system;

public:
  //! Constructor
  SimpleIsingStep(unsigned int new_flip_index, int new_energy_difference, SimpleIsingSystem* new_step_system) :
    flip_index(new_flip_index), energy_difference(new_energy_difference), step_system(new_step_system) {}

  //! Get-Accessor for the index
  unsigned int get_flip_index() { return flip_index; } 

  //! Execute the step
  void execute();
  //! Calculate the energy difference
  int delta_E() const { return energy_difference; }

  //! Flag whether the step is executable
  bool is_executable() const { return true; }
  //! Ratio of the selection probabilities
  double selection_probability_factor() const { return 1.0; }
};

//! A very simple Ising system that can be used as a prototype for configuration spaces that can be used in mocasinns. Provides all necessary functions for using in mocasinns.
class SimpleIsingSystem
{
private:
  std::vector<int> spins;

public:
  //! Constructor for serialisation
  SimpleIsingSystem() : spins() {}
  //! Constructor taking the length of the 1d-Ising system and setting all spins to one
  SimpleIsingSystem(unsigned int length) : spins(length, 1.0) {}
  
  //! Calculate the energy of the system
  int energy() const;

  //! Propose a possible step
  template<class RandomNumberGenerator> 
  SimpleIsingStep propose_step(RandomNumberGenerator* rng); 

  //! Commits a SimpleIsingStep to the system
  void commit(SimpleIsingStep& step_to_commit);

  //! Placeholder for serialization
  template <class Archiv>
  void serialize(Archiv&, const unsigned int&) {}
};

//! Implementation of SimpleIsingStep::commit
void SimpleIsingStep::execute() 
{ 
  step_system->commit(*this); 
}

//! Implementation of SimpleIsingSystem::energy
int SimpleIsingSystem::energy() const
{
   int result = 0;
    for (unsigned int i = 0; i < spins.size() - 1; ++i)
      result += spins[i] * spins[i+1];

    return -result - spins[0]*spins[spins.size() - 1];
}
//! Implementation of SimpleIsingSytem::propose_step
template<class RandomNumberGenerator>
SimpleIsingStep SimpleIsingSystem::propose_step(RandomNumberGenerator* rng)
{
  //! Calculate a random number between 0 and spins.size() - 1
  unsigned int flip_index = rng->random_uint32(0, spins.size() - 1);
  //! Calculate the next neighbours
  int flip_index_lower = flip_index - 1;
  if (flip_index_lower == -1) flip_index_lower = spins.size() - 1;
  int flip_index_upper = flip_index + 1;
  if (flip_index_upper == (int)spins.size()) flip_index_upper = 0;
  //! Calculate the energy difference
  int energy_difference = 2*spins[flip_index]*(spins[flip_index_lower] + spins[flip_index_upper]);

  // Create and return a new step
  return SimpleIsingStep(flip_index, energy_difference, this);
}
//! Implementation of SimpleIsingSystem::commit
void SimpleIsingSystem::commit(SimpleIsingStep& step_to_commit)
{
  spins[step_to_commit.get_flip_index()] *= -1;
}
