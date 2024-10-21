// main.c
#include "../include/chat_utils.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <peer_ip_1> [peer_ip_2] .. \n", argv[0]);
        return 1;
    }

    // start the server (peer listening)
    int server_fd = create_TCP_IPv4_socket();
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 10) < 0)
    {
        perror("Listen failed");
        close(server_fd);
        return 1;
    }

    pthread_t listener_thread;
    pthread_create(&listener_thread, NULL, peer_listener, &server_fd);

    // connect to specific peers
    connect_to_peers(argv + 1, argc - 1);

    // main loop
    char *message = NULL;
    size_t message_size = 0;
    while (1)
    {
        getline(&message, &message_size, stdin);
        if (strcmp(message, "exit\n") == 0)
        {
            break;
        }
        broadcast_message(message);
    }

    // clean up
    free(message);
    pthread_cancel(peer_listener);
    pthread_join(listener_thread, NULL);
    close(server_fd);

    return 0;
}
