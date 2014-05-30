#!/bin/bash

# File for printing all simulation classes
# Give option -H for giving the simulation names for replacing in HTML
# Give option -l for giving the simulation names for replacing in Latex

while getopts ":Hlu" opt; do
    case "${opt}" in
        H)
	    echo "Simulation $(doctools/algorithm_classes_user.sh -H)"
            ;;
        l)
	    echo "Simulation $(doctools/algorithm_classes_user.sh -l)"
            ;;
    esac
done