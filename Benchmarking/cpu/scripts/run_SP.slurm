#!/bin/bash

#SBATCH --nodes=1
#SBATCH --output=./output/CPU_SP.out
#SBATCH --wait-all-nodes=1

srun ./run_SP.sh $SLURM_JOB_NODELIST
