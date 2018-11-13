
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

    list *l_conf_curr;
    list *l_conf_tmp;

    char *path_conf;
    char *tmp;

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

    // init lists to hold conflicted nodes
    l_conf_curr = list_init(LIST_INIT_LEN, &data_comp_tf_node);
    l_conf_tmp = list_init(LIST_INIT_LEN, &data_comp_tf_node);

    // init trees to hold changelog files
    changelog_curr = tf_load(path_curr);
    changelog_tmp = tf_load(path_tmp);

    cmd_sync_h(changelog_tmp->root, changelog_curr, 
            l_conf_curr, l_conf_tmp);

    // loop through all conflicted nodes and sync them
    for (i = 0; i < l_conf_curr->size; i++) {
        path_conf = ((tf_node *) list_get(l_conf_curr, i))->p_abs;
        
        // if the conflicted file is not a dup file, solve the conflict as per
        // usual
        tmp = strstr(list_get(l_conf_curr, i), DUP_EXT);
        if (tmp != NULL) {
            sc_conflict_res(sock_fd, path_conf, info_client.id);
        }
        // otherwise, since we don't wish to create dup files of dup files,
        // the server has priority and will overwrite changes client made to
        // local dup file
        else {
            // TODO: revamp communication methods
            // they should be server centered
            // this means that if a client ever needs anything, it must send the
            // server a request with all the necessary info
            // the server will then perform any other necessary things by
            // calling client side functions in preparation of recieveing
            // things
            // as such, the client no longer needs to premptively begin any comm
            // actions, and should wait for the server to initiate them. 
            // basically, the client requests the server to do something, and
            // then the server will take it from there
            // TODO: Create reqs reqc cmds cmdc functions to handle dedicated
            // server/client request commands
            stat_comm = reqc_confres(sock_fd, path_conf);
        }

    }
}

void cmd_sync_h(tf_node *n_root, tree_file *changelog_curr,
        list *l_conf_curr, list *l_conf_tmp) {
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
                // if both nodes are edited, then save them into list for later
                // syncing 
                if (!n_query_curr->del) {
                    list_append(l_conf_curr, n_query_curr);
                    list_append(l_conf_tmp, n_root);
                }
                
                // copy tmp over    
            }

            // do nothing
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
