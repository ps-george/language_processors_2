#!/bin/bash

# Run test script for part a
cd a
bash ./test_lexer.sh
# Clean a
make clean

# Run test script for part b
cd ../b
bash ./test_b.sh
# Clean b
make clean

# Run test script for part c
cd ../c
bash ./test_c.sh
# Clean c
make clean
