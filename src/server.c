#include "../lib/macros.h"
#include "../lib/config.h"
#include "../lib/utils.h"
#include "../lib/comm.h"


// setup later
/*
struct command {
    char *name;
    void (*function) (void);
};

struct command commands[] = {
    {"req_file", req_file(int, char *)},
}

// get corresponding command function for command string
int (*resolve_command(char *cmd))(int, char *) {
    int i;
    for (i = 0; i < sizeof(commands); i++) {
        // found the corresponding command, so return the function
        if (strncmp(commands[i].name, cmd) == 0) {
            return commands[i].function;
        }
    }
    
    // cmd not found
    return NULL;
}


// command functions
int req_file(int sock_fd, char *path) {
    return send_file(sock_fd, path);
}*/

// parse and execute command
// client commands follow the following format:
// command:arg1,arg2,...
//
// The following commands are:
// retr_file:file_path
int parsex(char* msg, int sender_fd) {
    char cmd[MSG_LEN];
    char *msg_p = strchr(msg, ':');

    if (msg_p == NULL) {
        printf("Command has incorrect format\n\n");
        return -1;
    }
    
    // copy the contents of the command into its own buffer
    strncpy(cmd, msg, msg_p - msg);
    cmd[msg_p - msg] = '\0';

    // req_p now points to the arg
    msg_p++;
    
    // determine which function to call
    if (strncmp(cmd, RETRIEVE, MSG_LEN) == 0) {
        send_file(sender_fd, msg_p);
    }
    // other commands to be implemented
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
    
    // communication vars
    char msg[MSG_LEN] = {0}; // buffer for sent and recieved messages
    int bc = 0; // tracks the bytes communicated

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

    // set preliminary sockets
    FD_SET(server_fd, &readfds_master);

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

                snprintf(msg, MSG_LEN, 
                        "New connection from %s on socket %d\n", 
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
                if (recv_msg(i, msg, MSG_LEN) == -1) {
                    // clean up connection
                    close(i);
                    FD_CLR(i, &readfds_master);
                    continue;
                }
                
                printf("Recieved msg: %s\n\n", msg);
                // parsex the msg
                parsex(msg, i);
            }
        }
    }
}
