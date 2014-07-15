#ifndef MOCASINNS_ANALYSIS_MULTICANONICAL_AVERAGE_HPP
#define MOCASINNS_ANALYSIS_MULTICANONICAL_AVERAGE_HPP

namespace ba = boost::accumulators;

#include <iterator>

// Boost accumulator
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>

#include "../histograms/histocrete.hpp"

namespace Mocasinns
{
  namespace Analysis
  {
    //! Helper class for the analysis of multicanonical simulation results. Calculates the microcanonical averages for each energy.
    class MulticanonicalAverage
    {
    public:
      //! Function to calculate a Histocrete of the microcanonical averages
      /*!
       * \tparam PairObservableIterator Iterator to a range of PairObservables. The first type of the PairObservables must be the energy of the configurations, the second the observable to consider.
       * \param pair_observables_begin Begin of the range of PairObservables.
       * \param pair_observables_end End of the range of PairObservables
       * \returns Histocrete with the microcanonical averages (values) for each energy (key)
       */
      template <class PairObservableIterator>
      static Histograms::Histocrete<typename std::iterator_traits<PairObservableIterator>::value_type::first_type,
				    typename std::iterator_traits<PairObservableIterator>::value_type::second_type> average(PairObservableIterator pair_observables_begin,
															    PairObservableIterator pair_observables_end)
      {
	typedef typename std::iterator_traits<PairObservableIterator>::value_type::first_type EnergyType;
	typedef typename std::iterator_traits<PairObservableIterator>::value_type::second_type ObservableType;
	typedef typename Mocasinns::Observables::PairObservable<EnergyType, ObservableType> PairType;
	typedef ba::accumulator_set<ObservableType, ba::stats<ba::tag::mean> > AccumulatorType;

	// Accumulate the single energies
	std::map<EnergyType, AccumulatorType> accumulator_map;
	for (PairObservableIterator it = pair_observables_begin; it != pair_observables_end; ++it)
	{
	  if (accumulator_map.find(it->first) == accumulator_map.end())
	    accumulator_map[it->first] = AccumulatorType();

	  accumulator_map[it->first](it->second);
	}

	// Create the Histocrete with the means
	Histograms::Histocrete<EnergyType, ObservableType> result;
	for (typename std::map<EnergyType, AccumulatorType>::const_iterator it = accumulator_map.begin(); it != accumulator_map.end(); ++it)
	  result[it->first] = ba::mean(it->second);

	return result;
      }
    };
  }
}

#endif
