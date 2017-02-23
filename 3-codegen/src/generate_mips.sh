#!/bin/bash

# Run the script using bin/generate_mips
bin/generate_c $1 $2.c

# Compile to file specified as output using gcc and mips
gcc -o $2 $2.c

# rm $2.tmp
