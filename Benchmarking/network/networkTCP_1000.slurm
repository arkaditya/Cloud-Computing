#!/bin/bash

#SBATCH --nodes=2
#SBATCH --ntasks=2
#SBATCH --output=./output/networkTCP_1000.dat
#SBATCH --wait-all-nodes=1

srun ./TCP_1000.sh $SLURM_JOB_NODELIST
