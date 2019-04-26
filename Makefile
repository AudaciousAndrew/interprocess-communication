CC=clang
CFLAGS= -std=c99 -Wall -pedantic -g -Llib64 -lruntime
TARGET=lab2

all: 
	$(CC) $(CFLAGS) main.c ipc.c transfer.c -o main

.PHONY: clean
clean:
	rm $(TARGET).tar.gz
	rm main
	rm *.log
	
.PHONY: tar
tar:
	tar zcf $(TARGET).tar.gz ../lab1/