#!/bin/bash


./MyDiskBench disk-WR-10000-1thread.dat
./MyDiskBench disk-WR-10000-2thread.dat
./MyDiskBench disk-WR-10000-4thread.dat

sleep 2m

./MyDiskBench disk-RR-10000-1thread.dat
./MyDiskBench disk-RR-10000-2thread.dat
./MyDiskBench disk-RR-10000-4thread.dat

