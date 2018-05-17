#!/bin/bash

for FILE in networkUDP_1000.slurm networkUDP_32000.slurm networkUDP_1.slurm 
do
	sbatch $FILE
done
