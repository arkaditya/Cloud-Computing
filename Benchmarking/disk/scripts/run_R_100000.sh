#!/bin/bash

./MyDiskBench disk-WR-100000-1thread.dat
./MyDiskBench disk-WR-100000-2thread.dat
./MyDiskBench disk-WR-100000-4thread.dat

sleep 2m

./MyDiskBench disk-RR-100000-1thread.dat
./MyDiskBench disk-RR-100000-2thread.dat
./MyDiskBench disk-RR-100000-4thread.dat

