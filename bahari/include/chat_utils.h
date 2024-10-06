#ifndef CHAT_UTILS_H
#define CHAT_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_CLIENTS 10
#define PORT 8080
#define BUFFER_SIZE 1024

// struct to hold info about accepted socket
struct accepted_socket_t
{
    int accepted_socket_fd;
    struct sockaddr_in client_addr;
    int error;
    bool accepted_successfully;
};

int create_TCP_IPv4_socket();
void *listen_and_print(void *arg);
struct accepted_socket_t *accept_incoming_connection(int serversock_fd);
void *receive_and_broadcast_data(void *arg);
void start_accepting_incoming_connections(int serversock_fd);
void send_to_other_clients(char *buffer, int socket_fd);

#endif /* CHAT_UTILS_H */
