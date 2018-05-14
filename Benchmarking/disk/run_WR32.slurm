#!/bin/bash

#SBATCH --nodes=1
#SBATCH --tmp=15G
#SBATCH --ntasks=1
#SBATCH --output=./output/DiskLatency_32.out
#SBATCH --wait-all-nodes=1

srun ./run_WR32.sh $SLURM_JOB_NODELIST

