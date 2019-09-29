#!/bin/bash

DATA_PATH="/run/media/daniele/WD_IRB/data/2016_INFN_LNL_Ne_He/"

first_run=$1

if [ -z "$1" ]; then
  echo "Please type at least a valid run number"
  exit 1
fi

if [ -z "$2" ]; then
  last_run=$1
else
  last_run=$2
fi

for run_num in `seq $first_run $last_run`
do
  re='^[0-9]+$'

  if ! [[ $run_num =~ $re ]] ; then
    echo "Error: Invalid run number" >&2; exit 1
  fi
  
  for file_name in ${DATA_PATH}R${run_num}_*
  do
  
    echo "Unpacking file $file_name..."
    eval './exec_Midas2ROOT.exe --run=${file_name}'
  done
  
done