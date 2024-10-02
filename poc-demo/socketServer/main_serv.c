#include "../socketUtils/sock_utils.h"

#define MAX_CLIENTS 10

struct accepted_socket_t
{
    int accepted_socket_fd;
    struct sockaddr_in *new_client_addr;
    int error;
    bool accepted_successfully;
};

struct accepted_socket_t all_accepted_sockets[MAX_CLIENTS];
int accepted_sockets_count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct accepted_socket_t *accept_incoming_connection(int serverSocketFD);

void *receive_and_broadcast_data(void *arg);
void start_accepting_incoming_connections(int serverSocketFD);
/* void accept_new_connection_and_print_data(int serverSocketFD);
void receive_and_print_incoming_data_in_thread(struct accepted_socket_t *p_socket); */
void send_to_other_clients(char *buffer, int socket_fd);

int main()
{
    int serverSocketFD = create_TCP_Ipv4_socket();
    if (serverSocketFD == -1)
    {
        printf("Failed to create socket\n");
        return (0);
    }

    struct sockaddr_in *server_address = create_Ipv4_addr("", 2000);

    int result = bind(serverSocketFD, (struct sockaddr *)server_address, sizeof(*server_address));
    if (result == 0)
    {
        printf("server socket bind successful\n");
    }
    else
    {
        printf("server socket bind failed\n");
        return (0);
    }
    
    int listen_res = listen(serverSocketFD, 10);
    if (listen_res == 0)
        printf("waiting for connection... \n");
    
    /* struct sockaddr_in client_address;
    int client_addr_size = sizeof(client_address);

    int client_sock_fd = accept(serverSocketFD, &client_address, &client_addr_size); */

    start_accepting_incoming_connections(serverSocketFD);
 
    shutdown(serverSocketFD, SHUT_RDWR);
    return (0);
}

struct accepted_socket_t *accept_incoming_connection(int serverSocketFD)
{
    struct sockaddr_in *client_addr = malloc(sizeof(struct sockaddr_in));
    socklen_t client_addr_size = sizeof (struct sockaddr_in);
    int client_socket_fd = accept(serverSocketFD, (struct sockaddr *)client_addr, &client_addr_size);

    struct accepted_socket_t *accepted_socket = malloc(sizeof (struct accepted_socket_t));
    accepted_socket->new_client_addr = client_addr;
    accepted_socket->accepted_socket_fd = client_socket_fd;
    accepted_socket->accepted_successfully = (client_socket_fd > 0);

    if (!accepted_socket->accepted_successfully)
        accepted_socket->error = client_socket_fd;

    return accepted_socket;
}

void *receive_and_broadcast_data(void *arg)
{
    struct accepted_socket_t *client_socket = (struct accepted_socket_t *) arg;
    int socket_fd = client_socket->accepted_socket_fd;
    char buffer[1024];

    while (1)
    {
        ssize_t amount_received = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);

        if (amount_received > 0)
        {
            buffer[amount_received] = '\0';
            printf("Received from %d: %s\n", socket_fd, buffer);

            send_to_other_clients(buffer, socket_fd);
        }

        if (amount_received == 0)
            break;
    }
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < accepted_sockets_count; i++)
    {
        if (all_accepted_sockets[i].accepted_socket_fd == socket_fd)
        {
            // remove socket from the array
            memmove(&all_accepted_sockets[i], &all_accepted_sockets[i + 1], (accepted_sockets_count - i - 1) * sizeof(struct accepted_socket_t));

            accepted_sockets_count--;
            break;
        }
    }
    pthread_mutex_unlock(&mutex);

    close(socket_fd);
    free(client_socket->new_client_addr);
    free(client_socket);

    return NULL;
}

void start_accepting_incoming_connections(int serverSocketFD)
{
    while(1)
    {
        struct accepted_socket_t *client_socket = accept_incoming_connection(serverSocketFD);
        if (client_socket->accepted_successfully)
        {
            pthread_t thread_id;
            pthread_mutex_lock(&mutex);
            if (accepted_sockets_count < MAX_CLIENTS)
            {
                all_accepted_sockets[accepted_sockets_count++] = *client_socket;
                pthread_create(&thread_id, NULL, receive_and_broadcast_data, client_socket);
                printf("New client hopped in. total: %d\n", accepted_sockets_count);
            }
            else
            {
                printf("Max number of clients reached. Connection denied.\n");
                close(client_socket->accepted_socket_fd);
                free(client_socket->new_client_addr);
                free(client_socket);
            }
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            printf("Failed to accept client connection\n");
            free(client_socket->new_client_addr);
            free(client_socket);
        }
        // receive_and_print_incoming_data_in_thread(client_socket);
    }
}

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
