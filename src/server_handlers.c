#include "server_handlers.h"

// list of all callable server commands
void (*(g_commands[])(sync_info *)) = {
    &cmd_login,
    &cmd_receive,
    &cmd_retrieve,
    &cmd_createid,
    &cmd_sync
}

// parse and execute command
// client commands follow the following format:
// command:arg1,arg2,...
//
// The following commands are:
// retr_file:file_path
int parsex(int req, sync_info *si_client) {
    *(g_commands[req])(si_client);
}

void cmd_login(sync_info *si_client) {
    int stat_comm;
    int32_t action;

    // determine what the client wishes to do, storing info in action
    // 0: login an existing client
    // 1: create a new id for the client
    stat_comm = recv_int32(si_client->sock_fd, &action);

    switch (action) {
        case 0:
            stat_comm = recv_msg(si_client->sock_fd, si_client->id, sizeof(uuid_t));
            // TODO: error handling
        case 1:
            uuid_generate(si_client->id);

            // send id to client
            stat_comm = send_msg(si_client->sock_fd, si_client->id, 
                sizeof(uuid_t), sizeof(uuid_t));
            // TODO: error handling
        default:
            uuid_generate(si_client->id);

            // send id to client
            stat_comm = send_msg(si_client->sock_fd, si_client->id, 
                sizeof(uuid_t), sizeof(uuid_t));
            // TODO: error handling
    }

    return resp_send(si_client->sock_fd, RESP_SUCCESS);
}

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

void cmd_delete(sync_info *si_client) {
    int stat_comm;
    char path[BUF_LEN];

    stat_comm = recv_msg(si_client->sock_fd, path, BUF_LEN, BUF_LEN);

    remove(path);
    // TODO: error handling

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

void cmd_sync_info(sync_info *si_client) {
    int stat_comm;

    // send the sync info to the client
    stat_comm = send_msg(sock_fd, si_client,
            sizeof(sync_info), sizeof(sync_info));

    return resp_send(si_client->sock_fd, RESP_SUCCESS);
}

// cmd create id for client
void cmd_create_id(sync_info *si_client) {
    int stat_comm;
    uuid_t id_client;
    uuid_generate(id_client);

    // send id to client
    stat_comm = send_msg(si_client->sock_fd, id_client, 
        sizeof(uuid_t), sizeof(uuid_t)); 

    return resp_send(sock_fd, RESP_SUCCESS);
}


// cmd commence sync process for client

int req_login(int sock_fd, sync_info *si_client, int action) {
    int stat_comm;

    stat_comm = cmd_prompt(sock_fd, CMD_LOGIN);

    stat_comm = send_int32_t(sock_fd, &action);

    switch (action) {
        case 0:
            stat_comm = send_msg(sock_fd, si_client->id, 
                sizeof(uuid_t), sizeof(uuid_t));
        case 1:
            stat_comm = recv_msg(sock_fd, si_client->id,
                sizeof(uuid_t));
        default:
            stat_comm = recv_msg(sock_fd, si_client->id,
                sizeof(uuid_t));
    }
}

// req client to upload file to server
int req_upload(int sock_fd, char *path) {
    int stat_comm;
    int len_path;

    // prompt client with request
    stat_comm = cmd_prompt(sock_fd, CMD_UPLOAD);

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
    stat_comm = cmd_prompt(sock_fd, CMD_DOWNLOAD);

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
    stat_comm = cmd_prompt(sock_fd, CMD_DELETE);

    // send the server the file to delete
    len_path = strlen(path); 
    stat_comm = send_msg(sock_fd, path, len_path, len_path);

    return resp_await(sock_fd);
}

// req obtain client id
int req_sync_info(int sock_fd, sync_info *info) {
    int stat_comm;

    // prompt server with request
    stat_comm = cmd_prompt(sock_fd, CMD_SYNC_INFO);

    // obtain id from client
    status_comm = recv_msg(sock_fd, info,
            sizeof(sync_info));

    return resp_await(sock_fd);
}

// req obtain client's changelog
int req_changelog(int sock_fd, char *path) {
    int stat_comm;

    // prompt server with request
    stat_comm = cmd_prompt(sock_fd, CMD_CHANGELOG);

    // receive the updated changelog file from the client
    stat_comm = recv_file(sock_fd, path);

    return resp_await(sock_fd);
}

// parse and execute command
// client commands follow the following format:
// command:arg1,arg2,...
//
// The following commands are:
// retr_file:file_path
int parsex(int req, int sock_fd) {
    *(g_commands[req])(sock_fd);
}

/*
// command functions
int cmd_upload(int sock_fd, char *file) {
    int stat_comm;

    stat_comm = send_file(sock_fd, file);
    printf("cmd_retrieve: finished sending file\n\n");
    return stat_comm;
}

int cmd_download(int sock_fd) {
    char msg[MSG_LEN] = {0};
    int stat_comm;
    
    // recv file name
    stat_comm = revc_msg(sock_fd, msg, MSG_LEN);
    
    int status_recv_file = recv_file(sock_fd, msg);
    printf("cmd_receive: finished receiving file\n\n");
    return status_send_file;
}

int cmd_createid(int sock_fd) {
    int status_send_msg;
    uuid_t id_client;
    uuid_generate(id_client);
    status_send_msg = send_msg(sock_fd, id_client, sizeof(id_client), sizeof(id_client)); 
    printf("cmd_createid: finished sending id to client\n\n");
    return status_send_msg;
}

int cmd_sync(int sock_fd) {
    sync_s_sync_client(sock_fd);
}

int hl_upload(int sock_fd) {
    uuid_t inode_id;
    char msg[MSG_LEN] = {0};
    int stat_comm;
    
    // recv the file clients wishes to obtain
    stat_comm = recv_msg(sock_fd, msg, MSG_LEN, MSG_LEN);

    return cmd_upload(sock_fdm, msg);
}
*/
