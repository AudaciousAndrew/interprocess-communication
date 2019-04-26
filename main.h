#include "banking.h"

#define PROC_MAX 10
#define FD_MAX 2
#define ARR_SIZE 11
#define STDIN 0
#define STDOUT 1
#define ARGUMENTS_OFFSET 3

int proc_number;
int pipes[PROC_MAX+1][PROC_MAX+1][FD_MAX];
balance_t balances[PROC_MAX];

Message init_msg(MessageType type, size_t payload_len);
