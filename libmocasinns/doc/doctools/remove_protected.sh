#!/bin/bash
# This file removes all protected members (attributes and functions) from a html file

TARGET=$1

doctools/remove_protected_declaration.sh $TARGET
python doctools/remove_protected_documentation.py $TARGET > temp.html
mv temp.html $TARGET