#include "tracker.h"

int initialize_client_id(int server_fd) {
    char msg[MSG_LEN];
    uuid_t id_client;
    sync_info client_info;

    // create a string for the createid request
    snprintf(msg, MSG_LEN, 
            "createid:");

    // send the request
    send_msg(server_fd, msg, MSG_LEN, strlen(msg));

    // recv the uuid
    recv_msg(server_fd, msg, MSG_LEN);
    
    // copy over the contents of the msg to the uuid_t
    memcpy(client_info.id, msg, sizeof(client_info.id));

    write_sync_info(&client_info);
}

int write_sync_info(sync_info *info) {
    FILE *fp;
    
    // write the sync_info struct to disk
    fp = fopen(FP_SYNC_INFO, "w");
    fwrite(info, sizeof(info), 1, fp);

    fclose(fp);
    return 0;
}

int read_sync_info(sync_info *info) {
    FILE *fp;

    fp = fopen(FP_SYNC_INFO, "r");
    fread(info, sizeof(info), 1, fp);
    
    fclose(fp);

    return 0;
}


// in the future store filenames as hashes instead of the file name itself
// TODO: error handling for the memory allocs
// remember to free this list too
list *load_changelog(char *path) {
    // gen vars
    int i;

    // changelog vars 
    list *changelog; // array of sync_file_update *
    sync_file_update * sfu_new;

    // file vars
    FILE *fp;
    char buf_file[BUF_LEN];
    int br = 0; // bytes read
    int lo = 0; // leftover bytes
    
    // create dynamically expanding list to hold paths
    changelog = list_init(LIST_INIT_LEN, &data_free_sync_file_update);

    // get the filestream for the changelog 
    fp = fopen(path, "r");
    
    
    // loop through the file char by char, dynamically allocating space for each
    // newline delimited string 
    i = 0;
    while (1) {
        memset(buf_file, 0, BUF_LEN);
        null_reached = 0;
        
        fread(buf_file, 1, BUF_LEN, fp);

        // loop through all read in characters from the file
        // the loop will exit when the end of the buffer is reached or when
        // there are at least two null terminators found in a row, signaling EOF
        for (i = 0; i < BUF_LEN; i++) {
            if (buf_file[i] == '\0') {
                // end of file is reached since two nulls are in a row
                if (null_reached >= 1) {
                    null_reached += 1;
                    break;
                }
                // else the end of this path is reached
                else {
                    // calculate the size of the path
                    path_len = strlen(buf_path);

                    su_new = (*sfu_new) malloc(sizeof(sync_update));

                    // allocate memory for path plus null terminator
                    su_new->path = (char *) calloc(path_len + 1, sizeof(char));
                    su_new->

                    
                    // append the pointer to the new path into the changelog
                    list_append(changelog, path_new);

                    // reset the buffer to start reading in a new file
                    memset(buf_path, 0, MSG_LEN);
                    

                    // set null_reached
                    null_reached += 1;
                    continue;
                }
            }

            // copy char by char the path name held in buf_file to buf_path
            if (strlen(buf_path) < BUF_LEN - 1) {
                // pass in the pointer to the ith byte of buf_file, not the
                // dereferenced value at i, hence buf_file + i
                // only copy 1 byte
                strncat(buf_path, buf_file + i, 1);
            }
            
            // reset null_reached
            null_reached = 0;
        }

        // EOF has been reached since there are multiple null
        // terminators in a row
        if (null_reached >= 2) {
            break;
        }
    }

    fclose(fp);

    return changelog;
}

// writes a changelog to file
int save_changelog(char *path, list *changelog) {
    int i;
    FILE *fp;

    fp = fopen(path, "w"); 

    // loop through all elements in changlog and write each to the file
    for (i = 0; i < changelog->size; i++) {
        // write a sync_file_update to file
        fwrite(list_get(changelog, i), sizeof(sync_file_update), 1, fp);
    }

    fclose(fp);

    return 0;
}

ht_file *load_inode_res(char *path) {
    FILE *fp;
    uuid_t id;
    unsigned int inode;
    ht_file *ht = ht_file_init(8, HT_THRESH);
    //ht_file *ht = ht_file_init(HT_CAP_INIT, HT_THRESH);

    fp = fopen(path, "r");

    // fill hashtable with values
    while (1) {
        fread(id, sizeof(uuid_t), 1, fp);
        if (feof(fp)) {
            break;
        }
        fread(&inode, sizeof(unsigned int), 1, fp);
        
        ht_file_insert(ht, id, inode);
    }

    fclose(fp);

    return ht;
}

int save_inode_res(ht_file *ht, char *path) {
    int i;
    FILE *fp;
    ht_node *node_curr;

    fp = fopen(path, "w");

    for (i = 0; i < ht->cap; i++) {
        node_curr = ht->list[i];
        while (node_curr) {
            fwrite(node_curr->key, sizeof(uuid_t), 1, fp);
            fwrite(&(node_curr)->val, sizeof(unsigned int), 1, fp);
            node_curr = node_curr->next;
        }
    }

    fclose(fp);

    return 0;
}
