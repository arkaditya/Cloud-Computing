#!/bin/bash

#SBATCH --nodes=1
#SBATCH --output=./output/CPU_HP.out
#SBATCH --wait-all-nodes=1

srun ./run_HP.sh $SLURM_JOB_NODELIST
