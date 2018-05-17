#!/bin/bash

name=$(echo $1 | cut -d '-' -f1 -)
node1=$(echo $1 | cut -d '-' -f2 - | tr -d '[')
node2=$(echo $1 | cut -d '-' -f3 - | tr -d ']')

for FILE in network-UDP-1-1thread.dat network-UDP-1-2thread.dat network-UDP-1-4thread.dat 
do
  if [ "$(hostname)" == "$name-$node1" ]
	then
#      	echo $(hostname)
        	./MyNETBench-UDP $FILE -S
	else
#        	echo $(hostname)
        	./MyNETBench-UDP $FILE -C $name-$node1
 fi 
 sleep 20
done

