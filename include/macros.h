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

// hash map
#define HM_CAP_INIT 1024
#define HM_THRESH 0.75

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

// server-client commands
#define CMD_DOWNLOAD 0
#define CMD_UPLOAD 1
#define CMD_DELETE 2
#define CMD_SYNC_INFO 3
#define CMD_CHANGELOG 4
#define CMD_KILL 5
#define CMD_LOGIN 6


// cmdreq util
#define RESP_SUCCESS 1

// directory files
#define CHANGELOG_SIZE 512
#define CHANGELOG_CLIENT "clog_cl"

#endif
