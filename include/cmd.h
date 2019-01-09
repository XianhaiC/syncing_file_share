#ifndef CMD_H
#define CMD_H

#include <uuid/uuid.h>

#include "macros.h"
#include "sync.h"
#include "comm.h"

void parsex(int req, sync_info *si_client);
void cmd_login(sync_info *si_client);
void cmd_download(sync_info *si_client);
void cmd_upload(sync_info *si_client);
void cmd_delete(sync_info *si_client);
void cmd_changelog(sync_info *si_client);
void cmd_sync_info(sync_info *si_client);
void cmd_create_id(sync_info *si_client);
int req_login(int sock_fd, sync_info *si_client, int action);
int req_download(int sock_fd, char *path);
int req_upload(int sock_fd, char *path);
int req_delete(int sock_fd, char *path);
int req_changelog(int sock_fd, char *path);
int req_sync_info(int sock_fd, sync_info *info);

// list of all callable server commands
void (*g_cmds[])(sync_info *) = {
    &cmd_login,
    &cmd_download,
    &cmd_upload,
    &cmd_delete,
    &cmd_changelog,
    &cmd_sync_info,
    &cmd_create_id
};

#endif
