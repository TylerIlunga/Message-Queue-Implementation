#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string>
#include <iostream>

void create_message_queue(key_t &msgq_key, int &msgq_id);
void delete_message_queue();
void handle_operations(key_t &msgq_key, int &msgq_id);