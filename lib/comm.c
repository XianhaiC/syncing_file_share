#include "comm.h"

/*
 * sends a message of size msg_len
 * msg_len - the size of the buffer
 * to_send - the amount of bytes you wish to send
 */
int send_msg(int sock_fd, char *msg, int msg_len, int to_send) {
    // bytes communicated
    int bc = 0;

    if (to_send > msg_len) {
        printf("send_msg: to_send is larger than buffer");
        return -1;
    }

    // send recipient bytes of msg to recv
    send_int(sock_fd, to_send);

    // loop until all bytes in buffer are sent
    do {
        if ((bc = send(sock_fd, msg, to_send, 0)) == -1) {
            perror("send");
            return -1;
        }
        else {
            msg += bc;
            to_send -= bc;
        }
    }
    while (to_send > 0);

    return 0;
}

/*
 * recieves a message of size msg_len
 * msg_len - the size of the msg buffer
 */
int recv_msg(int sock_fd, char *msg, int msg_len) {
    // bytes communicated
    int bc;
    long to_recv;

    // recv bytes to read
    recv_int(sock_fd, &to_recv);

    if (to_recv > msg_len) {
        // push to_recv back into the socket and return
        printf("recv_msg: buffer not large enough for incoming msg\n\n");
        return -1;
    }

    //reset the msg buffer
    memset(msg, 0, msg_len); 
    
    do {
        if ((bc = recv(sock_fd, msg, to_recv, 0)) == -1) {
            perror("send");
            return -1;
        }
        else {
            msg += bc;
            to_recv -= bc;
        }
    }
    while (to_recv > 0);

    return 0;
}

/*
 *  sends a 32 bit int
 */
int send_int(int sock_fd, long num) {
    // convert to standard 32 bit int in network byte order
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

/*
 *  recieves a 32 bit int
 */
int recv_int(int sock_fd, long *num)
{
    int32_t ret;
    char *msg = (char*) &ret;
    int left = sizeof(ret);
    int bc;

    do {
        if ((bc = read(sock_fd, msg, left)) <= 0) {

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

/*
 * sends the file at path to sock_fd
 */
int send_file(int sock_fd, char *path) {
    int i;
    char msg[MSG_LEN];
    char *msg_p;
    FILE *fp;
    long fsize;
    int to_send;
    int bc;
    int eof_reached = 0;

    // open filestream of requested file
    fp = fopen(path, "r");
    if (fp == NULL) {
        perror("fopen");
        return -1;
    }

    // first determine the total number of bytes to transfer
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    rewind(fp);

    // send recipient file size
    send_int(sock_fd, fsize);
    
    // begin sending the file
    while (fsize > 0) {
        // i is used to track how many chars have been buffered
        for (i = 0; i < MSG_LEN; i++) {
            // get the next char
            msg[i] = fgetc(fp);

            // stop buffering if we've reached EOF
            if (msg[i] == EOF) {
                eof_reached = 1;
                break;
            }
        }

        // if i <= 0 then no bytes have been buffered, so don't send anything
        if (i > 0) {
            // determine how many bytes to send for this buffer
            to_send = i;
            msg_p = msg;

            // loop until all bytes in buffer are sent
            do {
                if ((bc = send(sock_fd, msg_p, to_send, 0)) == -1) {
                    perror("send");
                    printf("Unable to send requested file: %s\n\n", path);
                    fclose(fp);
                    return -1;
                }
                else {
                    msg_p += bc;
                    to_send -= bc;
                }
            }
            while (to_send > 0);

            // decrement the amount of bytes left to send by the initial value of
            // to_send before we modified it
            fsize -= i;
        }

        // break since we have reached the EOF
        if (eof_reached) {
            break;
        }
    }

    fclose(fp);
    return 0;
}

/*
 *  recieves file from sock_fd and writes it to path
 */
int recv_file(int sock_fd, char *path) {
    int i;
    char msg[MSG_LEN];
    FILE *fp;
    long fsize;
    int to_recv;
    int bc;
    
    // open filestream to write into
    fp = fopen(path, "w");
    if (fp == NULL) {
        perror("fopen");
        return -1;
    }

    // recv file size
    recv_int(sock_fd, &fsize);

    // continue to recieve file bytes until specified amount is reached
    while (fsize > 0) {
        // determine how many bytes to recv for this buffer
        to_recv = min(fsize, MSG_LEN);

        // recieve the bytes
        if ((to_recv = recv(sock_fd, msg, to_recv, 0)) == -1) {
            perror("recv");
            printf("Unable to recieve requested file: %s", path);
            fclose(fp);
            return -1;
        }
        
        // write them to the filestream
        for (i = 0; i < to_recv; i++) {
            fputc(msg[i], fp);
        }

        // decrease the amount of bytes left to recieve
        fsize -= to_recv;
    }

    fclose(fp);
    return 0;
}

/*
 * retrieves the network byte-order representation of the IP in the given
 *  sockaddr
 */
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

/* 
 * fills sockaddr_in struct given address params
 */
void fill_sockaddr(int family, char *ip, int port, struct sockaddr_in *addr) {
    addr->sin_family = family;
    addr->sin_addr.s_addr = inet_addr(ip);
    addr->sin_port = htons(port);
}

/* 
 * creates a socket connection binded to a local port
 * does not listen or accept, handle those functions elsewhere
 * returns the socket upon succes, -1 otherwise
 */
int initcon_local(char *ip, int port, struct sockaddr_in *addr) {
    int sock_fd;
    int opt = 1;
    
    fill_sockaddr(AF_INET, ip, port, addr);

    // create socket
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket:");
        return -1;
    }

    // allow port reuseability
    if (setsockopt(sock_fd, SOL_SOCKET, 
                SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt");
        return -1;
    }
    
    // bind to local port
    if (bind(sock_fd, (struct sockaddr *) addr, sizeof(*addr)) == -1) {
        perror("bind failed");
        return -1;
    }

    return sock_fd;
}

/* creates a socket connection to remote port
 * returns the socket upon success, -1 otherwise
 */
int initcon_remote(char *ip, int port, struct sockaddr_in *addr, float timeout) {
    int sock_fd;
    time_t time_start;

    fill_sockaddr(AF_INET, ip, port, addr);
   
    // create socket 
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket:");
        return -1;
    }
    
    time(&time_start);

    // attempt to connect until timeout
    while (difftime(time(NULL), time_start) < timeout) { 
        if (connect(sock_fd, (struct sockaddr *) addr, sizeof(*addr)) == -1) {
            perror ("connect:");
            usleep(DELAY_CONN);
        }
        else {
            // successfully connected
            return sock_fd;
        }
    }

    // connection attempts timed out
    return -1;
}
