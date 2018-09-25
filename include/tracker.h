#ifndef TRACKER_H
#define TRACKER_H

#include <uuid/uuid.h>

#include "macros.h"
#include "utils.h"
#include "comm.h"

int initialize_client_id(int server_fd);
char **load_changelog(char *path);

#endif
