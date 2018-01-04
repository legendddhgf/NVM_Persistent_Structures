#!/bin/bash
mv *.csv ../results
for i in $(ls ../results/* | grep .csv); do
  ../results/pilot_analyze.sh $i > ../results/${i/.csv/Analysis.txt}
done
