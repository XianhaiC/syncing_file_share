#include "comm.h"

/*
 * sends a message of size msg_len
 */
int send_msg(int sock_fd, char *msg, int msg_len) {
    // bytes communicated
    int bc = 0;
    int left = msg_len;

    //write the contents of msg_buf into socket s and return status
    do {
        if ((bc = send(sock_fd, msg, msg_len, 0)) == -1) {
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

/*
 * recieves a message of size msg_len
 */
int recv_msg(int sock_fd, char *msg, int msg_len) {
    // bytes communicated
    int bc;
    int left = msg_len;

    //reset the msg buffer
    memset(msg, 0, msg_len); 
    
    do {
        if ((bc = recv(sock_fd, msg, msg_len, 0)) <= 0) {
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

/*
 *  sends a 32 bit int
 */
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
    FILE *fp;
    long fsize;
    char ch_current;
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

    // tell client the size of the file so it knows how much to read
    send_int(sock_fd, fsize);
    
    // begin sending the file
    // stop when the necessary amount of bytes have been sent
    while (fsize > 0) {
        // i is used to track how many chars have been buffered
        for (i = 0; i < MSG_LEN; i++) {
            // get the next char
            ch_current = fgetc(fp);
            if (ch_current == EOF) {
                // decrement i because we do not count this EOF char as a char
                // to be sent
                i--;
                eof_reached = 1;
                break;
            }
        }

        // send i + 1, totaling the amount of buffered chars, bytes
        // if i < 0 then no bytes have been buffered, so don't send anything
        if (i >= 0) {
            if (send_msg(sock_fd, msg, i + 1)) {
                printf("Unable to send requested file: %s", path);
                fclose(fp);
                return -1;
            }

            // decrease the amount of bytes left to send
            fsize -= i + 1;
        }

        // break since we have reached the end of the file
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
    int to_read;
    char ch_current;
    
    // open filestream to write into
    fp = fopen(path, "w");
    if (fp == NULL) {
        perror("fopen");
        return -1;
    }

    // recieve the byte size, or how many bytes to subsequently read to obtain the
    // entire file
    recv_int(sock_fd, &fsize);

    // continue to recieve file bytes until specified amount is reached
    while (fsize > 0) {
        // determine how many bytes to read for this partition
        to_read = min(fsize, MSG_LEN);

        // recieve the bytes
        if (recv_msg(sock_fd, msg, to_read) == -1) {
            printf("Unable to recieve requested file: %s", path);
            fclose(fp);
            return -1;
        }
        
        // write them to the filestream
        for (i = 0; i < to_read; i++) {
            fputc(msg[i], fp);
        }

        // decrease the amount of bytes left to recieve
        fsize -= to_read;
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
