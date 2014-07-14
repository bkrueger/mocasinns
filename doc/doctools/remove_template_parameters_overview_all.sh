#!/bin/bash

# This script removes the template parameters in the given HTML file of the documentation for all classes
# As second argument give the target to work on
# Give option -H for using on HTML file
# Give option -l for using on Latex file

while getopts "Hl" opt; do
    case "${opt}" in
        H)
	    MODE="HTML"
            ;;
        l)
	    MODE="latex"
            ;;
    esac
done

TARGET=$2

if [ "$MODE" == "HTML" ];
then
    sed -i "s/\&lt;[^\&]*\&gt;/\&lt;...\&gt;/g" $TARGET
fi

if [ "$MODE" == "latex" ];
then
    sed -i "s/\\$<\\$.*\\$>\\$/\\$< \\\dots >\\$/g" $TARGET
fi
