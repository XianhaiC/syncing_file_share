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

int min(int x, int y);
int max(int x, int y);
char* str_concat(const char *s1, const char *s2);

#endif
