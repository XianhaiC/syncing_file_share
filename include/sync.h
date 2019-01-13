#ifndef SYNC_H
#define SYNC_H

#include "config.h"
#include "sync_info.h"
#include "sync_util.h"
#include "tree_file.h"
#include "cmd.h"

void sync_synchronize(sync_info *si_client, int sock_fd);
void sync_changelogs(sync_info *si_client, int sock_fd,
    tree_file *ch_old, tree_file *ch_new);
void sync_old_r(sync_info *si_client, int sock_fd,
    tf_node *n_root, tree_file *ch_new, list *l_conf);
void sync_new_r(int sock_fd, tf_node *n_root);
int sync_dupe_upload(sync_info *si_client, int sock_fd, char *path_ori);

#endif
