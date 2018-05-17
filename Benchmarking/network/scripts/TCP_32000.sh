#!/bin/bash

name=$(echo $1 | cut -d '-' -f1 -)
node1=$(echo $1 | cut -d '-' -f2 - | tr -d '[')
node2=$(echo $1 | cut -d '-' -f3 - | tr -d ']')

for FILE in network-TCP-32000-1thread.dat network-TCP-32000-2thread.dat network-TCP-32000-4thread.dat network-TCP-32000-8thread.dat
do 
  if [ "$(hostname)" == "$name-$node1" ]
	then
#      	echo $(hostname)
        	./MyNETBench-TCP $FILE -S
	else
#        	echo $(hostname)
        	./MyNETBench-TCP $FILE -C $name-$node1
 fi 
 sleep 20
done

