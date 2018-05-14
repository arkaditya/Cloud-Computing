###### Compile ######

make all

########### Disk Benchmark Input ###########

<mode (RS WS RR WR)>
<block size (1KB, 1000KB, 10000KB, 100000KB)>
<number of threads (1,2,4)> # For throughput
<number of threads (1,2,4,8,16,32,64,128)> # For latency

### PROGRAM DESC ###
The Program runs becnhmarking experiment on Disk Read Write operations suing Wrote Sequential,Read Sequential , Write Random and Read Random pattern on 1MB,10MB and 100MB of blocks on workload of 10GB.
It also performa latency measure and Input output operations per second using 1KB blocks on 1GB workload. 


## HOW TO RUN ##
1. Build the binary using make all
$ make all

2. RUn the file DiskThroughput_Random.sh and DiskThroughput_Sequential.sh to submit slurm jobs inside run_1.slurm run_2.slurm run_3.slurm run_4.slurm run_5.slurm run_6.slurm.
  These slurm jobs will first create a file in /tmp folder using write operation and then Read from the samefile.
	
$ ./DiskThroughput_Random.sh
$ ./DiskThroughput_Sequential.sh 
$ ./run_WR_batch.sh 	              //For latency computation

4. Output in output folder




