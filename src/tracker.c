// returns a randomly generated uuid, which is an array of 16 chars
uuid_t idgen() {
    uuid_t uuid;
    uuid_generate(uuid);
    return uuid;
}

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

char *(* load_changelog(char *path))[] {
    // array of strings
    int i;
    char *(*changelog)[];
    FILE *fp_changelog;
    struct stat st;
    off_t fsize;
    int result;
    char c;
    char buf_path[MSG_LEN] = {0};
    int psize;

    // get the filestream for the changelog 
    fp_changelog = fopen(path, "r");
    
    // loop through the file char by char, dynamically allocating space for each
    // newline delimited string 
    i = 0;
    while (1) {
        result = fgetc(fp_changelog);

        if ((char) result == '\n' || c == EOF) {
            // calculate the size of the path
            psize = strlen(buf_path);

            // allocate memory for path plus null terminator
            changelog[i] = (char *) malloc(sizeof(char) * psize + 1);
            
            // copy the contents of buf_path to the allocated memory
            strncpy(changelog[i], buf_path, psize);
            changelog[i][psize] = '\0';
            i++;

            if (c == EOF) {
                break;
            }
        }
        else {
            c = (char) result;
            strcat(buf_path, &c);
        }
    }

    return changelog;
}
