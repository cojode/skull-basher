#!/bin/bash

for i in {1..51}; do
    if diff <(./solution.sh "tests/test_${i}_input.txt") "tests/test_${i}_expected.txt"; then
        echo "Test $i: PASS"
    else
        echo "Test $i: FAIL"
    fi
done
