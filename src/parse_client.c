// handles incoming command parsing

int parser_c(int sock_fd, sync_info *info) {
    int stat_comm;
    int32_t cmd;

    while (1) {
        stat_comm = recv_int32_t(sock_fd, &cmd);

        // TODO: write the handler methods for each client request
        switch(cmd) {
            case CMD_C_DOWNLOAD:
                cmdc_download(sock_fd);
                break;
            case CMD_C_UPLOAD:
                cmdc_upload(sock_fd);
                break;
            case CMD_C_DELETE:
                cmdc_delete(sock_fd);
                break;
            case CMD_C_DUPE:
                cmdc_dupe(sock_fd, info->id);
                break;
            case CMD_C_SYNC_INFO:
                cmdc_sync_info(sock_fd, info);
                break;
            case CMD_C_CHANGELOG:
                cmdc_changelog(sock_fd);
                break;
            default:
                break;
        }
    }
}
