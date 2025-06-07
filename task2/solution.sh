#!/bin/bash

cat "$@" | awk '{
    gsub(/^(a|an)[[:space:]]/i, "");
    gsub(/^(a|an)$/i, "");

    if (NR % 2 == 1) $0 = toupper($0);
    else $0 = tolower($0);

    if (!/^$/ && !/[.?!:;]$/) $0 = $0 ".";

    if (/^$/) $0 = ".";

    print;
}'