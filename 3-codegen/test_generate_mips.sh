#!/bin/bash

mkdir -p working

# Try to find a portable way of getting rid of
# any stray carriage returns
if which dos2unix ; then
    DOS2UNIX="dos2unix"
elif which fromdos ; then
    DOS2UNIX="fromdos"
else
    # This works on a GNU version of sed. I think this
    # will work in OSX as well, but don't have a machine
    # on which to test that. From reading the OSX docs,
    # it looks compatible.
    # The code \x0D is the ASCII code of carriage-return,
    # so it the regex should delete any CRs at the end of
    # a line (or anywhere in a line)
    DOS2UNIX="sed -e s/\x0D//g -"
    # Tested for combinations of:
    # - Ubuntu
    # - Cygwin
    # and inputs formats:
    # - LF
    # - CRLF
fi

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

    bin/generate_mips $i/in.code.txt working/$b/c_code.txt \
        2> working/$b/c_code.stderr.txt
done
