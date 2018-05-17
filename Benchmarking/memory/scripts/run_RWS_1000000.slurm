#!/bin/bash

#SBATCH --nodes=1
#SBATCH --output=output/memoryRWS_1000000.out
#SBATCH --wait-all-nodes=1

srun ./RWS_1000000.sh $SLURM_JOB_NODELIST
