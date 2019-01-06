#include "macros.h"
#include "config.h"
#include "utils.h"
#include "comm.h"

int main() {
    int server_fd;
    int resp;
    struct sockaddr_in addr_client;

    char msg[MSG_LEN];
    char path[MSG_LEN];

    sync_info si_client;

    //char *test_file = "62692957_p0.png";
    char *test_file = "test_file2.txt";
    char *name = getenv("USER");

    if ((server_fd = initcon_remote(LOCALHOST, PORT_SERV, &addr_client, CON_REM_TM)) == -1) {
        exit(EXIT_FAILURE);
    }

    // connected, read in local sync_info
    if (sync_info_read(&si_client, FP_SYNC_INFO)) {
        // encountered an error, request new id
        req_login(server_fd, &si_client, 1);
    }
    else {
        // send existing id to server
        req_login(server_fd, &si_client, 0);
    }

    // command loop
    // do stuff here

    sync_synchronize(

    close(server_fd);
    return 0;
}
