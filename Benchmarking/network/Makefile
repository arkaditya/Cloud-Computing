CC=gcc
CFLAGS=-g -Wall -lpthread -O2 -pthread

all:
	$(CC) $(CFLAGS) -o MyNETBench-TCP MyNETBench-TCP.c
	$(CC) $(CFLAGS) -o MyNETBench-UDP MyNETBench-UDP.c

clean:
	$(RM) *.o MyNETBench-TCP MyNETBench-UDP
