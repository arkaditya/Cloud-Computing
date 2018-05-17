#!/bin/bash

#SBATCH --nodes=1
#SBATCH --output=output/memoryRWR_1.out
#SBATCH --wait-all-nodes=1

srun ./RWR_1.sh $SLURM_JOB_NODELIST
