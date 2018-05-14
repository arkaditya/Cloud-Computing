#!/bin/bash

#SBATCH --nodes=1
#SBATCH --exclude=hadoop-c,hadoop-i,hadoop-b,hadoop-a,hadoop-d,hadoop-h,hadoop-g
#SBATCH --output=spark20GB.log
#SBATCH --wait-all-nodes=1

srun ./runSpark_20GB.sh $SLURM_JOB_NODELIST
