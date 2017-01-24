#!/bin/bash

# Compare output of programs directly
echo "========================================"
echo "Cleaning the temporaries and outputs"
make clean
echo "Force building regex substitute"
make regex_substitute -B
if [[ "$?" -ne 0 ]]; then
    echo "Build failed.";
fi

echo "========================================="

chmod u+x regex_substitute
PASSED=0
CHECKED=0

for i in test/*.txt; do
  echo "Input file : ${i}"
  exec 6< $i
  read -r sedpattern <&6
     
  BASENAME=$(basename $i .txt);
  tail --lines=+2 $i | eval ./regex_substitute_ref.sh ${sedpattern}
  diff <(tail --lines=+2 $i | eval ./regex_substitute_ref.sh ${sedpattern}) <(tail --lines=+2 $i | eval ./regex_substitute ${sedpattern})
  if [ $? -ne 0 ]; then
    echo -e "\nERROR"
  else
    PASSED=$(( ${PASSED}+1 ));
  fi
    CHECKED=$(( ${CHECKED}+1 ));
  exec 6<&-
done

echo "Passed ${PASSED} out of ${CHECKED} tests."

if [ "${PASSED}" == "${CHECKED}" ]; then
  exit 0
else
  exit 1
fi
