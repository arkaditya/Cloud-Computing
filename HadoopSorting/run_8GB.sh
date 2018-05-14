#!/bin/bash

#`hadoop jar HadoopSort.jar HadoopSort /input/data-8GB /user/averma16/output8G-hadoop -D dfs.blocksize=128m -D mapreduce.job.reduces=4 -D mapreduce.task.io.sort.mb=1024`
START_TIME=$SECONDS

hadoop jar HadoopSort.jar HadoopSort -D mapreduce.job.reduces=4 /input/data-8GB /user/averma16/output8G-hadoop

ELAPSED_TIME=$(($SECONDS - $START_TIME))

echo "VALIDATING THE OUTPUTi  ==> 8GB "

hadoop jar /opt/hadoop-2.9.0/share/hadoop/mapreduce/hadoop-mapreduce-examples-2.9.0.jar teravalidate /user/averma16/output8G-hadoop /user/averma16/report-hadoop

hadoop fs -get /user/averma16/report-hadoop/part-r-00000

echo -e "Time taken: $ELAPSED_TIME secs"

mv part-r-00000 part-r-00000_HADOOP_8GB

