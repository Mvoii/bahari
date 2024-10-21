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

// Struct to hold info about accepted socket
struct peer_socket_t {
    int peer_socket_fd;
    struct sockaddr_in peer_addr;
};

// Globals for all peers
struct peer_socket_t all_peers[MAX_CLIENTS];
int peer_count = 0;
pthread_mutex_t peer_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function declarations
int create_TCP_IPv4_socket();
void *listen_and_print(void *arg);
void *peer_listener(void *arg);
void connect_to_peers(char **peer_ips, int peer_count);
void broadcast_message(char *buffer);
void accept_new_peer(int server_fd);
void send_to_peers(char *buffer, int socket_fd);

#endif /* CHAT_UTILS_H */
