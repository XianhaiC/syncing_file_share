#ifndef SYNC_UTIL_H
#define SYNC_UTIL_H

#include <string.h>
#include <uuid/uuid.h>

#include "config.h"

void su_get_dup_path(char *path_ori, char *path_dup, uuid_t id);

#endif
