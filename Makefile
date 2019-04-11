CC=gcc
CFLAGS=-std=c99 -Wall -pedantic 

all: clean
	$(CC) $(CFLAGS) main.c -o main


clean:
	rm *.log