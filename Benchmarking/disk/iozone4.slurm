#!/bin/bash

#SBATCH --nodes=1
#SBATCH --tmp=15G
#SBATCH --output=Iozone4.out
#SBATCH --wait-all-nodes=1

./iozone4.sh $SLURM_JOB_NODELIST
