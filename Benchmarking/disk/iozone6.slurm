#!/bin/bash

#SBATCH --nodes=1
#SBATCH --tmp=15G
#SBATCH --output=Iozone6.out
#SBATCH --wait-all-nodes=1

./iozone6.sh $SLURM_JOB_NODELIST
