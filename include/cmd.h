#ifndef CMD_H
#define CMD_H

#include <uuid/uuid.h>

#include "macros.h"
#include "config.h"
#include "sync.h"
#include "comm.h"
#include "hash_map.h"
#include "cmd_util.h"

void parsex(int req, hash_map *hm_sync_info, int sock_fd);
void cmd_login(hash_map *hm_sync_info, int sock_fd);
void cmd_download(hash_map *hm_sync_info, int sock_fd);
void cmd_upload(hash_map *hm_sync_info, int sock_fd);
void cmd_delete(hash_map *hm_sync_info, int sock_fd);
void cmd_changelog(hash_map *hm_sync_info, int sock_fd);
void cmd_sync_info(hash_map *hm_sync_info, int sock_fd);
int req_login(int sock_fd, sync_info *si_client, int action);
int req_download(int sock_fd, char *path);
int req_upload(int sock_fd, char *path);
int req_delete(int sock_fd, char *path);
int req_changelog(int sock_fd, char *path);
int req_sync_info(int sock_fd, sync_info *info);

// list of all callable server commands
void (*g_cmds[])(hash_map *, int) = {
    &cmd_login,
    &cmd_download,
    &cmd_upload,
    &cmd_delete,
    &cmd_changelog,
    &cmd_sync_info,
};

#endif
