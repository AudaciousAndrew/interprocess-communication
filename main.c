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

int create_logs(FILE *pipes_log , FILE *events_log){
	pipes_log = fopen("pipes.log","w");
	if(pipes_log == NULL){
		 perror("can't open/create pipes.log file (fopen)");
		return -1;
	}
	events_log = fopen("events.log" , "w+");
	if(events_log == NULL){
		 perror("can't open/create events.log file (fopen)");
		return -1;
	}
	return 0;
}

int main(int argc, char **argv) {
	
	FILE *pipes_log = NULL;
	FILE *events_log = NULL;

	if(!get_args(argc, argv)) return -1;
	
	if ( create_logs(pipes_log , events_log) < 0) return -1;
	
	return 0;
}
