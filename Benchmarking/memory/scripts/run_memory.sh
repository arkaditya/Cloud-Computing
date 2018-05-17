#!/bin/bash

for FILE in run_RWS_1000.slurm run_RWR_1000.slurm run_RWS_1000000.slurm run_RWR_1000000.slurm  run_RWS_10000000.slurm run_RWR_10000000.slurm run_RWS_1.slurm run_RWR_1.slurm
do
	sbatch $FILE
done
