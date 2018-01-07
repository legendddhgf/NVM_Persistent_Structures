#!/bin/bash
if [ -e "*.csv" ]; then
  sudo mv *.csv ../results
fi
for i in $(ls ../results/* | grep .csv); do
  sudo ../results/pilot_analyze.sh $i > ../results/${i/.csv/Analysis.txt}
done
