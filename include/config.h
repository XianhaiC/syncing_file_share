#ifndef CONFIG_H 
#define CONFIG_H

#define SYNC_ROOT "/home/xianhai/projects/c/file_share_sync/test/syncroot_local"
#define SYNC_ROOT_REMOTE "/home/xianhai/projects/c/file_share_sync/test/syncroot_rem"

// .sync macros
#define DP_CHANGELOG SYNC_ROOT "/.sync/changelogs"
#define FP_SYNC_INFO SYNC_ROOT "/.sync/sync_info"
#define SYNC_IP "127.0.0.1"
#define TEST_FILE "/test_file2.txt"
#define CHANGELOG_TMP "_tmp"

// file permissions
#define PERM_CHANGELOG 0755

#endif
