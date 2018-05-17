#!/bin/bash

START_TIME=$SECONDS

hadoop jar HadoopSortPartition.jar HadoopSortPartition -D mapreduce.job.reduces=20 /input/data-80GB /user/averma16/partition /user/averma16/output80G-hadoop

ELAPSED_TIME=$(($SECONDS - $START_TIME))

echo "VALIDATING THE HADOOP OUTPUT ==> 80GB..."

hadoop jar /opt/hadoop-2.9.0/share/hadoop/mapreduce/hadoop-mapreduce-examples-2.9.0.jar teravalidate /user/averma16/output80G-hadoop /user/averma16/report-hadoop

echo -e "Time taken: $ELAPSED_TIME secs"

hadoop fs -get /user/averma16/report-hadoop/part-r-00000

mv part-r-00000 part-r-00000_HADOOP_80GB
