#ifndef TRACKER_H
#define TRACKER_H

#include <uuid/uuid.h>
#include <openssl/sha.h>

#include "macros.h"
#include "config.h"
#include "utils.h"
#include "comm.h"
#include "list.h"
#include "ht_file.h"

// forward declaration for externally defined structs
typedef struct _list list;

typedef struct _sync_file_update sync_file_update;


// hash contains the SHA1 hash of the file path
// del if 1 means the file is deleted, othewise it was modified
struct _sync_file_update {
    unsigned char hash[SHA_DIGEST_LENGTH]; 
    int del;
};

// hash contains the SHA1 hash of the file path
// del if 1 means the file is deleted, othewise it was modified
struct _sync_update {
    char *path; 
    int del;
};
int initialize_client_id(int server_fd);
int write_sync_info(sync_info *info);
int read_sync_info(sync_info *info);
list *load_changelog(char *path);
int save_changelog(char *path, list *changelog);
ht_file *load_inode_res(char *path);
int save_inode_res(ht_file *ht, char *path);

#endif
