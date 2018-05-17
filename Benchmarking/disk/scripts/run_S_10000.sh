#!/bin/bash


./MyDiskBench disk-WS-10000-1thread.dat
./MyDiskBench disk-WS-10000-2thread.dat
./MyDiskBench disk-WS-10000-4thread.dat

sleep 2m

./MyDiskBench disk-RS-10000-1thread.dat
./MyDiskBench disk-RS-10000-2thread.dat
./MyDiskBench disk-RS-10000-4thread.dat

