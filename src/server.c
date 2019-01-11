#include "macros.h"
#include "config.h"
#include "utils.h"
#include "comm.h"
#include "cmd.h"
#include "cmd_util.h"
#include "hash_map.h"

int main() {
    // generic vars
    int i, j;

    int server_fd, newcon_fd;
    struct sockaddr_in addr_server;
    struct sockaddr_storage addr_remote; // addr for remote clients
    int addrlen; // holds length of addr struct 
    char ip_remote[INET6_ADDRSTRLEN]; // holds remote ip
    
    // communication vars
    char msg[MSG_LEN] = {0}; // buffer for sent and recieved messages
    int32_t cmd;
    int bc = 0; // tracks the bytes communicated

    // vars for select
    struct timeval tv;
    fd_set readfds_master; // master set of fd's
    fd_set readfds_temp; // modifiable set of fd's
    int fdmax = 0;

    // hold client sync_info's
    hash_map *hm_sync_info;
    sync_info *new_info;

    if ((server_fd = initcon_local(LOCALHOST, PORT_SERV, &addr_server)) == -1) {
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, BACKLOG) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    printf("Socket initiated, listening for connections..\n\n");

    // selection processes
    tv.tv_sec = 2;
    tv.tv_usec = 500000;

    // reset fds
    FD_ZERO(&readfds_master);
    FD_ZERO(&readfds_temp);

    // set preliminary sockets
    FD_SET(server_fd, &readfds_master);

    // set fdmax to the server socket
    fdmax = server_fd;

    hm_sync_info = hash_map_init(HM_CAP_INIT, HM_THRESH, 
        &data_free_sync_info);
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

                snprintf(msg, MSG_LEN, 
                        "New connection from %s on socket %d\n\n", 
                        inet_ntop(addr_remote.ss_family,
                            get_in_addr((struct sockaddr *) &addr_remote),
                            ip_remote,
                            INET6_ADDRSTRLEN),
                        newcon_fd);

                printf("%s", msg);
            }
            // handle incoming data from client
            else {
                // read in data
                if (cmd_acknowledge(i, &cmd) <= 0) {
                    // clean up connection
                    close(i);
                    FD_CLR(i, &readfds_master);
                    continue;
                }
                
                printf("Recieved cmd: %d\n\n", cmd);
                // parsex the msg and provide it the corresponding
                // client sync info
                parsex(cmd, hm_sync_info, i);
            }
        }
    }
}
