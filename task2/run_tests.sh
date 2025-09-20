#!/bin/sh

for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51; do
    ./solution.sh "tests/test_${i}_input.txt" > "tests/test_${i}_actual.txt"
    if diff "tests/test_${i}_actual.txt" "tests/test_${i}_expected.txt" > /dev/null 2>&1; then
        echo "Test $i: PASS"
    else
        echo "Test $i: FAIL"
    fi
    rm -f "tests/test_${i}_actual.txt"
done


for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51; do
    cat "tests/test_${i}_input.txt" | ./solution.sh > "tests/test_${i}_actual.txt"
    if diff "tests/test_${i}_actual.txt" "tests/test_${i}_expected.txt" > /dev/null 2>&1; then
        echo "Test $i: PASS"
    else
        echo "Test $i: FAIL"
    fi
    rm -f "tests/test_${i}_actual.txt"
done