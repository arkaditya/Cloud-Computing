#!/bin/bash

./MyDiskBench disk-WS-1000-1thread.dat
./MyDiskBench disk-WS-1000-2thread.dat
./MyDiskBench disk-WS-1000-4thread.dat

sleep 2m

./MyDiskBench disk-RS-1000-1thread.dat
./MyDiskBench disk-RS-1000-2thread.dat
./MyDiskBench disk-RS-1000-4thread.dat

