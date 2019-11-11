/* Program execute operations for a Message Queue  */
#include "queue.h"

#define PERMISSIONS 0644
#define INITIAL_MESSAGE "What operation would you like to execute:\n[1] Create Message Queue\n[2] Delete Message Queue\n"
#define INVALID_OP_ARGUMENT "Invalid operation argument entered by user."
#define PATH_REQUEST_MESSAGE "Please enter the path for the Message Queue below[without extension]:\n"
#define ID_REQUEST_MESSAGE "Please enter the ID for the Message Queue below[without extension]:\n"
#define KEY_CREATED_MESSAGE "Key for Message Queue has been created!\n"
#define CREATE_QUEUE_ERROR "Could not create Message Queue with path: "
#define DELETE_QUEUE_ERROR "Could not delete Message Queue with path: "

void create_message_queue(key_t &msgq_key, int &msgq_id)
{
    std::string path;
    std::string path_w_ext;
    std::string path_id;
    std::string create_command;

    std::cout << PATH_REQUEST_MESSAGE;
    std::cin >> path;
    path_w_ext = path + ".txt";
    if (system(("touch " + path_w_ext).c_str()) == -1)
    {
        throw std::runtime_error(std::string(CREATE_QUEUE_ERROR + path));
    }
    std::cout << std::string("File for Message Queue with path " + path + " has been created!\n");
    std::cout << ID_REQUEST_MESSAGE;
    std::cin >> path_id;
    if ((msgq_key = ftok(path_w_ext.c_str(), path_id[0])) == -1)
    {
        int r = remove(path_w_ext.c_str());
        throw std::runtime_error(std::string(CREATE_QUEUE_ERROR + path));
    }
    std::cout << std::string(KEY_CREATED_MESSAGE);
    if ((msgq_id = msgget(msgq_key, PERMISSIONS | IPC_CREAT)) == -1)
    {
        int r = remove(path_w_ext.c_str());
        throw std::runtime_error(std::string(CREATE_QUEUE_ERROR + path));
    }
    // NOTE:: MAYBE STORE KEY AND ID ADDRESSES IN SHARED MEMORY FOR PUBLISH AND SUBSCRIBE PROCESS TO FETCH FROM.
    std::cout
        << std::string("Message Queue with path " + path_w_ext + " has been created!\n")
        << "KEY: "
        << msgq_key << "\nID: "
        << msgq_id << "\n";
}

void delete_message_queue()
{
    std::string msgq_id;
    std::string path;
    
    std::cout << ID_REQUEST_MESSAGE;
    std::cin >> msgq_id;

    if (msgctl(std::stoi(msgq_id), IPC_RMID, NULL) == -1) {
        throw std::runtime_error(std::string(DELETE_QUEUE_ERROR + path));
    }
    std::cout << PATH_REQUEST_MESSAGE;
    std::cin >> path;
    path += ".txt";
    if (remove(path.c_str()) == -1)
    {
        throw std::runtime_error(std::string(DELETE_QUEUE_ERROR + path));
    }
    std::cout << std::string("Message Queue with path " + path + " has been deleted!\n");
}

void handle_operations(key_t &msgq_key, int &msgq_id)
{
    std::string userInput;

    std::cout << INITIAL_MESSAGE;
    std::cin >> userInput;
    if (!(userInput == "1" || userInput == "2"))
    {
        throw std::invalid_argument(INVALID_OP_ARGUMENT);
    }
    switch (std::stoi(userInput))
    {
    case 1:
        create_message_queue(msgq_key, msgq_id);
        break;
    case 2:
        delete_message_queue();
        break;
    }
    handle_operations(msgq_key, msgq_id);
}

int main(int argc, char **argv)
{
    int msgq_id;
    key_t msgq_key;
    handle_operations(msgq_key, msgq_id);
    return 0;
}