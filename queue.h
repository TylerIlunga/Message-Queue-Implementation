#include <cstdlib>
#include <cstdio>
#include <errno.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string>
#include <iostream>

/* Queue */
void create_message_queue(key_t &msgq_key, int &msgq_id);
void delete_message_queue();
void handle_operations(key_t &msgq_key, int &msgq_id);

/* Publish/Subscribe */
void publish_messages();
void subscribe_to_queue();
struct msg_buffer {
    long mtype;
    char mtext[200];
    int msg_content_len;
};