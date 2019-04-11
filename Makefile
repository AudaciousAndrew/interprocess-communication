CC=gcc
CFLAGS=-std=c99 -Wall -pedantic 

all: 
	$(CC) $(CFLAGS) main.c -o main


clean:
	rm *.log