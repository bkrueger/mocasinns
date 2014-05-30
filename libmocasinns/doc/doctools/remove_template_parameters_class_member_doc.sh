#!/bin/bash

# This script removes the template parameters in the given HTML file of the documentation
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
	for CLASS_FULL in $CLASS ${CLASS}Base ${CLASS}RejectionFree
	do
	    # Remove some ugly linebreaks
	    sed -i ':a;N;$!ba;s@<br class=\"typebreak\"/>\n@<br class=\"typebreak\">@g' $TARGET
	    # Remove the unnecessary template parameters
	    sed -i "s@<td class=\"memItemLeft\" align=\"right\" valign=\"top\">\(.*\)${CLASS_FULL}</a><br class=\"typebreak\">\&lt;.*\&gt;@<td class=\"memItemLeft\" align=\"right\" valign=\"top\">\1${CLASS_FULL}</a>\&lt;...\&gt;@g" $TARGET
	    sed -i "s@<td class=\"memname\">\(.*\)${CLASS_FULL}</a>\&lt;.*\&gt;@<td class=\"memname\">\1${CLASS_FULL}</a>\&lt;...\&gt;@g" $TARGET
	done
    fi
    
    if [ "$MODE" == "latex" ];
    then
	for CLASS_FULL in "$CLASS" "${CLASS}\\\-Base" "${CLASS}\\\-Rejection\\\-Free"
	do
	    sed -i "s/\\\bf \\\-Mocasinns\\\-::\\\-${CLASS_FULL}\\$<\\$.*\\$>\\$/$\\\bf \\\-Mocasinns\\\-::\\\-{CLASS_FULL}\\$< \\\dots >\\$/g" $TARGET
	done
    fi
done