#include "utils.h"

// min of two ints
int min(int x, int y)
{
  return (x < y) ? x : y
}

// max of two ints
int max(int x, int y)
{
  return (x > y) ? x : y
}

char *str_concat(const char *s1, const char *s2) {
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1+len2+1);
    memcpy(result, s1, len1);
    memcpy(result+len1, s2, len2+1);
    return result;
}

// retrieves the network byte-order representation of the IP in the given
// sockaddr
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

// fills sockaddr_in struct given address params
void fill_sockaddr(int family, char *ip, int port, struct sockaddr_in *addr) {
    addr->sin_family = family;
    addr->sin_addr.s_addr = inet_addr(ip);
    addr->sin_port = htons(port);
}

// creates a socket connection binded to a local port
// does not listen or accept, handle those functions elsewhere
// returns the socket upon succes, -1 otherwise
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

// creates a socket connection to remote port
// returns the socket upon success, -1 otherwise
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
