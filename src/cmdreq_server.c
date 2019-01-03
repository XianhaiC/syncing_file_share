/*
 * cmd and req functions for the server
 */


// TODO: write new req and cmd methods to handle server client comm
// cmd calls should be executed server side, providing requested info back to
// the client
// req call should be executed client side, providing necessary info to the
// server

// cmd: client downloads file from server
void cmd_download(sync_info *si_client) {
    int stat_comm;
    char path[BUF_LEN];

    // recv requested file path from client
    stat_comm = recv_msg(si_client->sock_fd, path, BUF_LEN, BUF_LEN);

    // send the file to the client
    stat_comm = send_file(si_client->sock_fd, path);

    return resp_send(si_client->sock_fd, RESP_SUCCESS);
}

// cmd: client uploads file to server
void cmd_upload(sync_info *si_client) {
    int stat_comm;
    char path[BUF_LEN];

    // recv uploading file path from client
    stat_comm = recv_msg(si_client->sock_fd, path, BUF_LEN, BUF_LEN);

    // recv the file from the client
    stat_comm = recv_file(si_client->sock_fd, path);

    return resp_send(si_client->sock_fd, RESP_SUCCESS);
}

void cmd_changelog(sync_info *si_client) {
    int stat_comm;
    char path[BUF_LEN];
    
    // determine the client's respective changelog path based 
    // on their id
    strcpy(path, DP_CHANGELOG);
    strcat(path, "/");
    strncat(path, si_client->id, sizeof(uuid_t));

    // TODO: check if file exists first
    // send the file to the client
    stat_comm = send_file(si_client->sock_fd, path);

    return resp_send(si_client->sock_fd, RESP_SUCCESS);
}

// cmd create id for client
void cmd_create_id(sync_info *si_client) {
    int stat_comm;
    uuid_t id_client;
    uuid_generate(id_client);

    stat_comm = send_msg(si_client->sock_fd, id_client, 
        sizeof(uuid_t), sizeof(uuid_t)); 

    return resp_send(sock_fd, RESP_SUCCESS);
}

// cmd commence sync process for client

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
int req_upload(int sock_fd, char *path) {
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

int req_download(int sock_fd, char *path) {
    int stat_comm;
    int len_path;

    // prompt server with request
    stat_comm = prompt_req(sock_fd, CMD_C_DOWNLOAD);

    // send the server the file to upload
    len_path = strlen(path); 
    stat_comm = send_msg(sock_fd, path, len_path, len_path);

    // send the file
    stat_comm = send_file(sock_fd, path);
    
    return resp_await(sock_fd);
}

int req_delete(int sock_fd, char *path) {
    int stat_comm;
    int len_path;

    // prompt server with request
    stat_comm = prompt_req(sock_fd, CMD_C_DELETE);

    // send the server the file to delete
    len_path = strlen(path); 
    stat_comm = send_msg(sock_fd, path, len_path, len_path);

    return resp_await(sock_fd);
}

// req obtain client id
int req_sync_info(int sock_fd, sync_info *info) {
    int stat_comm;

    // prompt server with request
    stat_comm = prompt_req(sock_fd, CMD_C_SYNC_INFO);

    // obtain id from client
    status_comm = recv_msg(sock_fd, info,
            sizeof(sync_info), sizeof(sync_info));

    return resp_await(sock_fd);
}

// req obtain client's changelog
int req_changelog(int sock_fd, char *path) {
    int stat_comm;

    // prompt server with request
    stat_comm = prompt_req(sock_fd, CMD_C_CHANGELOG);

    // receive the updated changelog file from the client
    stat_comm = recv_file(sock_fd, path);

    return resp_await(sock_fd);
}
