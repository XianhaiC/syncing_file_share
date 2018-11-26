#include "sync_client.h"

// handles client side conflict resolution
// creates a duplicate of file specified by path_ori
// overwrites existing dupes with identical path_ori
// and editor client
sc_conflict_res(int sock_fd, char *path_ori, uuid_t id) {
    int status_send_file;
    char path_dup[BUF_LEN];

    // create path_dup
    su_get_path_dup(path_ori, path_dup, id);

    // create the dup file
    sc_file_dup(path_ori, path_dup, id);

    // TODO: we want the server to copy over this file. So, we must inform the
    // server to call its hl_download. To do this, create dedicated request
    // functions that communicate with the server this way
    // Remember that the server and client will split off individual threads
    // upon requests. This way they can take more requests while others are
    // running
    stat_comm = req_download(sock_fd, path_dup);
}

// duplicates a given file, giving it file path:
// <path_ori>.dup~<id>
void sc_file_dup(char *path_ori, char *path_dup, uuid_t id) {
    file *fp_ori;
    file *fp_dup;
    char buf[buf_len];
    unsigned int br;

    // check that original file exists
    if (access(path_ori, f_ok) == -1) {
        return;
    }
    
    // open relevant files
    fp_ori = fopen(path_ori, "r");
    fp_dup = fopen(path_dup, "w"); 

    while (1) {
        // read contents into buf
        br = fread(buf, sizeof(char), buf_len, fp_ori);
        // write contents to dupe file
        fwrite(buf, sizeof(char), br, fp_dup);
        
        // if eof has been reached, we are done
        if (feof(fp)) {
            break;
        }
    }

    return;
}
