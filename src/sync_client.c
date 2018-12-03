#include "sync_client.h"

// handles client side conflict resolution
// creates a duplicate of file specified by path_ori
// overwrites existing dupes with identical path_ori
// and editor client
sc_conflict_res(sync_info *si_client, char *path_ori) {
    int status_send_file;
    char path_dup[BUF_LEN];

    // create path_dup
    su_get_path_dup(path_ori, path_dup, id);

    // create the dup file
    sc_file_dup(path_ori, path_dup, id);

    // TODO: we want the server to copy over this file. So, we must inform the
    // server to call its hl_download. To do this, create dedicated request
    // functions that communicate with the server this way
    // Remember that the server and client will split off individual threads
    // upon requests. This way they can take more requests while others are
    // running
    stat_comm = req_download(sock_fd, path_dup);
}

// duplicates a given file, giving it file path:
// <path_ori>.dup~<id>
void sc_file_dup(char *path_ori, char *path_dup, uuid_t id) {
    file *fp_ori;
    file *fp_dup;
    char buf[buf_len];
    unsigned int br;

    // check that original file exists
    if (access(path_ori, f_ok) == -1) {
        return;
    }
    
    // open relevant files
    fp_ori = fopen(path_ori, "r");
    fp_dup = fopen(path_dup, "w"); 

    while (1) {
        // read contents into buf
        br = fread(buf, sizeof(char), buf_len, fp_ori);
        // write contents to dupe file
        fwrite(buf, sizeof(char), br, fp_dup);
        
        // if eof has been reached, we are done
        if (feof(fp)) {
            break;
        }
    }

    return;
}

void sync_synchronize(sync_info *si_client, int sock_fd) {
    int i;
    // tmp is the new changelog received from the client
    // cur is the server on disk changelog yet to be updated
    char path_old[MSG_LEN];
    char path_new[MSG_LEN];

    int resp;

    sync_info info_client;

    tree_file *ch_old;
    tree_file *ch_new;

    // req server for client's changelog
    // path_new is now contains the correct contents
    resp = cmd_get_changelog(si_client, path_new);
    
    // read in the old changelog
    ch_old = tf_load(path_old);
    // read in new changelog
    ch_new = tf_load(path_new);

    // init trees to hold changelog files
    ch_old = tf_load(path_old);
    ch_new = tf_load(path_new);

    // sync the changelogs
    sync_changelogs(si_client, ch_old, ch_new);
}

// copies over client's files given clientside changelog
// keeps track of conflicting files 
void sync_changelogs(sync_info *si_client, 
        tree_file *ch_old, tree_file *ch_new) {
    list *l_conf_old;
    list *l_conf_new;

    // init lists to hold conflicted nodes
    l_conf_old = list_init(LIST_INIT_LEN, &data_comp_tf_node);
    l_conf_new = list_init(LIST_INIT_LEN, &data_comp_tf_node);

    // traverse through old changelog first, tracking conflicted nodes and
    // uploading changes to server as needed
    sync_old_r(si_client, ch_old->root, ch_new, *l_conf_old, *l_conf_new);

    // resolve conflicts looping through all conflicted nodes and sync them
    for (i = 0; i < l_conf_old->size; i++) {
        path_conf = ((tf_node *) list_get(l_conf_old, i))->p_abs;
        
        // if the conflicted file is not a dup file, solve the conflict as per
        // usual
        tmp = strstr(list_get(l_conf_old, i), DUP_EXT);
        if (tmp == NULL) {
            // duplicate the file and upload it to the server
            sync_dupe_upload(si_client, sock_fd, path_conf, info_client);
        }

        // otherwise, since we don't wish to create dup files of dup files,
        // overwrite client's file with server's this will be done when we
        // download all of server's files
        // thus we don't need to do anything more here
    }

    // traverse through new changelog, downloading from server as needed
    sync_new_r(si_client, sock_fd, ch_new->root);
}

void sync_old_r(sync_info *si_client, 
        tf_node *n_root, tree_file *ch_new, list *l_conf) {
    int resp;
    tf_node *n_query_curr;

    // we're only concerned with leaf nodes since those represent files
    if (n_root->children->size == 0) {
        // search for the corresponding node in ch_old
        n_query_curr = tf_find(ch_new, n_root->p_abs);    

        // if item in server cl
        if (n_query_curr != NULL) {
            // if item modified, add both nodes to list to be transfered
            // later
            if (!n_root->del) {
                // if both nodes are edited, then save node for later reference
                if (!n_query_curr->del) {
                    list_append(l_conf, n_query_curr);
                }
                else {
                    // convert our file to a dup file and upload it to the
                    // server
                    resp = sync_dupe_upload(si_client, n_root->p_abs);

                    // TODO: delete our original file
                }
            }
            
            // do nothing otherwise
            // the client will recieve this file later when we copy over
            // ch_new's remaining files
        }
        else {
            // if item modified
            if (!n_root->del) {
                // upload the file to the server
                resp = cmd_upload(si_client, n_root->p_abs);
            }
            // else delete it from the server
            else {
                if (remove(n_root->abs)) {
                    // TODO: error handling
                }
            }
        }

        // no need to do anything further with children
        return;
    }

    // recurse on children
    for (i = 0; i < n_root->children->size; i++) {
        sync_old_r(si_client, list_get(n_root->children, i), ch_old, l_conf);
    }
}

// traverse through server's changelog and copy over modified files, else delete
// them clientside
void sync_new_r(sync_info *si_client, tf_node *n_root) {
    int resp;

    // we only about root nodes, or files
    if (n_root->children->size == 0) {
        // delete client's respective file
        if (n_root->del) {
            // TODO: delete the file clientside
            // TODO: handle resp
        }
        // download the file from the server
        else {
            resp = cmd_download(si_client, n_root->p_abs);
        }

        return;
    }

    // recurse on children
    for (i = 0; i < n_root->children->size; i++) {
        sync_new_r(list_get(n_root->children, i), changelog_curr, l_conf_curr, l_conf_tmp);
    }
}

int sync_dupe_upload(sync_file *si_client, int sock_fd, char *path_ori) {
    size_t br;
    int resp;
    char path_dup[MSG_LEN];

    // get the name of the dupe file
    su_get_dup_path(path_ori, path_dup, si_client->id);

    // open relevant files
    fp_ori = fopen(path_ori, "r");
    fp_dup = fopen(path_dup, "w"); 

    while (1) {
        // read contents into buf
        br = fread(buf, sizeof(char), buf_len, fp_ori);

        if (ferror(fp)) {
            // TODO: error handle
        }

        // write contents to dupe file
        fwrite(buf, sizeof(char), br, fp_dup);
        
        // if eof has been reached, we are done
        if (feof(fp)) {
            break;
        }
    }

    // upload the duped file to server
    resp = cmd_upload(si_client, path_dup);

    return resp;
}
