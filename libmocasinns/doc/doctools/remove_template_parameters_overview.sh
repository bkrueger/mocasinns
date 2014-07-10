#!/bin/bash

# This script removes the template parameters in the given HTML file of the documentation for the algorithm classes
# As second argument give the target to work on
# Give option -H for using on HTML file
# Give option -l for using on Latex file

while getopts "Hl" opt; do
    case "${opt}" in
        H)
	    CLASSES=$(doctools/algorithm_classes.sh -H)
	    MODE="HTML"
            ;;
        l)
	    CLASSES=$(doctools/algorithm_classes.sh -l)
	    MODE="latex"
            ;;
    esac
done

TARGET=$2

for CLASS in $CLASSES
do
    if [ "$MODE" == "HTML" ];
    then
	sed -i "s/${CLASS}\&lt;[^\&]*\&gt;/${CLASS}\&lt;...\&gt;/g" $TARGET
    fi

    if [ "$MODE" == "latex" ];
    then
	sed -i "s/${CLASS}\\$<\\$.*\\$>\\$/${CLASS}\\$< \\\dots >\\$/g" $TARGET
    fi
done