#include "macros.h"
#include "config.h"
#include "utils.h"
#include "comm.h"

int main() {
    int server_fd;
    struct sockaddr_in addr_client;

    char msg[MSG_LEN];
    char path[MSG_LEN];

    sync_info info_client;

    //char *test_file = "62692957_p0.png";
    char *test_file = "test_file2.txt";
    char *name = getenv("USER");

    if ((server_fd = initcon_remote(LOCALHOST, PORT_SERV, &addr_client, CON_REM_TM)) == -1) {
        exit(EXIT_FAILURE);
    }

    // once connected, read in client id
    sync_info_read(&info_client, FP_SYNC_INFO);
    
    // TODO: if failed to read in, then request server to create new one

    // start listening for commands from server

    // TODO: figure out how to now have one thread listening for server requests
    // and one thread for sending sync requests from this client
    parser_c(server_fd, &client_info);

    close(server_fd);
    return 0;
}
