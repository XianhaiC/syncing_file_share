// handles incoming command parsing

int parser_c(int sock_fd) {
    int stat_comm;
    int32_t cmd;

    while (1) {
        stat_comm = recv_int32_t(sock_fd, &cmd);

        // TODO: write the handler methods for each client request
        switch(cmd) {
            case 
        }
    }
}
