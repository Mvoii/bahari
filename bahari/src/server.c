#include  "../include/chat_utils.h"

int main(void)
{
    // create a sock
    int serversock_fd = create_TCP_IPv4_socket();
    if (serversock_fd == -1)
    {
        perror("Failed to create server socket");
        return 1;
    }

    // set up server addr struct
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY; // bind all to available interface
    server_address.sin_port = htons(PORT);

    // Bind the sock to server addr
    if (bind(serversock_fd, (struct sockaddr *)&server_address, sizeof(server_address)) != 0)
    {
        perror("Server bind failed");
        return 1;
    }
    printf("Server bind successful\n");

    // Start listening for incomming connections
    if (listen(serversock_fd, 10) != 0)
    {
        perror("Listen failed");
        return 1;
    }
    printf("Waiting for connection on port %d...\n", PORT);

    // Start accepting incoming connections
    start_accepting_incoming_connections(serversock_fd);

    // Clean up
    shutdown(serversock_fd, SHUT_RDWR);

    return 0;
}