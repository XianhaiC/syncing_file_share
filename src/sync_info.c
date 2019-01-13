#include "sync_info.h"

// reads in sync_info from disk and attaches provided sock_fd to it
int sync_info_read(sync_info *info, int sock_fd, char *path) {
    FILE *fp;

    fp = fopen(path, "r");

    // read in sync_info stored at path
    fread(info, sizeof(sync_info), 1, fp);

    // TODO: error handling
    if (ferror(fp)) {
        fclose(fp);
        return 1;
    }

    info->sock_fd = sock_fd;

    fclose(fp);
    return 0;
}
