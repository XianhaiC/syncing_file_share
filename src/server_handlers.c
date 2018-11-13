#include "server_handlers.h"

// list of all callable server commands
void (*(g_commands[])(int)) = {
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
int parsex(int req, int sock_fd) {
    *(g_commands[req])(sock_fd);
}

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
