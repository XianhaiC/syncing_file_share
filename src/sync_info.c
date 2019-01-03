#include "sync_info.h"

// reads in sync_info from disk and attaches provided sock_fd to it
int sync_info_read(sync_info *info, int sock_fd, char *path) {
    FILE *fp;
    int br;

    fp = fopen(path, "r");

    br = fread(info, sizeof(sync_info), 1, fp);

    // TODO: err handling

    info->sock_fd = sock_fd;

    close(fp);
    return 0;
}
