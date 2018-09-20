#ifndef MACROS_H
#define MACROS_H

// general
#define NULL_FD -1
#define STDIN 0
#define MSG_LEN 10240 // at most 10kb
#define CON_REM_TM 20.0

// util
#define DELAY_CONN 500000

// networking
#define LOCALHOST "127.0.0.1"
#define PORT_SERV 12347
#define PORT_LOG 12346
#define BACKLOG 10
#define TIMEOUT_LOG 20.0

// server commands
#define RETRIEVE "retrieve"

#endif
