#include "server_handlers.h"

// list of all callable server commands
void (*(commands[])(int)) = {
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
    
    //*(commands[req])(sock_fd);

    char cmd[MSG_LEN];
    char *msg_p = strchr(msg, ':');

    if (msg_p == NULL) {
        printf("Command has incorrect format\n\n");
        return -1;
    }
    
    // copy the contents of the command into its own buffer
    strncpy(cmd, msg, msg_p - msg);
    cmd[msg_p - msg] = '\0';

    // req_p now points to the arg
    msg_p++;
    
    // determine which function to call
    if (strncmp(cmd, RETRIEVE, MSG_LEN) == 0) {
        cmd_retrieve(sock_fd, msg_p);
    }
    else if (strncmp(cmd, CMD_RECEIVE, MSG_LEN) == 0) {
        // receive data from the client
        cmd_receive(sock_fd, msg_p);
    }
    else if (strncmp(cmd, CMD_INITIALIZE, MSG_LEN) == 0) {
        cmd_createid(sock_fd);
        // send all the files to get the client up to speed
    }
    else if (strncmp(cmd, CMD_SYNC, MSG_LEN) == 0) {
        cmd_sync(sock_fd, msg_p); 
    }
    // other commands to be implemented
    return 0;
}

// command functions
int cmd_retrieve(int sock_fd, char *args) {
    int status_send_file = send_file(sock_fd, args);
    printf("cmd_retrieve: finished sending file\n\n");
    return status_send_file;
}

int cmd_receive(int sock_fd, char *args) {
    int status_recv_file = recv_file(sock_fd, args);
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
    int i;
    // tmp is the new changelog received from the client
    // cur is the server on disk changelog yet to be updated
    char path_cur[MSG_LEN];
    char path_tmp[MSG_LEN];

    int status_comm;

    sync_info info_client;
    sync_file_update *sfu_s;
    sync_file_update *sfu_c;
    list *changelog_cur;
    list *changelog_tmp;

    // receive the client's info
    status_comm = recv_msg(sock_fd, info_client.id, 
            sizeof(info_client.id), sizeof(info_client.id));

    // create the file paths to be used
    memset(path_tmp, 0, MSG_LEN);
    memset(path_cur, 0, MSG_LEN);

    // create the path to the cur file from the id
    strcpy(path_cur, CHANGELOG_DIR);
    strcat(path_cur, "/");
    strncat(path_cur, (char *) id_client, sizeof(id_client));

    // create the path to the tmp file
    strcpy(path_tmp, path_cur);
    strcat(path_tmp, CHANGELOG_TMP);
    
    // receive the updated changelog file from the client
    status_comm = recv_file(sock_fd, path_tmp);

    // read in the server's changelog
    changelog_cur = load_changelog(path_cur);
    // read in client's changelog
    changelog_tmp = load_changelog(path_tmp);

    // loop through every item in changelog_tmp
    // if item modified
    //   if item in server cl, change client's item to copy and retrieve it
    //   if item not in server cl then copy over as normal
    // else item is deleted
    //   if item in server cl then do nothing
    //   else item not in server cl so delete server item
    for (i = 0; i < changelog_tmp->size; i++) {
        sfu_c = (sync_file_update *) list_get(changelog_tmp, i);
        sfu_s = (sync_file_update *) list_find(changelog_cur, sfu_c);
        
        // if client's item was deleted
        if (sfu_c->del) {
            // if server's cl contains item
            if (sfu_s) {
                // tell client to rename file and copy it over
            }
            else {
                // copy over file

            }
        }
        // else it was modified
        else {

        }
    }
}


