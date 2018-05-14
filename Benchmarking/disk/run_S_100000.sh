#!/bin/bash

./MyDiskBench disk-WS-100000-1thread.dat
./MyDiskBench disk-WS-100000-2thread.dat
./MyDiskBench disk-WS-100000-4thread.dat

sleep 2m

./MyDiskBench disk-RS-100000-1thread.dat
./MyDiskBench disk-RS-100000-2thread.dat
./MyDiskBench disk-RS-100000-4thread.dat

