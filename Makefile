CC=clang
CFLAGS=-g -std=c99 -Wall -pedantic 

all: 
	$(CC) $(CFLAGS) main.c -o main


clean:
	rm *.log