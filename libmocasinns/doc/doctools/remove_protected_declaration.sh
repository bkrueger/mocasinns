#!/bin/bash
# This file removes the declaration of protected members (attributes and functions) from a html file

TARGET=$1

## Protected methods
#sed -i ':a;N;$!ba;s@<tr><td colspan="2"><h2><a name="pro-methods"></a>.*<tr><td colspan="2">@<tr><td colspan="2">@g' $TARGET
#sed -i ':a;N;$!ba;s@<tr><td colspan="2"><h2><a name="pro-methods"></a>.*</table>@</table>@g' $TARGET
#
## Static protected methods
#sed -i ':a;N;$!ba;s@<tr><td colspan="2"><h2><a name="pro-static-methods"></a>.*<tr><td colspan="2">@<tr><td colspan="2">@g' $TARGET
#sed -i ':a;N;$!ba;s@<tr><td colspan="2"><h2><a name="pro-static-methods"></a>.*</table>@</table>@g' $TARGET
#
## Protected attributes
#sed -i ':a;N;$!ba;s@<tr><td colspan="2"><h2><a name="pro-attribs"></a>.*<tr><td colspan="2">@<tr><td colspan="2">@g' $TARGET
#sed -i ':a;N;$!ba;s@<tr><td colspan="2"><h2><a name="pro-attribs"></a>.*</table>@</table>@g' $TARGET
#
## Static attributes
#sed -i ':a;N;$!ba;s@<tr><td colspan="2"><h2><a name="pro-static-attribs"></a>.*<tr><td colspan="2">@<tr><td colspan="2">@g' $TARGET
#sed -i ':a;N;$!ba;s@<tr><td colspan="2"><h2><a name="pro-static-attribs"></a>.*</table>@</table>@g' $TARGET

perl -i -pe 'BEGIN{undef $/;} s@<tr><td colspan="2"><h2><a name="pro-methods"></a>.*?<tr><td colspan="2">@<tr><td colspan="2">@smg' $TARGET
perl -i -pe 'BEGIN{undef $/;} s@<tr><td colspan="2"><h2><a name="pro-static-methods"></a>.*?<tr><td colspan="2">@<tr><td colspan="2">@smg' $TARGET
perl -i -pe 'BEGIN{undef $/;} s@<tr><td colspan="2"><h2><a name="pro-attribs"></a>.*?<tr><td colspan="2">@<tr><td colspan="2">@smg' $TARGET
perl -i -pe 'BEGIN{undef $/;} s@<tr><td colspan="2"><h2><a name="pro-static-attribs"></a>.*?<tr><td colspan="2">@<tr><td colspan="2">@smg' $TARGET