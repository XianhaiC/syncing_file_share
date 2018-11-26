/*
 * general req/cmd functions shared by both server and client
 */

int prompt_req(int sock_fd, int cmd) {
    return int stat_comm = send_int32_t(sock_fd, cmd);
}

// use to block a req until a response from the requested node is recieved
int resp_await(int sock_fd) {
    int32_t response = 0;
    recv_int32_t(sock_fd, &response);
    return response;
}


// use to let caller know that request was completed
// flag denotes the condition/context of provided process
int resp_send(int sock_fd, int32_t flag) {
    send(sock_fd, flag);
    // TODO: error handling
}
