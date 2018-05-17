#!/bin/bash

#SBATCH --nodes=1
#SBATCH --output=./output/CPU_QP.out
#SBATCH --wait-all-nodes=1

srun ./run_QP.sh $SLURM_JOB_NODELIST
