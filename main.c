#include "common.h"
#include "pa1.h"
#include "ipc.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int n;

int get_args(int argc, char **argv) {
	if (argc != 3 || (strcmp(argv[1], "-p") != 0)) 	return -1;
	n = atoi(argv[2]);
	return 0;
}

int main(int argc, char **argv) {
	if(!get_args(argc, argv)) return -1;
	return 0;
}
