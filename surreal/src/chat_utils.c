// chat_utils.h
#include "../include/chat_utils.h"

struct peer_socket_t all_peers[MAX_CLIENTS];
int peer_count = 0;
pthread_mutex_t peer_mutex = PTHREAD_MUTEX_INITIALIZER;

// Create a TCP/IPv4 socket
int create_TCP_IPv4_socket()
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1)
    {
        perror("Failed to create socket");
        return -1;
    }
    return sock_fd;
}

// listen for incoming peer connections
void *peer_listener(void *arg)
{
    int server_fd = *(int *)arg;
    struct sockaddr_in peer_addr;
    socklen_t addr_len = sizeof(peer_addr);

    while (1)
    {
        int peer_fd = accept(server_fd, (struct sockaddr *)&peer_addr, &addr_len);
        if (peer_fd < 0)
        {
            perror("Failed to accept connection");
            continue;
        }

        // add new peer to lobal list
        pthread_mutex_lock(&peer_mutex);
        if (peer_count < MAX_CLIENTS)
        {
            all_peers[peer_count].peer_socket_fd = peer_fd;
            all_peers[peer_count].peer_addr = peer_addr;
            peer_count++;
            printf("New peer connected. Total peers: %d\n", peer_count);
        }
        else
        {
            printf("Max peer limit reached. Closing connection...\n");
            close(peer_fd);
        }
        pthread_mutex_unlock(&peer_mutex);
    }
}

// connect to other peers
void connect_to_peers(char **peer_ips, int peer_count)
{
    struct sockaddr_in peer_addr;

    for (int i = 0; i < peer_count; i++)
    {
        int peer_fd = create_TCP_IPv4_socket();
        if (peer_fd < 0)
        {
            perror("Failed to create socket for peer");
            continue;
        }

        peer_addr.sin_family = AF_INET;
        peer_addr.sin_port = htons(PORT);
        if (inet_pton(AF_INET, peer_ips[i], &peer_addr.sin_addr) <= 0)
        {
            perror("Invalid address");
            continue;
        }

        if (connect(peer_fd, (struct sockaddr *)&peer_addr, sizeof(peer_addr)) < 0)
        {
            perror("Connection failed");
            close(peer_fd);
            continue;
        }

        // Add connected peer to global list
        pthread_mutex_lock(&peer_mutex);
        if (peer_count < MAX_CLIENTS)
        {
            all_peers[peer_count].peer_socket_fd = peer_fd;
            all_peers[peer_count].peer_addr = peer_addr;
            peer_count++;

            print("Connected to peer %s\n", peer_ips[i]);
        }
        else
        {
            printf("Max peer limit reached. Cannot connect to more peers\n");
            close(peer_fd);
        }
        pthread_mutex_unlock(&peer_mutex);
    }
}

// broadcast a message to all peers
void broadcast_message(char *buffer)
{
    pthread_mutex_lock(&peer_mutex);
    for (int i = 0; i < peer_count; i++)
    {
        if(send(all_peers[i].peer_socket_fd, buffer, strlen(buffer), 0) < 0)
        {
            perror("Failed to send message to peer");
        }
    }
    pthread_mutex_unlock(&peer_mutex);
}
