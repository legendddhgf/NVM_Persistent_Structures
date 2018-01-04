#!/bin/bash

#this was created because pilot's data analysis system crashes if you send a file directly but piping it in works just fine

if [[ ! -e "$1" ]]; then
  echo "file $1 doesn't exist"
  exit 1
fi

declare -a result_type

result_type=(
  'Insert() Time [ms]'
  'Get() Time [ms]'
  'Memory Usage [kB]'
)
echo "Results for $1:"
echo
let result_len_one_less=${#result_type[@]}-1
for i in $(seq 0 $result_len_one_less)
do
  echo -n "Analyzing "
  echo ${result_type[$i]}
  let i_plus_one=$i+1
  cut -d ',' $1 -f $i_plus_one | pilot analyze -i 1 -
  echo
done
