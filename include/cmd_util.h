#ifndef CMD_UTIL_H
#define CMD_UTIL_H

#include "comm.h"

int cmd_prompt(int sock_fd, int32_t cmd);
int cmd_acknowledge(int sock_fd, int32_t *cmd);
int resp_await(int sock_fd);
int resp_send(int sock_fd, int32_t flag);

#endif
