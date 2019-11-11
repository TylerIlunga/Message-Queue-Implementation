/* Program allows user to publish messages to a Message Queue */
#include "publish.h"

#define PERMISSIONS 0644
#define INITIAL_MESSAGE "Please enter the path for the Message Queue below[without extension]:\n"
#define ID_REQUEST_MESSAGE "Please enter the ID for the Message Queue below:\n"
#define KEY_CREATED_MESSAGE "Key for Message Queue has been created!\n"
#define CONNECTION_ERROR "Could not connect to Message Queue: "
#define CONNECTION_SUCCESS "Connected!\nTo Exit: ['Exit'||'exit'||^C||^D]\nEnter your messages below:\n"
#define PUBLISH_ERROR "Error while attempting to publish message"
#define CLOSING_CONNECTION "Closing Connection...\n"


void publish_messages()
{
    std::string path_to_msgq;
    std::string path_id;
    key_t msgq_key;
    int msgq_id;
    struct msg_buffer buffer;

    std::cout << INITIAL_MESSAGE;
    std::cin >> path_to_msgq;
    path_to_msgq += ".txt";
    std::cout << ID_REQUEST_MESSAGE;
    std::cin >> path_id;
    if ((msgq_key = ftok(path_to_msgq.c_str(), path_id[0])) == -1)
    {
        throw std::runtime_error(std::string(CONNECTION_ERROR + path_to_msgq));
    }
    std::cout << std::string(KEY_CREATED_MESSAGE);
    std::cout << "KEY: " << msgq_key << std::endl;
    if ((msgq_id = msgget(msgq_key, PERMISSIONS | IPC_CREAT)) == -1)
    {
        throw std::runtime_error(std::string(CONNECTION_ERROR + path_to_msgq));
    }

    std::cout << "MESSAGE QUEUE ID: " << msgq_id << std::endl;
    std::cout << CONNECTION_SUCCESS;
     
    buffer.msg_type = 1;

    while(std::getline(std::cin, buffer.msg_content)) {
        if (buffer.msg_content == "Exit" || buffer.msg_content == "exit") {
            break;
        }
        if (buffer.msg_content.length() > 0) {
            std::cout << "[SENT]: " << buffer.msg_content << std::endl;
            buffer.msg_content_len = buffer.msg_content.length();
            if (buffer.msg_content[buffer.msg_content_len - 1] == '\n') {
                buffer.msg_content[buffer.msg_content_len - 1] = '\0';
            }
            if (msgsnd(msgq_id, &buffer, buffer.msg_content_len + 1, 0) == -1) {
                throw std::runtime_error(PUBLISH_ERROR);
            }
        }
    }

    buffer.msg_content = "[END]";
    buffer.msg_content_len = buffer.msg_content.length();
     if (msgsnd(msgq_id, &buffer, buffer.msg_content_len + 1, 0) == -1) {
            throw std::runtime_error(PUBLISH_ERROR);
    }
    std::cout << CLOSING_CONNECTION;
}

int main(int argc, char **argv)
{
     publish_messages();
     return 0;
}