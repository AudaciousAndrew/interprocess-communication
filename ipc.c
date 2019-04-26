#include <errno.h>
#include <unistd.h>
#include "main.h"
#include "ipc.h"
#include "proc.h"

int send(void * self, local_id dst, const Message * msg){
	process *p   = (process*)self;
    local_id src  = p->id;
    if (src == dst) return -1;
    int fd   = pipes[src][dst][STDOUT];
    return write(fd, msg, sizeof(MessageHeader) + msg->s_header.s_payload_len);

}


int send_multicast(void * self, const Message * msg) {
	for (local_id i = 0; i <= proc_number; i++) {
        send(self, i, msg);
    }
    return 0;
}

int receive(void *self, local_id from, Message *msg) {
    process *p   = (process*)self;
    local_id dst  = p->id;
    if (dst == from) return -1;
    int fd   = pipes[from][dst][STDIN];
    int read_result = read(fd, msg, sizeof(Message));
    if (read_result < 0) {
        perror("read");
        return -1;
    }
    return read_result > 0 ? 0 : -1;
}

int receive_any(void *self , Message *msg){
    for(int i = 0 ; i < proc_number ; i++){
        if(receive(self, i, msg) == 0)
            return 0;
    }
    return -1;
}
