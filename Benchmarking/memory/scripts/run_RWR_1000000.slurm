#!/bin/bash

#SBATCH --nodes=1
#SBATCH --output=output/memory_RWR1000000.out
#SBATCH --wait-all-nodes=1

srun ./RWR_1000000.sh $SLURM_JOB_NODELIST
