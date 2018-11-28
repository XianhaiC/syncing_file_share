/*
 * cmd and req functions for the server
 */

// cmd upload file to client
// cmd download file from client

// req ask client to create dupe of file
int reqc_dupe(int sock_fd, char *path_ori) {
    int stat_comm;
    int len_path_ori;

    // prompt client with request
    stat_comm = send_cmd(sock_fd, CMD_C_DUPE);
    
    // send the client the file to dupe
    len_path_ori = strlen(path_ori); 
    stat_comm = send_msg(sock_fd, path_ori, len_path_ori, len_path_ori);

    return resp_await(sock_fd);
}

// req client to upload file to server
int reqc_upload(int sock_fd, char *path) {
    int stat_comm;
    int len_path;

    // prompt client with request
    stat_comm = prompt_req(sock_fd, CMD_C_UPLOAD);

    // send the client the file to upload
    len_path = strlen(path); 
    stat_comm = send_msg(sock_fd, path, len_path, len_path);

    // wait and recv the file
    stat_comm = recv_file(sock_fd, path);
    
    return resp_await(sock_fd);
}

int reqc_download(int sock_fd, char *path) {
    int stat_comm;
    int len_path;

    // prompt client with request
    stat_comm = prompt_req(sock_fd, CMD_C_DOWNLOAD);

    // send the client the file to upload
    len_path = strlen(path); 
    stat_comm = send_msg(sock_fd, path, len_path, len_path);

    // send the file
    stat_comm = send_file(sock_fd, path);
    
    return resp_await(sock_fd);
}

int reqc_delete(int sock_fd, char *path) {
    int stat_comm;
    int len_path;

    // prompt client with request
    stat_comm = prompt_req(sock_fd, CMD_C_DELETE);

    // send the client the file to delete
    len_path = strlen(path); 
    stat_comm = send_msg(sock_fd, path, len_path, len_path);

    return resp_await(sock_fd);
}

// req obtain client id
int reqc_sync_info(int sock_fd, sync_info *info) {
    int stat_comm;

    // prompt client with request
    stat_comm = prompt_req(sock_fd, CMD_C_SYNC_INFO);

    // obtain id from client
    status_comm = recv_msg(sock_fd, info,
            sizeof(sync_info), sizeof(sync_info));

    return resp_await(sock_fd);
}

// req obtain client's changelog
int reqc_changelog(int sock_fd, char *path) {
    int stat_comm;

    // prompt client with request
    stat_comm = prompt_req(sock_fd, CMD_C_CHANGELOG);

    // receive the updated changelog file from the client
    stat_comm = recv_file(sock_fd, path);

    return resp_await(sock_fd);
}
