#!/bin/bash

#SBATCH --nodes=1
#SBATCH --mem=3000
#SBATCH --output=memory.out
#SBATCH --wait-all-nodes=1

echo $SLURM_JOB_NODELIST
echo "Pattern	Block		Threads	 	 Throughput(GB/sec)	Latency(MicroSeconds)"
./MyRAMBench memory-RWS-1000-1thread.dat
./MyRAMBench memory-RWS-1000-2thread.dat
./MyRAMBench memory-RWS-1000-4thread.dat
./MyRAMBench memory-RWR-1000-1thread.dat
./MyRAMBench memory-RWR-1000-2thread.dat
./MyRAMBench memory-RWR-1000-4thread.dat
./MyRAMBench memory-RWS-1000000-1thread.dat
./MyRAMBench memory-RWS-1000000-2thread.dat
./MyRAMBench memory-RWS-1000000-4thread.dat
./MyRAMBench memory-RWR-1000000-1thread.dat
./MyRAMBench memory-RWR-1000000-2thread.dat
./MyRAMBench memory-RWR-1000000-4thread.dat
./MyRAMBench memory-RWS-10000000-1thread.dat
./MyRAMBench memory-RWS-10000000-2thread.dat
./MyRAMBench memory-RWS-10000000-4thread.dat
./MyRAMBench memory-RWR-10000000-1thread.dat
./MyRAMBench memory-RWR-10000000-2thread.dat
./MyRAMBench memory-RWR-10000000-4thread.dat
./MyRAMBench memory-RWS-1-1thread.dat
./MyRAMBench memory-RWS-1-2thread.dat
./MyRAMBench memory-RWS-1-4thread.dat
./MyRAMBench memory-RWR-1-1thread.dat
./MyRAMBench memory-RWR-1-2thread.dat
./MyRAMBench memory-RWR-1-4thread.dat
