#include <cstdlib>
#include <cstdio>
#include <errno.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string>
#include <iostream>

void publish_messages();

struct msg_buffer {
    long msg_type;
    std::string msg_content;
    int msg_content_len;
};