// TODO: finish this shit
typedef struct _sync_info sync_info;
struct _sync_info {
    uuid_t id;
};

// reads in sync_info from disk
int sync_info_read(sync_info *info, char *path);
