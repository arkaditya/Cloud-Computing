#!/bin/bash

#SBATCH --nodes=1
#SBATCH --tmp=15G
#SBATCH --output=./output/DiskThroughput.out
#SBATCH --wait-all-nodes=1

./run_RS_10000.sh $SLURM_JOB_NODELIST
