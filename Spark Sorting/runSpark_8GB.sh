#!/bin/bash

START_TIME=$SECONDS

spark-submit --class SparkSort --master yarn --deploy-mode client --driver-memory 1g --executor-memory 2g --executor-cores 2 --num-executors 16 SparkSort.jar /input/data-8GB/data-8GB.in /user/averma16/output-spark

ELAPSED_TIME=$(($SECONDS - $START_TIME))

echo " VALIDATING SPARK OUTPUT ==> 8GB "

hadoop jar /opt/hadoop-2.9.0/share/hadoop/mapreduce/hadoop-mapreduce-examples-2.9.0.jar teravalidate /user/averma16/output-spark /user/averma16/report-spark

echo -e "Time taken: $ELAPSED_TIME secs"

`hadoop fs -get /user/averma16/report-spark/part-r-00000`

mv part-r-00000 part-r-00000_8GB
