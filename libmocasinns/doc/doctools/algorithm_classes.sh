#!/bin/bash

# File for printing all simulation classes
# Give option -H for giving the simulation names for replacing in HTML
# Give option -l for giving the simulation names for replacing in Latex

while getopts ":Hl" opt; do
    case "${opt}" in
        H)
	    echo "Simulation Metropolis MetropolisHastings MetropolisParallel ParallelTempering SerialTempering EntropicSampling WangLandau OptimalEnsembleSampling MulticanonicalCounting"	    
            ;;
        l)
	    echo "Simulation Metropolis Metropolis\\\-Hastings Metropolis\\\-Parallel Parallel\\\-Tempering Serial\\\-Tempering Entropic\\\-Sampling Wang\\\-Landau OptimalEnsemble\\\-Sampling Multicanonical\\\-Counting"
            ;;
    esac
done