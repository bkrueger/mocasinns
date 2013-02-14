#ifndef MOCASINNS_OBSERVABLES_HISTOGRAM_ACCUMULATOR_HPP
#define MOCASINNS_OBSERVABLES_HISTOGRAM_ACCUMULATOR_HPP

namespace Mocasinns
{
  namespace Observables
  {
    //! Class template for accumulating measured observables into a histogram.
    /*!
      \details This class template can be used with various histogram types, especially with the histograms in the namespace Mocasinns::Histograms.
      
      \tparam Histo Class template for a histogram, use e.g. the histograms defined in Mocasinns::Histograms. The first template parameter coresponds to the type of the x-values, the second template parameteres corresponds to the type of the y-values of the histogram.
      \tparam Observable Type of the observables that should be accumulated in the histogram, this is the type of the x-values of the histogram
    */
    template <template <class,class> class Histo, class Observable>
    class HistogramAccumulator : public Histo<Observable, unsigned long int>
    {
    public:
      typedef Histo<Observable, unsigned long int> BaseHisto;

      //! Default constructor
      HistogramAccumulator() : BaseHisto () {}
      //! Constructor taking a base object
      HistogramAccumulator(const BaseHisto& base_histo) : BaseHisto(base_histo) {}
      //! Copy constructor
      HistogramAccumulator(const HistogramAccumulator<Histo, Observable>& other) : BaseHisto(static_cast<BaseHisto>(other)) {}

      //! Accumulating operator
      void operator()(const Observable& obs)
      {
	*this << obs;
      }

      //! Calculate and return the normalized histogram
      Histo<Observable, double> normalized_histogram() const
      {
	// Calculate the sum of all histogram entries
	unsigned long int counter = this->sum();

	// Define the result and set the binning
	Histo<Observable, double> result;
	result.set_binning(this->get_binning_width(), this->get_binning_reference());

	// Calculate the normalized histogram
	for (typename BaseHisto::const_iterator it = this->begin(); it != this->end(); ++it)
	{
	  result[it->first] = static_cast<double>(it->second)/static_cast<double>(counter);
	}

	// Return the normalized histogram
	return result;
      }
    };
  }
}

#endif
