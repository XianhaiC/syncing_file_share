#include "../lib/utils.h"
#include "../lib/macros.h"

int main() {
    int comm_fd;
    struct sockaddr_in addr_client;
    char msg[MSG_LEN];
    char* name = getenv("USER");

    if ((comm_fd = initcon_remote(LOCALHOST, PORT_SERV, &addr_client)) == -1) {
        exit(EXIT_FAILURE);
    }

    // notify server of connection
    snprintf(msg, CMD_LENGTH, "CLIENT %s: connection established.", name);
    if (respond(comm_fd, msg) == -1) {
        printf("Unable to respond to server. Exiting program.\n");
    }

    printf("Client created\nName: %s\n\n", name);
    
    printf("Listening for further commands\n\n");

    // loop to constantly check for commands
    while(1) {
        // recieve the msg
        recieve(comm_fd, msg);

        // parse and execute the msg if valid and addressed to this client
        parse(comm_fd, msg, name);
    }
}
