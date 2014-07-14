#!/bin/bash

# This script removes the template parameters in the given HTML file of the documentation
# As argument give the target to work on

TARGET=$1

# Remove some ugly linebreaks
sed -i ':a;N;$!ba;s@<br class=\"typebreak\"/>\n@<br class=\"typebreak\">@g' $TARGET
# Remove the unnecessary template parameters
sed -i "s@<td class=\"memItemLeft\" align=\"right\" valign=\"top\">\(.*\)</a><br class=\"typebreak\">\&lt;.*\&gt;@<td class=\"memItemLeft\" align=\"right\" valign=\"top\">\1</a>\&lt;...\&gt;@g" $TARGET
sed -i "s@<td class=\"memname\">\(.*\)</a>\&lt;.*\&gt;@<td class=\"memname\">\1</a>\&lt;...\&gt;@g" $TARGET