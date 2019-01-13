#ifndef SYNC_INFO_H
#define SYNC_INFO_H

#include <stdio.h>
#include <uuid/uuid.h>

typedef struct _sync_info sync_info;
struct _sync_info {
    uuid_t id;
    int sock_fd;
};

// reads in sync_info from disk
int sync_info_read(sync_info *info, int sock_fd, char *path);

#endif
