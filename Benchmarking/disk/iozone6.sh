#!/bin/bash

echo -e '####### IOZONE  WS #################'

iozone -s 10g -r 100m -t 1 -F /tmp/File1.txt -i 0 -l 1 -u 1  -T


