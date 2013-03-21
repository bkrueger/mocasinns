#ifndef MOCASINNS_ACCUMULATORS_FILE_ACCUMULATOR_HPP
#define MOCASINNS_ACCUMULATORS_FILE_ACCUMULATOR_HPP

#include <fstream>
#include <cstdio>

namespace Mocasinns
{
  namespace Accumulators
  {
    //! Class template for accumulating measured observables into a file.
    /*!
      \details The class caches the observables in a vector, After the vector has reached a predefined size, the values are written into the specified file.
      
      \tparam Observable Type of the observables that should be accumulated in the file, must provide an << operator.
    */
    template <class Observable>
    class FileAccumulator
    {
    public:
      //! Typedef for the size
      typedef uint64_t SizeType;

      //! Default constructor
      FileAccumulator() : filename("file_accumulator_data.dat"), cache_size(20), count(0) 
      { 
	cache.reserve(cache_size); 
	clear_file();
      }
      //! Constructor specifying the filename
      FileAccumulator(std::string filename_store) : filename(filename_store), cache_size(20), count(0) 
      { 
	cache.reserve(cache_size); 
	clear_file();
      }
      //! Destructor, writing the remaining cache elements to file
      ~FileAccumulator() { write_to_file(); }

      //! Get-Accessor for the cache size
      const SizeType& get_cache_size() const { return cache_size; }
      //! Set-Accessor for the cache size
      void set_cache_size(const SizeType& value) { cache_size = value; }
      //! Get-Accessor for the count
      const SizeType& get_count() const { return count; }
      //! Get-Accessor for the filename
      const std::string& get_filename() const { return filename; }

      //! Accumulating operator
      void operator()(const Observable& obs)
      {
	cache.push_back(obs);
	count++;
	if (cache.size() >= cache_size) write_to_file();
      }

      //! Return data as a vector
      std::vector<Observable> data() const
      {
	std::vector<Observable> result;
	result.reserve(count);

	// Open the file and read the data to the vector
	std::ifstream input_stream(filename.c_str());
	Observable line_data;
	while (input_stream >> line_data) result.push_back(line_data);

	// Append the cache vector
	result.insert(result.end(), cache.begin(), cache.end());

	return result;
      }

    private:
      //! Filename where to store the values
      std::string filename;
      
      //! Vector for caching the measurements
      std::vector<Observable> cache;
      //! Number of entries to cache maximal
      unsigned int cache_size;

      //! Number of measurments accumulated
      unsigned long int count;

      //! Deletes all data in the file specified by the file-name
      void clear_file()
      {
	remove(filename.c_str());
      }

      //! Write the cache to file and reset the cache
      void write_to_file()
      {
	// Write the cache to the file
	std::ofstream output_stream(filename.c_str(), std::ios_base::app);
	for (typename std::vector<Observable>::const_iterator cache_it = cache.begin(); cache_it != cache.end(); ++cache_it)
	{
	  output_stream << *cache_it << std::endl;
	}

	// Clear the cache
	cache.clear();

	// Close the stream
	output_stream.close();
      }
    };
  }
}

#endif
