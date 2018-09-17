#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "macros.h"

char* str_concat(const char *s1, const char *s2);
char* alias_img(void);
int respond (int s, char *msg_buf);
int recieve (int s, char *msg);
void *get_in_addr(struct sockaddr *sa);
void fill_sockaddr(int family, char *ip, int port, struct sockaddr_in *addr);
int initcon_local(char *ip, int port, struct sockaddr_in *addr);
int initcon_remote(char *ip, int port, struct sockaddr_in *addr, float timeout);

#endif
