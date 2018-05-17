#!/bin/bash

for FILE in run_1.slurm run_2.slurm  run_3.slurm
do
	sbatch $FILE
done
