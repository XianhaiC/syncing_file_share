
void sync_s_sync_client(int sock_fd) {
    int i;
    // tmp is the new changelog received from the client
    // cur is the server on disk changelog yet to be updated
    char path_curr[MSG_LEN];
    char path_tmp[MSG_LEN];

    int status_comm;

    sync_info info_client;
    sync_file_update *sfu_s;
    sync_file_update *sfu_c;

    tree_file *changelog_curr;
    tree_file *changelog_tmp;

    // receive the client's info
    status_comm = recv_msg(sock_fd, info_client.id, 
            sizeof(info_client.id), sizeof(info_client.id));

    // create the file paths to be used
    memset(path_tmp, 0, MSG_LEN);
    memset(path_curr, 0, MSG_LEN);

    // create the path to the cur file from the id
    strcpy(path_curr, CHANGELOG_DIR);
    strcat(path_curr, "/");
    strncat(path_curr, (char *) id_client, sizeof(id_client));

    // create the path to the tmp file
    strcpy(path_tmp, path_curr);
    strcat(path_tmp, CHANGELOG_TMP);
    
    // receive the updated changelog file from the client
    status_comm = recv_file(sock_fd, path_tmp);

    // read in the server's changelog
    changelog_cur = tf_load(path_tmp);
    // read in client's changelog
    changelog_tmp = tf_load(path_tmp);

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

void cmd_sync_h(tf_node *n_root, tree_file *changelog_curr,
        list *l_conf_serv, list *l_conf_client) {
    tf_node *n_query_curr;

    // we're only concerned with leaf nodes since those represent files
    if (n_root->children->size == 0) {
        // search for the corresponding node in changelog_curr
        n_query_curr = tf_find(changelog_curr, n_root->p_abs);    

        // if item in server cl
        if (n_query_curr != NULL) {
            // if item modified, add both nodes to list to be transfered
            // later
            if (!n_root->del) {
                // tell client to create a copy and return it
                list_append(l_conf_curr, n_query_curr);
                list_append(l_conf_tmp, n_root);
            }

            // copy file from tmp changelog over
        }
        else {
            // if item modified
            if (!n_root->del) {
                // download file from client
            }
            // else item was deleted, so delete it serverside too
            else {
                // delete file
            }
        }

        // no need to do anything further with children
        return;
    }

    // recurse on children
    for (i = 0; i < n_root->children->size; i++) {
        tf_save_h(list_get(n_root->children, i), fp);
    }
}
