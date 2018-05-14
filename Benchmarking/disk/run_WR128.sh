#!/bin/bash

./MyDiskBench disk-WR-1-128thread.dat
sleep 10
./MyDiskBench disk-RR-1-128thread.dat

