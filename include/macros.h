#ifndef MACROS_H
#define MACROS_H

// general
#define NULL_FD -1
#define STDIN 0
#define MSG_LEN 10240 // at most 10kb
#define BUF_LEN 10240
#define STRCMP_LEN 1024
#define CON_REM_TM 20.0

// uuid
#define LEN_UUID_T 16

// list
#define LIST_INIT_LEN 128

// hashtable
#define HT_CAP_INIT 1024
#define HT_THRESH 0.75

// tree_file
#define TF_ROOT "/"
#define TF_SEP_STR "/"
#define TF_SEP_CHAR '/'

// util
#define DELAY_CONN 500000

// networking
#define LOCALHOST "127.0.0.1"
#define PORT_SERV 12347
#define PORT_LOG 12346
#define PORT_TEST 12345
#define BACKLOG 10
#define TIMEOUT_LOG 20.0

// server commands
#define CMD_S_RETRIEVE 0
#define CMD_S_RECEIVE 1
#define CMD_S_CREATEID 2
#define CMD_S_SYNC 3

// client commands
#define CMD_C_DOWNLOAD 0
#define CMD_C_UPLOAD 1
#define CMD_C_DELETE 2
#define CMD_C_DUPE 3
#define CMD_C_SYNC_INFO 4
#define CMD_C_CHANGELOG 5
#define CMD_C_KILL 6


// cmdreq util
#define RESP_SUCCESS 1

// directory files
#define CHANGELOG_SIZE 512
#define CHANGELOG_CLIENT "clog_cl"

#endif
