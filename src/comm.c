#include "comm.h"

/*
 * sends a message of size msg_len
 * msg_len - the size of the buffer
 * to_send - the amount of bytes you wish to send
 */
int32_t send_msg(int sock_fd, char *msg, int32_t msg_len, int32_t to_send) {
    int32_t bc = 0; // bytes communicated
    int32_t bt = 0; // bytes total

    if (to_send > msg_len) {
        printf("send_msg: to_send is larger than buffer");
        return -1;
    }

    // send recipient bytes of msg to recv
    send_int32_t(sock_fd, to_send);

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

    return bt;
}

/*
 * receives a message of size msg_len
 * msg_len - the size of the msg buffer
 */
int32_t recv_msg(int sock_fd, char *msg, int32_t msg_len) {
    int32_t bc; // bytes communicated
    int32_t bt; // bytes total
    int32_t to_recv;
    int stat_recv_int;


    // recv bytes to read
    if ((stat_recv_int = recv_int32_t(sock_fd, &to_recv)) <= 0) {
        printf("recv_msg: socket error");
        return stat_recv_int;
    }

    if (to_recv > msg_len) {
        // push to_recv back into the socket and return
        printf("recv_msg: buffer not large enough for incoming msg\n\n");
        return -1;
    }

    //reset the msg buffer
    memset(msg, 0, msg_len); 
    bt = to_recv;

    do {
        if ((bc = recv(sock_fd, msg, to_recv, 0)) <= 0) {
            printf("recv_msg: socket error");
            return bc;
        }
        char 
        else {
            msg += bc;
            to_recv -= bc;
        }
    }
    while (to_recv > 0);

    return bt;
}

/*
 *  sends a 32 bit int
 */
int send_int32_t(int sock_fd, int32_t num) {
    // convert to standard 32 bit int in network byte order
    int32_t conv = htonl(num);
    int left = sizeof(conv);
    int bt = left; // bytes total
    int bc;
    char *msg = (char *) &conv;
    
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

            return bc;
        }
        else {
            msg += bc;
            left -= bc;
        }
    }
    while (left > 0);

    return bt;
}

/*
 *  receives a 32 bit int
 */
int recv_int32_t(int sock_fd, int32_t *num)
{
    int32_t ret;
    int left = sizeof(ret);
    int bt = left; // bytes total
    int bc;
    char *msg = (char*) &ret;

    do {
        if ((bc = recv(sock_fd, msg, left, 0)) <= 0) {

            if (bc == 0) {
                printf("selectserver: socked %d hung up\n", sock_fd);
            }
            else {
                perror("recv");
            }

            return bc;
        }
        else {
            msg += bc;
            left -= bc;
        }
    }
    while (left > 0);

    *num = ntohl(ret);
    return bt;
}

/*
 * sends the file at path to sock_fd
 */
int32_t send_file(int sock_fd, char *path) {
    char buf[BUF_LEN];
    char *buf_p;
    FILE *fp;
    struct stat st;
    int32_t fsize;
    int32_t to_send;
    int32_t bc; // bytes communicated
    long bt; // bytes total

    // open filestream of requested file
    fp = fopen(path, "r");
    if (fp == NULL) {
        perror("fopen");
        return -1;
    }

    // first determine the total number of bytes to transfer
    fstat(fileno(fp), &st);
    fsize = st.st_size;

    // send recipient file size
    // fsize is casted to an int32_t from a signed int
    send_int32_t(sock_fd, fsize);
    bt = fsize;
    
    // begin sending the file
    while (fsize > 0) {
        // buffer in a chunk of file data
        to_send = fread(buf, sizeof(char), BUF_LEN, fp);

        // error handling
        if (ferror(fp)) {
            fclose(fp);
            return -1;
        }

        // if i <= 0 then no bytes have been buffered, so don't send anything
        if (to_send > 0) {
            // determine how many bytes to send for this buffer
            buf_p = buf;

            // decrement the amount of bytes left to send by the initial value of
            // to_send before we modified it
            fsize -= to_send;

            // loop until all bytes in buffer are sent
            do {
                if ((bc = send(sock_fd, buf_p, to_send, 0)) == -1) {
                    perror("send");
                    printf("Unable to send requested file: %s\n\n", path);
                    fclose(fp);
                    return -1;
                }
                else {
                    buf_p += bc;
                    to_send -= bc;
                }
            }
            while (to_send > 0);
        }
    }

    fclose(fp);
    return bt;
}

/*
 *  receives file from sock_fd and writes it to path
 */
int32_t recv_file(int sock_fd, char *path) {
    int i;
    char buf[BUF_LEN];
    FILE *fp;
    int32_t fsize;
    int32_t bc; // bytes communicated
    int32_t bt; // bytes total 
    int32_t stat_recv_int;
    
    // open filestream to write into
    fp = fopen(path, "w");
    if (fp == NULL) {
        perror("fopen");
        return -1;
    }

    // recv file size
    if ((stat_recv_int = recv_int32_t(sock_fd, &fsize)) <= 0) {
        printf("recv_file: socket error");
        return stat_recv_int;
    }
    bt = fsize;

    // continue to receive file bytes until specified amount is reached
    while (fsize > 0) {
        // receive the bytes
        if ((bc = recv(sock_fd, buf, MSG_LEN, 0)) <= 0) {
            perror("recv");
            printf("recv_file: unable to receive requested file: %s", path);
            fclose(fp);
            return bc;
        }
        
        // write them to the filestream
        fwrite(buf, sizeof(char), bc, fp);

        // error handling
        if (ferror(fp)) {
            fclose(fp);
            return -1;
        }

        // decrease the amount of bytes left to receive
        fsize -= bc;
        printf("FSIZE: %d", fsize);
    }

    fclose(fp);
    return bt;
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

