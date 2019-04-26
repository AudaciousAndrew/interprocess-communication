CC=clang
CFLAGS=-g -std=c99 -Wall -pedantic 

all: 
	$(CC) $(CFLAGS) main.c ipc.c -o main

clean:
	rm main
	rm *.log