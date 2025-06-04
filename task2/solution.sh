#!/bin/bash

cat $* | sed -E '
    s/^(a|an)[[:space:]]//I;
    s/^(a|an)$//I;
    1~2 s/.*/\U&/;
    2~2 s/.*/\L&/;
    s/([^.?!:;])$/\1./;
    s/^$/./;
'