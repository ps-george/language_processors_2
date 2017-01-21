#!/bin/bash

for i in test/in/*.txt; do
    read -r firstline<$i
    echo $firstline;
    echo "==========================="
    echo ""
    echo "Input file : ${i}"
    BASENAME=$(basename $i .txt);
    cat $i | ./regex_substitute_ref.sh $firstline  > test/ref/$BASENAME.txt
done
