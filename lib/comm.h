#ifndef COMM_H
#define COMM_H

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "macros.h"

int send_msg(int sock_fd, char *msg, int msg_len);
int recv_msg(int sock_fd, char *msg, int msg_len);
int send_int(int sock_fd, long num);
int recv_int(int sock_fd, long *num);
int send_file(int sock_fd, char *path);
int recv_file(int sock_fd, char *path);

#endif
