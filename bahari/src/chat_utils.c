#include "../include/chat_utils.h"

// globals for the serv
struct accepted_socket_t all_accepted_sockets[MAX_CLIENTS];
int accepted_sockets_count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// create a TCP IPv4 socket
int create_TCP_IPv4_socket()
{
    return (socket(AF_INET, SOCK_STREAM, 0));
}

// Thread function to listen for and print incoming messages (cient-side)
void *listen_and_print(void *arg)
{
    int socket_fd = *(int *)arg;
    char buffer[BUFFER_SIZE];

    while(1)
    {
        ssize_t amount_received = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
        if (amount_received > 0)
        {
            buffer[amount_received] = 0;
            printf("%s\n", buffer);
        }
        if (amount_received <= 0)
        {
            printf("Server disconnected\n");
            break;
        }
    }
    return NULL;
}

// Accept an incoming connection (server-side)
struct accepted_socket_t *accept_incoming_connection(int serversock_fd)
{
    struct accepted_socket_t *accepted_socket = malloc(sizeof(struct accepted_socket_t));
    socklen_t client_addr_size = sizeof(struct sockaddr_in);

    int client_socket_fd = accept(serversock_fd, (struct sockaddr *)&accepted_socket->client_addr, &client_addr_size);

    accepted_socket->accepted_socket_fd = client_socket_fd;
    accepted_socket->accepted_successfully = (client_socket_fd > 0);
    if (!accepted_socket->accepted_successfully)
        accepted_socket->error = client_socket_fd;

    return accepted_socket;
}

// Thread function to resolve and broadcast data (server-side)
void *receive_and_broadcast_data(void *arg)
{
    struct accepted_socket_t *client_socket = (struct accepted_socket_t *)arg;
    int socket_fd = client_socket->accepted_socket_fd;
    char buffer[BUFFER_SIZE];
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_socket->client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);

    while (1)
    {
        ssize_t amount_received = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
        if (amount_received > 0)
        {
            buffer[amount_received] = '\0';
            printf("Received from %s: %s", client_ip, buffer);

            // Prepend the sender's IP to teh message
            char message[BUFFER_SIZE + INET_ADDRSTRLEN + 4];
            snprintf(message, sizeof(message) ,"[%s]: %s", client_ip, buffer);
            send_to_other_clients(message, socket_fd);
        }
        if (amount_received <= 0)
            break;
    }

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < accepted_sockets_count; i++)
    {
        if (all_accepted_sockets[i].accepted_socket_fd == socket_fd)
        {
            memmove(&all_accepted_sockets[i], &all_accepted_sockets[i + 1], (accepted_sockets_count - i - 1) * sizeof(struct accepted_socket_t));
            accepted_sockets_count--;
            break;
        }
    }
    pthread_mutex_unlock(&mutex);

    printf("Client %s disconnected\n", client_ip);
    close(socket_fd);
    free(client_socket);
    return NULL;
}

// Start accepting incoming connections (server-side)
void start_accepting_incoming_connections(int serversock_fd)
{
    while (1)
    {
        struct accepted_socket_t *client_socket = accept_incoming_connection(serversock_fd);
        if (client_socket->accepted_successfully)
        {
            pthread_t thread_id;
            pthread_mutex_lock(&mutex);
            if (accepted_sockets_count < MAX_CLIENTS)
            {
                all_accepted_sockets[accepted_sockets_count++] = *client_socket;
                pthread_create(&thread_id, NULL, receive_and_broadcast_data, client_socket);

                char client_ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(client_socket->client_addr.sin_addr), client_ip, accepted_sockets_count);
                printf("New Client connected from %s. Total connections: %d\n", client_ip, accepted_sockets_count);
            }
            else
            {
                printf("Maximum number of clients reached. Connection denied\n");
                close(client_socket->accepted_socket_fd);
                free(client_socket);
            }
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            perror("Failed to accept client connection");
            free(client_socket);
        }
    }
}

// send messages to all other clients (server-side)
void send_to_other_clients(char *buffer, int socket_fd)
{
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < accepted_sockets_count; i++)
    {
        if (all_accepted_sockets[i].accepted_socket_fd != socket_fd)
        {
            send(all_accepted_sockets[i].accepted_socket_fd, buffer, strlen(buffer), 0);
        }
    }
    pthread_mutex_unlock(&mutex);
}
