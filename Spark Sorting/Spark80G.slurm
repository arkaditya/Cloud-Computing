#!/bin/bash

#SBATCH --nodes=1
#SBATCH --exclude=hadoop-c,hadoop-i,hadoop-b,hadoop-a,hadoop-g,hadoop-d,hadoop-h
#SBATCH --output=spark80GB.log
#SBATCH --wait-all-nodes=1

srun ./runSpark_80GB.sh $SLURM_JOB_NODELIST
