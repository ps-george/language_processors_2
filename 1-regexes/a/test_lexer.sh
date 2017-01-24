#!/bin/bash

echo "========================================"
echo " Cleaning the temporaries and outputs"
make clean
echo " Force building histogram"
make histogram -B
if [[ "$?" -ne 0 ]]; then
    echo "Build failed.";
fi
echo ""
mkdir -p test/out

echo "========================================="

PASSED=0
CHECKED=0

for i in test/in/*.txt; do
    echo "==========================="
    echo ""
    echo "Input file : ${i}"
    BASENAME=$(basename $i .txt);
    cat $i | ./histogram  > test/out/$BASENAME.stdout.txt  2> test/out/$BASENAME.stderr.txt

    diff <(cat test/ref/$BASENAME.stdout.txt) <(cat test/out/$BASENAME.stdout.txt) > test/out/$BASENAME.diff.txt
    if [[ "$?" -ne "0" ]]; then
        echo -e "\nERROR"
    else
        PASSED=$(( ${PASSED}+1 ));
    fi
    CHECKED=$(( ${CHECKED}+1 ));
done


echo "########################################"
echo "Passed ${PASSED} out of ${CHECKED}".
echo ""

if [ "${PASSED}" == "${CHECKED}" ]; then
  exit 0
else
  exit 1
fi
