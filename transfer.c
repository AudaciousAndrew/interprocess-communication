#include <string.h>
#include <stdio.h>
#include "ipc.h"
#include "banking.h"
#include "main.h"

void transfer(void * parent_data, local_id src, local_id dst,
              balance_t amount)
{

Message msg = init_msg(TRANSFER,sizeof(TransferOrder));

TransferOrder order = (TransferOrder){
	.s_src = src,
	.s_dst = dst,
	.s_amount = amount
};

memcpy(msg.s_payload, &order, sizeof(TransferOrder));

fprintf(stderr, "%d: process 0 initiate transmission (%d -- %d).\n", get_physical_time(), src, dst);
send(parent_data, src , &msg);
while (receive(parent_data, dst, &msg))
	if(msg.s_header.s_type == ACK)
		break;
fprintf(stderr, "%d: process 0 received ACK from %d.\n", get_physical_time(), dst);
}

