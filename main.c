#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "pa1.h"
#include "ipc.h"

#define PROC_MAX 10
#define FD_MAX 2
#define ARR_SIZE 11
#define STDIN 0
#define STOUT 1

int proc_number;

/** Gets ammount of processes from command line
*/

int get_args(int argc, char **argv) {
	if (argc != 3 || (strcmp(argv[1], "-p") != 0)) 	return -1;
	proc_number = atoi(argv[2]);
	return 0;
}


/** Initialize pipes
*/
int initialize_pipes(int pipes[ARR_SIZE][ARR_SIZE][FD_MAX], FILE *pipes_log){
	int count = 1;
	for(int i = 0; i <= proc_number; i++){
		for(int j = 0; j <= proc_number; j++){
			if( i == j){
				pipes[i][j][0] = -1;
				pipes[i][j][1] = -1;
				continue;
			}
			if(pipe(pipes[i][j]) < 0){
				perror("Couldn't create pipe.");
				fprintf(pipes_log, "Couldn't create pipe:%d.\nTerminate programm.", count);
				return -1;
			}
			if (fprintf(pipes_log, "Pipe number:%d was created.\n", count++) < 0 );
		}
	}
	return 0;
}

/** Close unused file descriptors
*/

void close_fds(local_id id, FILE *pipes_log, int pipes[ARR_SIZE][ARR_SIZE][FD_MAX]){
	for (local_id m = 0; m <= proc_number; m++){
		for(local_id n = 0; n <= proc_number; n++){
			if (m != n) {
				if (m == id) {
					close(pipes[m][n][STDIN]);
					fprintf(pipes_log, "PID:%d closed read: %hhd -- %hhd\n", id , m , n);
				}

				if (n == id) {
					close(pipes[m][n][STOUT]);
					fprintf(pipes_log, "PID:%d closed write: %hhd -- %hhd\n", id , m , n);
				}

				if (m != id && n != id) {
					close(pipes[m][n][STDIN]);
					close(pipes[m][n][STOUT]);
					fprintf(pipes_log, "PID:%d closed pipe: %hhd -- %hhd\n", id , m , n);
				}
			}
		}
	}
	fprintf(pipes_log, "PID:%d closed all file descriptors.\n", id);
}

int main(int argc, char **argv) {

	FILE *pipes_log = NULL;
	FILE *events_log = NULL;
	int pipes[PROC_MAX+1][PROC_MAX+1][FD_MAX];

	if(!get_args(argc, argv)) {
		perror("can't read arguments");
		return -1;
	}
	
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

	if ( initialize_pipes(pipes, pipes_log) < 0) return -1;
	
	close_fds(PARENT_ID, pipes_log, pipes);
	fclose(pipes_log);
	fclose(events_log);
	return 0;
}
