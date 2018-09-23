
uuid_t idgen() {
    uuid_t uuid;
    uuid_generate(uuid);
    return uuid;
}

int initialize_client_id(int server_fd) {
    char msg[MSG_LEN];
    
    snprintf(msg, MSG_LEN, 
            "create_id:");
}
