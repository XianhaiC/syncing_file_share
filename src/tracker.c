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
char **load_changelog(char *path) {
    // gen vars
    int i;

    // changelog vars 
    char **changelog; // array of char *
    int size_changelog;
    int num_elements = 0; // tracks the current number of items

    // file vars
    FILE *fp_changelog;
    struct stat st;
    off_t fsize;
    char buf_file[BUF_LEN];
    
    int result;
    char c;
    char buf_path[BUF_LEN] = {0};
    int path_len;
    int null_reached; // keeps track of when the file ends

    size_changelog = CHANGELOG_SIZE;
    changelog = calloc(size_changelog, sizeof(char *));

    // get the filestream for the changelog 
    fp_changelog = fopen(path, "r");
    
    memset(buf_path, 0, BUF_LEN);
    
    // loop through the file char by char, dynamically allocating space for each
    // newline delimited string 
    i = 0;
    while (1) {
        memset(buf_file, 0, BUF_LEN);
        null_reached = 0;
        
        fread(buf_file, 1, BUF_LEN, fp_changelog);

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
                    // allocate space for changelog to hold the new pointer
                    if (num_elements >= size_changelog - 1) {
                        // double the array size
                        size_changelog *= 2;
                        changelog = realloc(changelog, sizeof(char *) * size_changelog);
                    }

                    // calculate the size of the path
                    path_len = strlen(buf_path);

                    // allocate memory for path plus null terminator
                    changelog[num_elements] = (char *) calloc(path_len + 1, sizeof(char));

                    // copy the contents of buf_path to the allocated memory
                    strncpy(changelog[num_elements], buf_path, path_len);

                    // reset the buffer to start reading in a new file
                    memset(buf_path, 0, MSG_LEN);
                    
                    // increment the number of elements in changelog
                    num_elements++;

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

    fclose(fp_changelog);

    return changelog;
}

int insert_file(char **changelog, char *file) {
    
}
