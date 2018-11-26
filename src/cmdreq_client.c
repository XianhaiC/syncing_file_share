/*
 * cmd and req functions for the client
 */

// cmd duplicates a given file, giving it file path:
// <path_ori>.dup~<id>
void cmdc_dupe(int sock_fd, uuid_t id) {
    char path_ori[MSG_LEN];
    char path_dupe[MSG_LEN];
    file *fp_ori;
    file *fp_dup;
    char buf[buf_len];
    unsigned int br;

    int stat_comm;

    // obtain the file to dupe's path
    stat_comm = recv_msg(sock_fd, path_ori, MSG_LEN);
    
    // create the dupe path
    su_dupe_path(path_ori, path_dupe, id);

    // check that original file exists
    if (access(path_ori, f_ok) == -1) {
        // TODO: give the server a proper response
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
    
    resp_send(sock_fd, RESP_SUCCESS);
    return;
}

// cmd upload file to server
void cmdc_upload(int sock_fd) {
    char path[MSG_LEN];
    int stat_comm;

    // obtain the file to upload's path
    stat_comm = recv_msg(sock_fd, path, MSG_LEN);
    
    stat_comm = send_file(sock_fd, path);

    resp_send(sock_fd, RESP_SUCCESS);
    return;
}

// cmd delete file requested by server
void cmdc_delete(int sock_fd) {
    char path[MSG_LEN];
    int stat_comm;
    
    // obtain the file to delete's path
    stat_comm = recv_msg(sock_fd, path, MSG_LEN);

    if (remove(path)) {
        // TODO: error handling
    }

    resp_send(sock_fd, RESP_SUCCESS);
    return;
}
// cmd download file from client

