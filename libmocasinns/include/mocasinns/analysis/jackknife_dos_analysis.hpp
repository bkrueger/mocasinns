#ifndef MOCASINNS_ANALYSIS_JACKKNIFE_DOS_ANALYSIS_HPP
#define MOCASINNS_ANALYSIS_JACKKNIFE_DOS_ANALYSIS_HPP

// Boost accumulator
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>
namespace ba = boost::accumulators;

namespace Mocasinns
{
  namespace Analysis
  {
    //! Class for doing a jackknife analysis of a number of data points.
    /*!
      \details 
      
      \tparam Observable Type of the observables that should be jackknifed, must be able to calculate a pow with integers and a sqrt
      \tparam FunctionOfObservables Functor that has a 1) static operator() that takes two Iterators and calculating a function of observables (e.g. the variance or the specific heat) and has 2) a type return_type typedefed that indicates the return type of the functor.
    */
    template <class DosHisto, class DosObservableCalculator = EnergyMean<DosHisto> >
    class JackknifeDosAnalysis
    {
    public:
      //! Typedef for the x-value-type of the DosHistogram (EnergyType)
      typedef DosHist::key_type EnergyType;
      typedef DosHist::mapped_type DosValueType;

      //! Type of the analysis result
      typedef std::pair<typename DosObservableCalculator::return_type, typename DosObservableCalculator::return_type> result_type;

      /*! 
	\brief Do the analyis

	\tparam InputIterator Type of the iterator that is used to iterate through the measurments.
	\param measurement_begin Iterator to the begin of the measurements
	\param measurement_end Iterator to the end of the measurments
	\param bin_size Number of successive measurements to gather in one jackknife bin (default value 1)

	\returns std::pair of Observables, the first entry is the mean of the observables and the second entry is the error of the mean of the observables.
      */
      template <class DosInputIterator, class TemperaturInputIterator>
      static std::vector<result_type> analyse(DosInputIterator measurement_begin, DosInputIterator measurement_end, 
					      TemperatureInputIterator inverse_temperatur_begin, TemperatureInputIterator inverse_temperature_end,
					      unsigned int bin_size = 1, DosObservableCalculator observable_functor = ObservableCalculator())
      {
	// Typedef for a single jackknife accumulator
	typedef ba::accumulator_set<typename DosObservableCalculator::return_type, ba::stats<ba::tag::mean, ba::tag::variance> > JackknifeAccumulatorType;

	// Calculate the size of the samples and the number of bins
	int measurement_size = std::distance(measurement_begin, measurement_end);
	int beta_number = std::distance(inverse_temperature_begin, inverse_temperature_end);
	unsigned int bin_number = measurement_size / bin_size;

	// Define the accumulator accumulating the jackknife bins
	std::vector<JackknifeAccumulatorType> jackknife_accumulator(beta_number);

	// Accumulate the jackknife values
	for (unsigned int jackknife_bin = 0; jackknife_bin < bin_number; ++jackknife_bin)
	{
	  unsigned int jackknife_bin_begin = jackknife_bin * bin_size; // First value not to take into account
	  unsigned int jackknife_bin_end = jackknife_bin_begin + bin_size; // Last value not to take into account

	  // Accumulator for calculating the mean of the density of states
	  ba::accumulator_set<DosType, ba::stats<ba::tag::mean> > dos_mean_accumulator;
	  
	  // Fill the vector
	  unsigned int counter = 0;
	  for (InputIterator it = measurement_begin; it != measurement_end; ++it)	  
	  {
	    if (counter < jackknife_bin_begin || counter >= jackknife_bin_end) dos_mean_accumulator(*it);
	    counter++;
	  }
	  
	  unsigned int beta_counter = 0;
	  for (TemperatureInputIterator beta = inverse_temperature_begin; beta != inverse_temperature_end; ++beta)
	  {
	    // Calculate the observable and push back to the jackknife accumulator
	    jackknife_accumulator[beta_counter](ba::mean(dos_mean_accumulator), *beta);

	    ++beta_counter;
	  }
	}

	// Analyse the accumulator for each beta
	result_type result;
	for (TemperatureInputIterator beta = inverse_temperature_begin; beta != inverse_temperature_end; ++beta)
	  return result.push_back(ba::mean(jackknife_accumulator), sqrt((bin_number - 1)*ba::variance(jackknife_accumulator)));
	return result;
      }
    };
  }
}
  

#endif
