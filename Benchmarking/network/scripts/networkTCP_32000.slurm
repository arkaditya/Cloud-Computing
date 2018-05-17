#!/bin/bash

#SBATCH --nodes=2
#SBATCH --ntasks=2
#SBATCH --output=./output/networkTCP_32000.dat
#SBATCH --wait-all-nodes=1

srun ./TCP_32000.sh $SLURM_JOB_NODELIST
