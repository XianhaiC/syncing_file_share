#include "tracker.h"

int initialize_client_id(int server_fd) {
    char msg[MSG_LEN];
    uuid_t id_client;

    // create a string for the createid request
    snprintf(msg, MSG_LEN, 
            "createid:");

    // send the request
    send_msg(server_fd, msg, MSG_LEN, strlen(msg));

    // recv the uuid
    recv_msg(server_fd, msg, MSG_LEN);
    
    // copy over the contents of the msg to the uuid_t
    memcpy(id_client, msg, sizeof(id_client));
}

// in the future store filenames as hashes instead of the file name itself
// TODO: error handling for the memory allocs
// remember to free this list too
list *load_changelog(char *path) {
    // gen vars
    int i;

    // changelog vars 
    list *changelog; // array of char *

    // file vars
    FILE *fp;
    struct stat st;
    off_t fsize;
    char buf_file[BUF_LEN];
    
    // path processing vars
    char buf_path[BUF_LEN] = {0};
    char *path_new;
    int path_len;
    int null_reached; // keeps track of when the file ends

    changelog = (list *) calloc(1, sizeof(list));
    list_init(changelog, &data_free_string);

    // get the filestream for the changelog 
    fp = fopen(path, "r");
    
    memset(buf_path, 0, BUF_LEN);
    
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

                    // allocate memory for path plus null terminator
                    path_new = (char *) calloc(path_len + 1, sizeof(char));

                    // copy the contents of buf_path to the allocated memory
                    strncpy(path_new, buf_path, path_len);
                    
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
        // the strlen relies on the contents of changelog to be null terminated
        fwrite(changelog->data[i], sizeof(char), strlen(changelog->data[i]), fp);
        fputc('\0', fp);
    }

    fclose(fp);

    return 0;
}
