#!/bin/bash

# This file extracts from each foo.cpp all doxygen blocks that start with "/*! \file" or "/*! \fn" and export the comments to a seprate file foo.dox.
# After this, doxygen is invoked with the normal Doxyfile
#
# This procedure is needed since we want to have the doxygen comments in front of every function in the implementation .cpp files, but doxygen does not recognise these blocks if they are mixed into the source file.

# Extract the documentation from the source files
find ../include/mocasinns/src/ -name "*.cpp" | while read FILENAME_CPP
do
    echo "Creating documentation for source file $FILENAME_CPP"

    FILENAME_DOX=$(echo $FILENAME_CPP | sed "s@\(.*\).cpp@\1.dox@g")
    
    python doctools/extract_functions_auto.py $FILENAME_CPP > $FILENAME_DOX
done

# Call Doxygen
doxygen Doxyfile

# Remove the additional included doxumentation files
find ../include/mocasinns/src/ -name "*.dox" | while read FILENAME_DOX
do
    echo "Removing documentation for source file $FILENAME_DOX"
    rm $FILENAME_DOX
done

# Remove template parameters from the html documentation overview that blow it up unneccessary
echo "Removing unnecessary template parameters from file html/annotated.html"
doctools/remove_template_parameters_overview.sh -H html/annotated.html
echo "Removing unnecessary template parameters from file html/hierarchy.html"
doctools/remove_template_parameters_overview.sh -H html/hierarchy.html
find ./html -name "functions*.html" | while read FUNCTIONS_HTML
do
    echo "Removing unnecessary template parameters from file $FUNCTIONS_HTML"
    doctools/remove_template_parameters_overview.sh -H $FUNCTIONS_HTML
done

# Remove template parameters from the tex overview files that blow it up unneccessary
echo "Removing unnecessary template parameters from file latex/annotated.tex"
doctools/remove_template_parameters_overview.sh -l latex/annotated.tex
echo "Removing unnecessary template parameters from file latex/hierarchy.tex"
doctools/remove_template_parameters_overview.sh -l latex/hierarchy.tex

# Remove template parameters from detailed member descriptions
for CLASS in $(doctools/algorithm_classes.sh -H)
do
    for CLASS_FULL in $CLASS ${CLASS}Base ${CLASS}RejectionFree
    do
	FILENAME="html/classMocasinns_1_1${CLASS_FULL}.html"
	if [ -f $FILENAME ];
	then
	    echo "Removing unnecessary template parameters in member documentation from file $FILENAME"
	    doctools/remove_template_parameters_class_member_doc.sh -H $FILENAME
	fi
    done
done

# Remove template parameters from detailed member descriptions
for CLASS in $(doctools/algorithm_classes.sh -l)
do
    for CLASS_FULL in "$CLASS" "${CLASS}\\\-Base" "${CLASS}\\\-Rejection\\\-Free"
    do
	CLASS_FILENAME=$(echo ${CLASS_FULL} | sed 's@\\\\-@@g')
	FILENAME="latex/classMocasinns_1_1${CLASS_FILENAME}.tex"
	if [ -f $FILENAME ];
	then
	    echo "Removing unnecessary template parameters in member documentation from file $FILENAME"
	    doctools/remove_template_parameters_class_member_doc.sh -l $FILENAME
	fi
    done
done

