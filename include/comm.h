#ifndef COMM_H
#define COMM_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#include "macros.h"
#include "utils.h"

int32_t send_msg(int sock_fd, char *msg, int32_t msg_len, int32_t to_send);
int32_t recv_msg(int sock_fd, char *msg, int32_t msg_len);
int send_int32_t(int sock_fd, int32_t num);
int recv_int32_t(int sock_fd, int32_t *num);
int32_t send_file(int sock_fd, char *path);
int32_t recv_file(int sock_fd, char *path);
void *get_in_addr(struct sockaddr *sa);
void fill_sockaddr(int family, char *ip, int port, struct sockaddr_in *addr);
int initcon_local(char *ip, int port, struct sockaddr_in *addr);
int initcon_remote(char *ip, int port, struct sockaddr_in *addr, float timeout);

#endif
