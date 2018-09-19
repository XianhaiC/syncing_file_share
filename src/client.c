#include "../lib/macros.h"
#include "../lib/utils.h"
#include "../lib/comm.h"

int main() {
    int server_fd;
    struct sockaddr_in addr_client;
    char msg[MSG_LEN];
    char path[MSG_LEN];
    char test_file = "test1.txt";
    char* name = getenv("USER");

    if ((server_fd = initcon_remote(LOCALHOST, PORT_SERV, &addr_client)) == -1) {
        exit(EXIT_FAILURE);
    }

    // notify server of connection
    snprintf(msg, CMD_LENGTH, "CLIENT %s: connection established.", name);
    if (respond(server_fd, msg) == -1) {
        printf("Unable to respond to server. Exiting program.\n");
    }

    printf("Client created\nName: %s\n\n", name);
    
    printf("Listening for further commands\n\n");

    // loop to constantly check for commands
    while(1) {
        memset(path, 0, MSG_LEN);
        memset(msg, 0, MSG_LEN);

        strcpy(path, SYNC_ROOT);
        strcat(path, test_file);

        strcpy(msg, RETRIEVE);
        strcat(msg, ":");
        strcat(msg, path);

        printf("Sending request: %s\n\n", msg);
        
        send_msg(server_fd, msg, strlen(msg));

        memset(path, 0, MSG_LEN);

        strcpy(path, SYNC_ROOT_REMOTE);
        strcat(path, test_file);
        recv_file(server_fd, path);
    }
}
