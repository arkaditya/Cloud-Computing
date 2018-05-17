#!/bin/bash

#SBATCH --nodes=2
#SBATCH --ntasks=2
#SBATCH --output=./output/networkTCP_Latency.dat
#SBATCH --wait-all-nodes=1

srun ./TCP_latency.sh $SLURM_JOB_NODELIST
