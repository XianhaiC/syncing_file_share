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
int cmd_upload(int sock_fd) {
    uuid_t inode_id;
    int stat_comm;

    stat_comm = recv_msg(sock_fd, inode_id, sizeof(uuid_t), sizeof(uuid_t));
    if (stat_comm <= 0) {
        // TODO: error
        return -1;
    }

    int status_send_file = send_file(sock_fd, args);
    printf("cmd_retrieve: finished sending file\n\n");
    return status_send_file;
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

/* traverse through every leaf node in changelog_tmp
 *
 * if item in server cl
 *   if modified, change client's item to copy and retrieve it
 *   else del, so do nothing
 * else item not in server cl
 *   if modified, copy over as normal
 *   else del, delete from server
 */
