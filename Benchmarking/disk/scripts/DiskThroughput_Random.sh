#!/bin/bash

for FILE in run_4.slurm run_5.slurm  run_6.slurm
do
	sbatch $FILE
done
