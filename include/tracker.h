#ifndef TRACKER_H
#define TRACKER_H

#include <uuid/uuid.h>

#include "macros.h"
#include "utils.h"
#include "comm.h"
#include "list.h"

int initialize_client_id(int server_fd);
list *load_changelog(char *path);
int save_changelog(char *path, list *changelog);

#endif
