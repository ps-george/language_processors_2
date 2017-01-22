#!/bin/bash
mkdir -p test/ref

for i in test/in/*.txt; do
  echo "Input file : ${i}"
  exec 6< $i
  read -r sedpattern <&6
     
  BASENAME=$(basename $i .txt);
  tail --lines=+2 $i | eval ./regex_substitute_ref.sh ${sedpattern}

  exec 6<&- 
done
  
