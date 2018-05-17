#!/bin/bash

#SBATCH --nodes=1
#SBATCH --exclude=hadoop-c,hadoop-i,hadoop-b,hadoop-g,hadoop-d,hadoop-h,hadoop-e
#SBATCH --output=80GB.log
#SBATCH --wait-all-nodes=1

srun ./run_80GB.sh $SLURM_JOB_NODELIST
