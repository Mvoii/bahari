#include "sock_utils.h"

/**
 * create_Ipv4_addr - creates an ipv address
 * 
 * char *ip: ip address
 * int port: port number
 * 
 * returns: the address
 */
struct sockaddr_in *create_Ipv4_addr(char *ip, int port)
{
    struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));
    if (!address)
    {
        printf("addr_in malloc failed\n");
        return (0);
    }
    address->sin_family = AF_INET;
    address->sin_port = htons(port);
    // address.sin_addr.s_addr;

    if ((strlen(ip)) == 0)
        address->sin_addr.s_addr = INADDR_ANY;
    else
        inet_pton(AF_INET, ip, &address->sin_addr.s_addr);
    return address;
}

/**
 * create_TCP_Ipv4_socket - creates a socket
 * returns: socket ipv4 address
 */
int create_TCP_Ipv4_socket()
{
    return (socket(AF_INET, SOCK_STREAM, 0));
}

/*
struct accepted_socket_t *accept_incoming_connection(int serverSocketFD)
{
    struct sockaddr_in client_addr;
    int client_addr_size = sizeof (struct sockaddr_in);
    int client_socket_fd = accept(serverSocketFD, &client_addr, &client_addr_size);

    struct accepted_socket_t *accepted_socket = malloc(sizeof (struct accepted_socket_t));
    accepted_socket->new_client_addr = &client_addr;
    accepted_socket->accepted_socket_fd = client_socket_fd;
    accepted_socket->accepted_successfully = (client_socket_fd > 0);

    if (!accepted_socket->accepted_successfully)
        accepted_socket->error = client_socket_fd;

    return accepted_socket;
}

void receive_and_print_incoming_data(int socket_fd)
{
    char buffer[1024];
    while (1)
    {
        ssize_t amount_received = recv(socket_fd, buffer, sizeof(buffer), 0);

        if (amount_received > 0)
        {
            buffer[amount_received] = '\0';
            printf("Responce was:\n%s\n", buffer);
        }

        if (amount_received == 0)
            break;
    }
    close(socket_fd);
}

void start_accepting_incoming_connections(int serverSocketFD)
{
    pthread_t id;
    pthread_create(&id, NULL, accept_new_connection_and_print_data, serverSocketFD);
}

void accept_new_connection_and_print_data(int serverSocketFD)
{
    while(true)
    {
        struct accepted_socket_t *client_socket = accept_incoming_connection(serverSocketFD);
        
        receive_and_print_incoming_data_on_thread(client_socket, serverSocketFD);
    }
}

void receive_and_print_incoming_data_in_thread(struct accepted_socket_t *p_socket)
{
    pthread_t id;
    pthread_create(&id, NULL, receive_and_print_incoming_data,p_socket->accepted_socket_fd);
}
*/
