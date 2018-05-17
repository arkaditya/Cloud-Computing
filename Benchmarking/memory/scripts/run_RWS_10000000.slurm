#!/bin/bash

#SBATCH --nodes=1
#SBATCH --output=output/memoryRWS_10000000.out
#SBATCH --wait-all-nodes=1

srun ./RWS_10000000.sh $SLURM_JOB_NODELIST
