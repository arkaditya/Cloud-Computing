#!/bin/bash

for FILE in  run_DP.slurm run_SP.slurm run_HP.slurm run_QP.slurm
do
	sbatch $FILE
done
