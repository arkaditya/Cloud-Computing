#!/bin/bash

#SBATCH --nodes=1
#SBATCH --exclude=hadoop-c,hadoop-i,hadoop-b,hadoop-e,hadoop-g,hadoop-d,hadoop-h
#SBATCH --output=new20GB.log
#SBATCH --wait-all-nodes=1

srun ./run_20GB.sh $SLURM_JOB_NODELIST
