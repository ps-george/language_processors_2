#!/bin/bash
mkdir -p test/ref

for i in test/in/*.txt; do
  echo "Input file : ${i}"
  exec 6< $i
  read -a sedpattern <&6
  echo ${sedpattern[0]} ${sedpattern[1]};
     
  BASENAME=$(basename $i .txt);
  echo "First_dt10_v3.pdf" | ./regex_substitute_ref.sh ${sedpattern[0]} ${sedpattern[1]} > test/ref/$BASENAME.txt

  exec 6<&- 
done
  
