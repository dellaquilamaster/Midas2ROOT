#!/bin/bash

DATA_PATH="/run/media/daniele/WD_IRB/data/2016_INFN_LNL_Ne_He/"
RUN_PREFIX="R"

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

if [ ! -z "$3" ]; then
  parallel_processes=$3
fi

for run_num in `seq $first_run $last_run`
do
  re='^[0-9]+$'

  if ! [[ $run_num =~ $re ]] ; then
    echo "Error: Invalid run number" >&2; exit 1
  fi
  
  for file_name in ${DATA_PATH}${RUN_PREFIX}${run_num}_*
  do
  
    if [ -z "$3" ]; then
  
      if [ ! -f $file_name ]; then
        echo "Skipping run $run_num!"
        continue
      fi
      run_name="${file_name##*/$RUN_PREFIX}"
      echo "Unpacking file $run_name..."
      eval './exec_Midas2ROOT.exe --run=${run_name}'
      
    else 
    
      if [ ! -f $file_name ]; then
        echo "echo Skipping run $run_num!" >> parallel_runs.txt
        continue
      fi
      run_name="${file_name##*/$RUN_PREFIX}"
      echo "echo Unpacking file $run_name... && eval './exec_Midas2ROOT.exe --run=${run_name}'" >> parallel_runs.txt      
    fi
  done
done

if [ ! -z "$3" ]; then
  parallel $parallel_processes < parallel_runs.txt
  rm -f parallel_runs.txt
fi
