#include "cmd_util.h"
/*
 * general req/cmd functions shared by both server and client
 */

int cmd_prompt(int sock_fd, int32_t cmd) {
    return send_int32_t(sock_fd, cmd);
}

int cmd_acknowledge(int sock_fd, int32_t *cmd) {
    return recv_int32_t(sock_fd, cmd);
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
    send_int32_t(sock_fd, flag);
    // TODO: error handling
}
