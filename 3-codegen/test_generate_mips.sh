#!/bin/bash

mkdir -p working

echo "========================================"
echo " Force building gen mips"
make -B bin/generate_mips
if [[ "$?" -ne "0" ]]; then
    echo "Error while building compiler."
    exit 1;
fi
echo "========================================="


for i in test/programs/*; do
    b=$(basename ${i});
    mkdir -p working/$b

    PARAMS=$(head -n 1 $i/in.params.txt | ${DOS2UNIX} );

    echo "==========================="
    echo ""
    echo "Input file : ${i}"
    echo "Testing $b"

    bin/generate_mips $i/in.code.txt working/$b/$b \
        2> working/$b/c_code.stderr.txt
done
