#include "macros.h"
#include "config.h"
#include "utils.h"
#include "comm.h"

int main() {
    int server_fd;
    struct sockaddr_in addr_client;
    char msg[MSG_LEN];
    char path[MSG_LEN];
    //char *test_file = "62692957_p0.png";
    char *test_file = "test_file2.txt";
    char *name = getenv("USER");

    if ((server_fd = initcon_remote(LOCALHOST, PORT_SERV, &addr_client, CON_REM_TM)) == -1) {
        exit(EXIT_FAILURE);
    }

    // notify server of connection
    /*snprintf(msg, MSG_LEN, "CLIENT %s: connection established.", name);
    if (send_msg(server_fd, msg, strlen(msg)) == -1) {
        printf("Unable to respond to server. Exiting program.\n");
    }*/

    printf("Client created\nName: %s\n\n", name);
    
    printf("Listening for further commands\n\n");

    // loop to constantly check for commands
    while(1) {
        memset(path, 0, MSG_LEN);
        memset(msg, 0, MSG_LEN);

        strcpy(path, SYNC_ROOT);
        strcat(path, TEST_FILE);

        strcpy(msg, RETRIEVE);
        strcat(msg, ":");
        strcat(msg, path);

        printf("Sending request: %s\n\n", msg);
        
        send_msg(server_fd, msg, MSG_LEN, strlen(msg));

        memset(path, 0, MSG_LEN);

        strcpy(path, SYNC_ROOT_REMOTE);
        strcat(path, TEST_FILE);
        recv_file(server_fd, path);

        printf("Finished recieving file\n\n");

        break;
    }

    close(server_fd);
    return 0;
}
