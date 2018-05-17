#!/bin/bash

#`hadoop jar HadoopSort.jar HadoopSort /input/data-20GB /user/averma16/output20G-hadoop -D dfs.blocksize=128m -D mapreduce.job.reduces=16 -D mapreduce.task.io.sort.mb=1024`

START_TIME=$SECONDS

hadoop jar HadoopSort.jar HadoopSort -D mapreduce.job.reduces=2 /input/data-20GB /user/averma16/output20G-hadoop

ELAPSED_TIME=$(($SECONDS - $START_TIME))

echo "VALIDATING THE OUTPUT HADOOP ==> 20GB ..."

hadoop jar /opt/hadoop-2.9.0/share/hadoop/mapreduce/hadoop-mapreduce-examples-2.9.0.jar teravalidate /user/averma16/output20G-hadoop /user/averma16/report-hadoop

hadoop fs -get /user/averma16/report-hadoop/part-r-00000

echo -e "Time taken is: $ELAPSED_TIME secs"

mv part-r-00000 part-r-00000_Hadoop20GB
