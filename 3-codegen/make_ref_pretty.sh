echo "========================================"
echo " Cleaning the temporaries and outputs"
make clean

echo "========================================"
echo " Force building pretty_print"
make -B bin/pretty_print
if [[ "$?" -ne "0" ]]; then
    echo "Error while building pretty_print."
    exit 1;
fi
echo "========================================="


for i in test/optimise/*; do
  
  b=$(basename ${i});
  
  echo "==========================="
  echo ""
  echo "Input file : ${i}"
  read -p "Enter to continue..."
  
  cat ${i}/ref.output.txt | bin/pretty_print \
    > ${i}/ref.pretty.txt
done 
