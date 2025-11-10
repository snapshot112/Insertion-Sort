#!/bin/bash

for test_input in `ls *txt`; do
    ref_out=$(basename $test_input ".txt").out;
    cat $test_input | sed 's/\s/\n/g' | sed '/^$/d' | sort -n  > $ref_out;
done
