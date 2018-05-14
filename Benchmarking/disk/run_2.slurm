#!/bin/bash

#SBATCH --nodes=1
#SBATCH --tmp=15G
#SBATCH --ntasks=1
#SBATCH --output=./output/DiskThroughput_10000.out
#SBATCH --wait-all-nodes=1

srun ./run_S_10000.sh $SLURM_JOB_NODELIST

