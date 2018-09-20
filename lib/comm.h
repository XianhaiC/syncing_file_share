#ifndef COMM_H
#define COMM_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "macros.h"
#include "utils.h"

int send_msg(int sock_fd, char *msg, int msg_len, int to_send);
int recv_msg(int sock_fd, char *msg, int msg_len);
int send_int(int sock_fd, long num);
int recv_int(int sock_fd, long *num);
int send_file(int sock_fd, char *path);
int recv_file(int sock_fd, char *path);
void *get_in_addr(struct sockaddr *sa);
void fill_sockaddr(int family, char *ip, int port, struct sockaddr_in *addr);
int initcon_local(char *ip, int port, struct sockaddr_in *addr);
int initcon_remote(char *ip, int port, struct sockaddr_in *addr, float timeout);

#endif
