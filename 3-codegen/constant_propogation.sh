#!/bin/bash

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
echo " Cleaning the temporaries and outputs"
make clean

echo "========================================"
echo " Force building constant_propogation"
make -B bin/constant_propogation
if [[ "$?" -ne "0" ]]; then
    echo "Error while building constant_propogation."
    exit 1;
fi
echo "========================================="

mkdir -p working

for i in test/optimise/*; do
  
  b=$(basename ${i});
  mkdir -p working/$b;
  
  echo "==========================="
  echo ""
  echo "Input file : ${i}"
  echo "Working dir : working/${b}"
  
  cp $i/in.code.txt working/$b/in.code.txt
  read -p "Enter to continue..."
  
  CHANGED=1;
  while [[ "$CHANGED" -ne 0 ]]; do
    CHANGED=0;
    
    cat working/$b/in.code.txt | bin/constant_propogation \
      > working/$b/got.output.txt
        
      if [[ "$?" -eq "0" ]]; then
        CHANGED=1;
      fi
      cp working/$b/got.output.txt working/$b/in.code.txt
  done 
done 

  # cat $SRCFILE | bin/dead_branch_removal > $SRCFILE.tmp
  # if [[ "$?" -eq "0" ]]; then
  #   CHANGED=1;
  # fi
  # cp $SRCFILE.tmp $SRCFILE
  # 
  # cat $SRCFILE | bin/constant_propagation > $SRCFILE.tmp
  # if [[ "$?" -eq "0" ]]; then
  #   CHANGED=1;
  # fi
  # cp $SRCFILE.tmp $SRCFILE
