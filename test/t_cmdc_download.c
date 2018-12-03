#include "test.h"

// tests client's cmd download
// client side test
int main() {
    int server_fd;
    struct sockaddr_in addr_client;
    char path[BUF_LEN];

    sync_info info_client;

    // get client user id
    char *name = getenv("USER");

    // connect to the server
    if ((server_fd = initcon_remote(LOCALHOST, PORT_TEST, &addr_client, CON_REM_TM)) == -1) {
        exit(EXIT_FAILURE);
    }

    // TODO: finish writing tests for the download
    // start listening for requests
    while (1) {
        stat_comm = recv_int32_t(server_fd, &cmd);

        switch (cmd) {
            case CMD_C_DOWNLOAD:
                break;
            case CMD_C_RECV_CHECKSUM:
                break;
        }
        if (cmd == CMD_C_KILL) {
            exit(EXIT_SUCCESS);
        }

        TEST(cmd == CMD_C_DOWNLOAD);
        if (cmd != CMD_C_DOWNLOAD) {
            exit(EXIT_FAILURE);
        }

        // download the file
        cmdc_download(server_fd, &path);

        // retreive file's checksum
        cmdc_recv_checksum(server_fd, &checksum);

        // check the downloaded file's checksum
        checksum_file(&path, &checksum);
    }

    close(server_fd);
    return 0;
}
