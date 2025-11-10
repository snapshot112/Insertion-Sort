#! /usr/bin/env bash

# Test PROG with all txt files in tests/
# Compares the output with reference output files *.out

PROG=./mysort
prog_output=$(mktemp)
tests=`find tests/*.txt`

trap "rm -f $prog_output" EXIT

for t in $tests
do
    echo -n "testing: $t: "
    $PROG < $t > $prog_output
    ret="$?"
    if [[ "ret" -ne 0 ]];
    then
        echo "FAILED"
        echo "$PROG returned exit code $ret"
        continue
    fi
    if diff -q $prog_output ${t%.*}.out > /dev/null;
    then
        echo "OK"
    else
        if diff -qwB $prog_output ${t%.*}.out > /dev/null;
        then
            echo "OK (apart from whitespace)"
        else
            echo "FAILED"
        fi
    fi
done

rm -f $prog_output
