##### Compile #####

make all

########### Memory Benchmark Input ###########

<benchmark_type (RWS RWR)>
<block size (1B, 1KB, 1MB, 10MB)> # in bytes
<number of threads (1,2,4)>

# PROGRAM DESCRITPTION  #
This program performs benchmarking for Memory by performing RWS and RWR operations on workload of 1GB using chunks of 1KB,1MB and 10MB.	
ReadWriteSequential and  ReadWriteRandom are the worker thread functions to compute which get executed depending  on the input from dat files.
RWSLatency and RWRLatency are the other worker thread functions which are used to compute the latency.

# HOW TO RUN #
1. Do make all to buld the binaries
$ make all

2. The Main script file run_memory.sh in turn executes each slurm file (run_RWS_1000.slurm,run_RWR_1000.slurm,run_RWS_1000000.slurm,run_RWR_1000000.slurm ,
run_RWS_10000000.slurm,run_RWR_10000000.slurm ,run_RWS_1.slurm,run_RWR_1.slurm) which submits slurm jobs in the queue.
$ ./run_memory.sh

3. The Output is redirected to the output directory.

