#!/bin/bash

./MyDiskBench disk-WR-1000-1thread.dat
./MyDiskBench disk-WR-1000-2thread.dat
./MyDiskBench disk-WR-1000-4thread.dat

sleep 2m

./MyDiskBench disk-RR-1000-1thread.dat
./MyDiskBench disk-RR-1000-2thread.dat
./MyDiskBench disk-RR-1000-4thread.dat

