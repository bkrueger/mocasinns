#!/bin/bash

# File for printing the simulation classes important to end users (no Simulation)
# Give option -H for giving the simulation names for replacing in HTML
# Give option -l for giving the simulation names for replacing in Latex

while getopts ":Hlu" opt; do
    case "${opt}" in
        H)
	    echo "ConfigurationTest Metropolis MetropolisHastings MetropolisParallel ParallelTempering SerialTempering EntropicSampling WangLandau OptimalEnsembleSampling MulticanonicalCounting"	    
            ;;
        l)
	    echo "Configuration\\\-Test Metropolis Metropolis\\\-Hastings Metropolis\\\-Parallel Parallel\\\-Tempering Serial\\\-Tempering Entropic\\\-Sampling Wang\\\-Landau OptimalEnsemble\\\-Sampling Multicanonical\\\-Counting"
            ;;
    esac
done