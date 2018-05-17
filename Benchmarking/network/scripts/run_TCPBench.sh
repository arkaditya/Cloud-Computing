#!/bin/bash

for FILE in networkTCP_1000.slurm networkTCP_32000.slurm networkTCP_1.slurm 
do
	sbatch $FILE
done
