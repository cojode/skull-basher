#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Usage: ${0##*/} file"
    exit 1
fi

filename=$1

if [ ! -f "$filename" ]; then
    echo "Error: file '$filename' does not exist"
    exit 1
fi

while IFS= read -r line; do
    if [[ $line =~ ^[0-9] ]]; then
        read -ra words <<< "$line"
        
        for (( i=1; i<${#words[@]}; i++ )); do
            echo "${words[$i]}"
        done
    fi
done < "$filename"