#include "sync_info.h"

// reads in sync_info from disk
int sync_info_read(sync_info *info, char *path) {
    FILE *fp;
    int br;

    fp = fopen(path, "r");

    br = fread(info, sizeof(sync_info), 1, fp);

    // TODO: err handling
    close(fp);
    return 0;
}
