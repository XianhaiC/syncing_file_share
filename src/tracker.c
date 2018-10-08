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
    
    // create dynamically expanding list to hold hashes
    changelog = (list *) calloc(1, sizeof(list));
    list_init(changelog, &data_free_sync_file_update);

    // get the filestream for the changelog 
    fp = fopen(path, "r");
    
    
    // loop through the file char by char, dynamically allocating space for each
    // newline delimited string 
    i = 0;
    while (1) {
        memset(buf_file, 0, BUF_LEN);
        
        // read in hash bytes from file and store in buf_file at leftover offset
        br = fread(buf_file + lo, 1, BUF_LEN - lo, fp);

        // loop through all the hash bytes in the file, buffering them in
        // buckets. 
        for (i = 0; i < br; i += SHA_DIGEST_LENGTH) {
            // if there are not enough bytes to form a hash, as we are at the
            // end of the bytes read into buf_file, move the leftover string 
            // to the beggining of buf_file and continue to read more bytes 
            // from the file from there
            if ((lo = br - i) < SHA_DIGEST_LENGTH) {
                strncpy(buf_file, buf_file + i, lo); 
                break;
            }

            // allocate memory for new sync_file_update struct
            sfu_new = (sync_file_update *) calloc(1, sizeof(sync_file_update));

            // copy the contents of buf_file to the struct's hash member
            strncpy(sfu_new->hash, buf_file + i, SHA_DIGEST_LENGTH);

            // append the pointer to the new struct to the changelog
            list_append(changelog, sfu_new);
        }

        // If EOF has been reached, we are done
        if (feof(fp)) {
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
        // the strlen relies on the contents of changelog to be null terminated
        fwrite(changelog->data[i], sizeof(char), strlen(changelog->data[i]), fp);
        fputc('\0', fp);
    }

    fclose(fp);

    return 0;
}
