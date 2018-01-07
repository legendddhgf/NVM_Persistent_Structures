#!/bin/bash
for i in $(ls | grep .csv); do
  sudo ./pilot_analyze.sh $i > ${i/.csv/Analysis.txt}
done
for f in *.csv *.txt; do
  sudo mv $f ../results
done
