/* Program allows users to subscribe to messages published on the Message Queue */
#include "queue.h"

#define PERMISSIONS 0644
#define INITIAL_MESSAGE "Please enter the path for the Message Queue below[without extension]:\n"
#define ID_REQUEST_MESSAGE "Please enter the ID for the Message Queue below:\n"
#define KEY_CREATED_MESSAGE "Key for Message Queue has been created!\n"
#define CONNECTION_ERROR "Could not connect to Message Queue: "
#define CONNECTION_SUCCESS "Connected!\nTo Exit: [^C]\n"
#define CONSUME_ERROR "Error while consuming message from queue"

void subscribe_to_queue()
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
    if ((msgq_id = msgget(msgq_key, PERMISSIONS)) == -1)
    {
        throw std::runtime_error(std::string(CONNECTION_ERROR + path_to_msgq));
    }

    std::cout << "MESSAGE QUEUE ID: " << msgq_id << std::endl;
    std::cout << CONNECTION_SUCCESS;

    while (true)
    {
        if (msgrcv(msgq_id, &buffer, sizeof(buffer.mtext), 0, 0) == -1)
        {
            throw std::runtime_error(CONSUME_ERROR);
        }
        printf("[CONSUMED]: %s\n", buffer.mtext);
    }
}

int main(int argc, char **argv)
{
    subscribe_to_queue();
    return 0;
}
