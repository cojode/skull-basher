#!/bin/bash

cat "$@" | awk '{
    gsub(/^(a|an|A|An|aN|AN)[[:space:]]/, "");
    gsub(/^(a|an|A|An|aN|AN)$/, "");

    if (NR % 2 == 1) $0 = toupper($0);
    else $0 = tolower($0);

    if (!/^$/ && !/[.?!:;]$/) $0 = $0 ".";

    if (/^$/) $0 = ".";

    print;
}'
