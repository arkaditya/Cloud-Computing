#!/bin/bash

#SBATCH --nodes=1
#SBATCH --tmp=15G
#SBATCH --output=Iozone3.out
#SBATCH --wait-all-nodes=1

./iozone3.sh $SLURM_JOB_NODELIST
