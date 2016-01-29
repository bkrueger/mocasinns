#ifndef SIMPLE_ISING_2D_HPP
#define SIMPLE_ISING_2D_HPP

#include <vector>

class IsingConfiguration2d;

// Class representing a single step in an Ising chain
class IsingStep2d
{
public:
  // Pointer to the ising configuration to which the spin belongs
  IsingConfiguration2d* configuration;
  // Index that specifies the index of the spin that will be flipped by this step
  unsigned int flip_index_x;
  unsigned int flip_index_y;

  // Constructor taking the configuration and the spin index of the flip
  IsingStep2d(IsingConfiguration2d* config, unsigned int index_x, unsigned int index_y) : configuration(config), flip_index_x(index_x), flip_index_y(index_y) { }

  // Calculates the energy difference induced by this step
  int delta_E();

  // Execute the flip
  void execute();
};

// Class representing a one-dimensiona chain of ising spins
class IsingConfiguration2d
{
public:
  // System sizes
  unsigned int size_x;
  unsigned int size_y;

  // The spins are stored as vectors of -1 and 1
  std::vector<std::vector<int> > spins;

  // External magnetic field
  double external_field;

  // Default constructor, needed for serialization
  IsingConfiguration2d() : external_field(0) { }
  // Create an Ising configuration with a certain number of spins
  IsingConfiguration2d(unsigned int length_x, unsigned int length_y) : size_x(length_x), 
								       size_y(length_y),
								       spins(length_x, std::vector<int>(length_y, 1)), 
								       external_field(0) { }

  // Apply a given flip to the configuration
  void commit(const IsingStep2d& step) { spins[step.flip_index_x][step.flip_index_y] *= -1; }

  // Calculate the magnetization
  int magnetization()
  {
    int result = 0;
    for (unsigned int i = 0; i < size_x; ++i)
      for (unsigned int j = 0; j < size_y; ++j)
	result += spins[i][j];
    return result;
  }

  // Calculate the energy (sum over products of nearest neighbour spins) of the chain
  int energy()
  {
    int result = 0;
    for (unsigned int i = 0; i < size_x; ++i)
      for (unsigned int j = 0; j < size_y; ++j)
      {
	unsigned int neighbour_x_lower = (i == 0 ? size_x - 1 : i - 1);
	unsigned int neighbour_x_upper = (i == size_x - 1 ? 0 : i + 1);
	unsigned int neighbour_y_lower = (j == 0 ? size_y - 1 : j - 1);
	unsigned int neighbour_y_upper = (j == size_y - 1 ? 0 : j + 1);

	// Interaction
	result -= 0.5 * spins[i][j] * (spins[neighbour_x_lower][j] + spins[neighbour_x_upper][j] + 
				       spins[i][neighbour_y_lower] + spins[i][neighbour_y_lower]);
	// External field
	result -= external_field * spins[i][j];
      }

    return result;
  }

  // Create a step (spin flip) using a given random number generator
  template <class RandomNumberGenerator>
  IsingStep2d propose_step(RandomNumberGenerator* rng) { return IsingStep2d(this, rng->random_int32(0, size_x - 1), rng->random_int32(0, size_y - 1)); }
};
  
int IsingStep2d::delta_E()
{
  unsigned int neighbour_x_lower = (flip_index_x == 0 ? configuration->size_x - 1 : flip_index_x - 1);
  unsigned int neighbour_x_upper = (flip_index_x == configuration->size_x - 1 ? 0 : flip_index_x + 1);
  unsigned int neighbour_y_lower = (flip_index_y == 0 ? configuration->size_y - 1 : flip_index_y - 1);
  unsigned int neighbour_y_upper = (flip_index_y == configuration->size_y - 1 ? 0 : flip_index_y + 1);
  
  // Calculate the energy difference
  return 2*configuration->spins[flip_index_x][flip_index_y]*(configuration->spins[flip_index_x][neighbour_y_lower] + 
							     configuration->spins[flip_index_x][neighbour_y_upper] + 
							     configuration->spins[neighbour_x_lower][flip_index_y] + 
							     configuration->spins[neighbour_x_upper][flip_index_y])
    + 2 * configuration->external_field * configuration->spins[flip_index_x][flip_index_y];
}

void IsingStep2d::execute() { configuration->commit(*this); }

#endif
