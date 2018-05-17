#!/bin/bash

#SBATCH --nodes=1
#SBATCH --exclude=hadoop-c,hadoop-i,hadoop-b,hadoop-e,hadoop-g,hadoop-d
#SBATCH --output=new8GB.log
#SBATCH --wait-all-nodes=1

srun ./run_8GB.sh $SLURM_JOB_NODELIST
