#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "common.h"
#include "pa2345.h"
#include "ipc.h"
#include "proc.h"
#include "banking.h"
#include "main.h" // constants and global variables



/** Gets ammount of processes from command line
*/

int get_args(int argc, char **argv) {
	if (argc != atoi(argv[2])+ARGUMENTS_OFFSET || (strcmp(argv[1], "-p") != 0)) 	return -1;
	proc_number = atoi(argv[2]);
	if(proc_number < 0) return -1;
	for (int i = 0; i < proc_number; i++){
		balances[i] = atoi(argv[i+ARGUMENTS_OFFSET]);
		if (balances[i] < 0){
			return -1;
		}
	}
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
			if (fprintf(pipes_log, "Pipe number:%d was created.\n", count++) < 0 ) return -1;
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
					close(pipes[m][n][STDOUT]);
					fprintf(pipes_log, "PID:%d closed write: %hhd -- %hhd\n", id , m , n);
				}

				if (m != id && n != id) {
					close(pipes[m][n][STDIN]);
					close(pipes[m][n][STDOUT]);
					fprintf(pipes_log, "PID:%d closed pipe: %hhd -- %hhd\n", id , m , n);
				}
			}
		}
	}
	fprintf(pipes_log, "PID:%d closed all file descriptors.\n", id);
}

Message init_msg(MessageType type, char *payload, size_t payload_len) {
	Message msg;
	msg.s_header.s_magic = MESSAGE_MAGIC;
    msg.s_header.s_payload_len = payload_len;
    msg.s_header.s_type = type;
    msg.s_header.s_local_time  = 0;
    return msg;
}


int proc(local_id proc_id, FILE *pipes_log, FILE *events_log){
	char payload[MAX_PAYLOAD_LEN];
	size_t len;
	close_fds(proc_id, pipes_log, pipes);

	process p;
	p.id = proc_id;

	/*Process starts*/
	fprintf(events_log, log_started_fmt, proc_id,getpid(),getppid());
	len = fprintf(stdout, log_started_fmt, proc_id,getpid(),getppid());
    
    Message msg = init_msg(STARTED, payload, len);

   	send_multicast((void*)&p, &msg);
	for (int i = 1; i <= proc_number; i++) {
       if (i != p.id)
           while(receive((void*)&p, i, &msg) != 0);
    }

    fprintf(events_log, log_received_all_started_fmt, proc_id);
	fprintf(stdout, log_received_all_started_fmt, proc_id);

	/*Process is done*/
	fprintf(events_log, log_done_fmt, proc_id);
	len = fprintf(stdout, log_done_fmt, proc_id);
    
    msg = init_msg(DONE, payload, len);

   	send_multicast((void*)&p, &msg);
	for (int i = 1; i <= proc_number; i++) {
       if (i != p.id)
           while(receive((void*)&p, i, &msg) != 0);
    }

    fprintf(events_log, log_received_all_done_fmt, proc_id);
	fprintf(stdout, log_received_all_done_fmt, proc_id);
	return 0;

}

int main(int argc, char **argv) {

	FILE *pipes_log = NULL;
	FILE *events_log = NULL;

	if(get_args(argc, argv) !=0) {
		perror("can't read arguments");
		return -1;
	}
	
	pipes_log = fopen("pipes.log","w");
	if(pipes_log == NULL){
		 perror("can't open/create pipes.log file (fopen)");
		return -1;
	}
	events_log = fopen("events.log","w+");
	if(events_log == NULL){
		 perror("can't open/create events.log file (fopen)");
		return -1;
	}

	if ( initialize_pipes(pipes, pipes_log) < 0) return -1;
	
	for(local_id i = 1; i <= proc_number; i++){
		if(fork() == 0){
			int exit_code = proc(i, pipes_log, events_log);
			exit(exit_code);
		}
	}
	proc(PARENT_ID, pipes_log, events_log);
	while(wait(NULL) > 0);
	close_fds(PARENT_ID, pipes_log, pipes);
	fclose(pipes_log);
	fclose(events_log);
	return 0;
}
