#!/bin/bash

PASSED=0
CHECKED=0

# Run test script for part a
echo "########################################"
echo "Testing part a..."
cd a
bash ./test_lexer.sh 2>&1 >/dev/null
if [[ "$?" -ne "0" ]]; then
    echo -e "\nFailed lexer test"
else
    PASSED=$(( ${PASSED}+1 ));
    echo "Passed."
fi
CHECKED=$(( ${CHECKED}+1 ));
# Clean a
make clean 2>&1 >/dev/null

# Run test script for part b
cd ../b
echo "########################################"
echo "Testing part b..."
bash ./test_b.sh
if [[ "$?" -ne "0" ]]; then
    echo -e "\nFailed regex substitute test"
else
    PASSED=$(( ${PASSED}+1 ));
fi
CHECKED=$(( ${CHECKED}+1 ));
# Clean b
make clean 2>&1 >/dev/null

# Run test script for part c
cd ../c
echo "########################################"
echo "Testing part c..."
bash ./test_c.sh
if [[ "$?" -ne "0" ]]; then
    echo -e "\nFailed regex engine test"
else
    PASSED=$(( ${PASSED}+1 ));
fi
CHECKED=$(( ${CHECKED}+1 ));


# Clean c
make clean 2>&1 >/dev/null

echo "########################################"
echo "Passed ${PASSED} out of ${CHECKED} test suites."
