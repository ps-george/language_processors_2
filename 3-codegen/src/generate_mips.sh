#!/bin/bash

# Run the script using bin/generate_mips
bin/generate_c $1 $2.c

# Compile to file specified as output using gcc and mips
mips-linux-gnu-g++ -o $2 $2.c -static

# Remove temporary .c file
rm $2.c
