#!/bin/bash

#SBATCH --nodes=1
#SBATCH --tmp=15G
#SBATCH --output=Iozone2.out
#SBATCH --wait-all-nodes=1

./iozone2.sh $SLURM_JOB_NODELIST
