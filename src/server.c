#include "../lib/utils.h"
#include "../lib/macros.h"

int parse_req(char* req) {

}


int send_file(int sock_fd, char* path, char* msg, int msg_len) {
    int i;
    FILE *fp;
    char ch_current;
    int eof_reached = 0;

    fp = fopen(path, "r");

    if (fp == NULL) {
        perror("fopen");
        return -1;
    }

    while (1) {
        // i is used to track how many chars have been buffered
        for (i = 0; i < msg_len; i++) {
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
            respond(sock_fd, msg, i + 1);
        }

        // break since we have reached the end of the file
        if (eof_reached) {
            break;
        }
    }

    return 0;
}


int main() {
    // generic vars
    int i, j;

    int server_fd, newcon_fd;
    struct sockaddr_in addr_server;
    struct sockaddr_storage addr_remote; // addr for remote clients
    int addrlen; // holds length of addr struct 
    char ip_remote[INET6_ADDRSTRLEN]; // holds remote ip
    
    // vars for select
    struct timeval tv;
    fd_set readfds_master; // master set of fd's
    fd_set readfds_temp; // modifiable set of fd's
    int fdmax = 0;

    if ((server_fd = initcon_local(LOCALHOST, PORT_SERV, &addr_server)) == -1) {
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, BACKLOG) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // selection processes
    tv.tv_sec = 2;
    tv.tv_usec = 500000;

    // reset fds
    FD_ZERO(&readfds_master);
    FD_ZERO(&readfds_temp);

    // set fdmax to the server socket
    fdmax = server_fd;

    // main loop:
    // handles accepts from new clients and broadcasts messages provided by
    // stdin
    while (1) {
        readfds_temp = readfds_master;
    
        // ingore writefds and exceptionfds
        // wait for connections indefinately
        if (select(fdmax + 1, & readfds_temp, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }

        // loop through all fd's
        for (i = 0; i <= fdmax; i++) {
            // check if fd is ready to be read
            if (!FD_ISSET(i, &readfds_temp)) {
                continue;
            }

            // handle new connections
            if (i == server_fd) {
                addrlen = sizeof(addr_remote);
                // accept new connections
                if ((newcon_fd = accept(server_fd, 
                                (struct sockaddr *) &addr_remote,  
                                &addrlen)) == -1) { 
                    perror("accept"); 
                    continue;
                } 
                
                // add new fd to master fds 
                FD_SET(newcon_fd, &readfds_master);

                // update new max
                if (newcon_fd > fdmax) {
                    fdmax = newcon_fd;
                }

                // continue if not connected to logger
                if (logger_fd == NULL_FD) {
                    continue;
                }

                snprintf(msg, MSG_LEN, 
                        "New connection from %s on socket %d\n", 
                        inet_ntop(addr_remote.ss_family,
                            get_in_addr((struct sockaddr *) &addr_remote),
                            ip_remote,
                            INET6_ADDRSTRLEN),
                        newcon_fd);

            }
            // handle incoming data from client
            else {
                // read in data
                if ((bytes_read = recieve(i, msg, MSG_LEN)) <= 0) {
                    // clean up connection
                    close(i);
                    FD_CLR(i, &readfds_master);
                    continue;
                }
                
                // continue if not connected to logger
                if (logger_fd == NULL_FD) {
                    continue;
                }

                // log the message
                printf("[MSG] %d: %s\n", i, msg);
            }
        }
    }
}
