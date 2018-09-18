#include "comm.h"

// sends response back to server
// msg_buf must be null terminated
int send_msg(int sock_fd, char *msg, int msg_len) {
    // bytes communicated
    int bc = 0;
    int left = msg_len;

    //write the contents of msg_buf into socket s and return status
    do {
        if ((bc = send(sock_fd, msg, msg_len, 0)) == -1) {
            perror("send");
        }
        else {
            msg += bc;
            left -= bc;
        }
    }
    while (left > 0);

    return bc;
}

int recv_msg(int sock_fd, char *msg, int msg_len) {
    // bytes communicated
    int bc;
    int left = msg_len;

    //reset the msg buffer
    memset(msg, 0, msg_len); 
    
    do {
        if ((bc = recv(socket_fd, msg, msg_len, 0)) <= 0) {
            perror("send");
            return -1;
        }
        else {
            msg += bc;
            left -= bc;
        }
    
    }
    while (left > 0);

    return 0;
}

int send_int(int sock_fd, long num) {
    // convert to a standard 32 bit int in network byte order
    int32_t conv = htonl(num);
    char *msg = (char *) &conv;
    int left = sizeof(conv);
    int bc;
    
    // send until no more bytes left
    do {
        if ((bc = send(sock_fd, msg, left, 0)) <= 0) {
            
            // connection closed
            if (bc == 0) {
                printf("selectserver: socked %d hung up\n", sock_fd);
            }
            else {
                perror("recv");
            }

            return -1;
        }
        else {
            msg += bc;
            left -= bc;
        }
    }
    while (left > 0);

    return 0;
}

int recv_int(int sock_fd, long *num)
{
    int32_t ret;
    char *msg = (char*) &ret;
    int left = sizeof(ret);
    int bc;

    do {
        if ((bc = read(fd, data, left)) <= 0) {

            if (bc == 0) {
                printf("selectserver: socked %d hung up\n", sock_fd);
            }
            else {
                perror("recv");
            }
        }
        else {
            msg += bc;
            left -= bc;
        }
    }
    while (left > 0);

    *num = ntohl(ret);
    return 0;
}
