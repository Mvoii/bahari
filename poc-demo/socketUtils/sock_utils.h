#ifndef SOCK_UTILS_H /* SOCK_UTILS_H */
#define SOCK_UTILS_H

#include <stdio.h>
#include <malloc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

int create_TCP_Ipv4_socket();
struct sockaddr_in *create_Ipv4_addr(char *ip, int port);

/*
struct accepted_socket_t
{
    int accepted_socket_fd;
    struct sockaddr_in *new_client_addr;
    int error;
    bool accepted_successfully;
};

struct accepted_socket_t *accept_incoming_connection(int serverSocketFD);

void receive_and_print_incoming_data(int socket_fd);
void start_accepting_incoming_connections(int serverSocketFD);
void accept_new_connection_and_print_data(int serverSocketFD);
void receive_and_print_incoming_data_in_thread(struct accepted_socket_t *p_socket);
*/

#endif /* SOCK_UTILS_h */
