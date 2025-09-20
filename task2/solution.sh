#!/bin/sh

cat "$@" | awk '
BEGIN {ORS=""}
{
    gsub(/^(a|an|A|An|aN|AN)[[:space:]]/, "");
    gsub(/^(a|an|A|An|aN|AN)$/, "");

    if (NR % 2 == 1) $0 = toupper($0);
    else $0 = tolower($0);

    if (!/^$/ && !/[.?!:;]$/) $0 = $0 ".";

    if (/^$/) $0 = ".";

    if (NR > 1) print "\n"

    print
}'