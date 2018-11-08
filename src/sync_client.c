#include "sync_client.h"

// handles client side conflict resolution
// creates a duplicate of file specified by path_ori
// overwrites existing dupes with identical path_ori
// and editor client
sc_conflict_res(int sock_fd, char *path_ori, uuid_t id) {
    int status_send_file;
    char path_dup[BUF_LEN];

    // create path_dup
    sc_get_path_dup(path_ori, path_dup, id);

    // create the dup file
    sc_file_dup(path_ori, path_dup, id);

    // send the dup file to the server
    status_send_file = send_file(sock_fd, path_dup);
}

void sc_get_dup_path(char *path_ori, char *path_dup, uuit_t id) {
    // create path_dup with format:
    // <path>.dup~<id>
    strcpy(path_dup, path_ori);
    strcat(path_dup, DUP_EXT);
    strncat(path_dup, id, sizeof(uuid_t));
}

// duplicates a given file, giving it file path:
// <path_ori>.dup~<id>
void sc_file_dup(char *path_ori, char *path_dup, uuid_t id) {
    FILE *fp_ori;
    FILE *fp_dup;
    char buf[BUF_LEN];
    unsigned int br;

    // check that original file exists
    if (access(path_ori, F_OK) == -1) {
        return;
    }
    
    // open relevant files
    fp_ori = fopen(path_ori, "r");
    fp_dup = fopen(path_dup, "w"); 

    while (1) {
        // read contents into buf
        br = fread(buf, sizeof(char), BUF_LEN, fp_ori);
        // write contents to dupe file
        fwrite(buf, sizeof(char), br, fp_dup);
        
        // If EOF has been reached, we are done
        if (feof(fp)) {
            break;
        }
    }

    return;
}
