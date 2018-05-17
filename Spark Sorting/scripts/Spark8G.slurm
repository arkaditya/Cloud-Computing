#!/bin/bash

#SBATCH --nodes=1
#SBATCH --exclude=hadoop-c,hadoop-i,hadoop-b,hadoop-a,hadoop-g,hadoop-d,hadoop-h
#SBATCH --output=spark8GB.log
#SBATCH --wait-all-nodes=1

srun ./runSpark_8GB.sh $SLURM_JOB_NODELIST
