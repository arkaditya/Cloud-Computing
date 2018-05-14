#!/bin/bash

#SBATCH --nodes=1
#SBATCH --output=./output/CPU_DP.out
#SBATCH --wait-all-nodes=1

srun ./run_DP.sh $SLURM_JOB_NODELIST
