#include "../include/chat_utils.h"

int main(int argc, char *argv[])
{
    // Check if server ip is privided
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <server_ip>\n", argv[0]);
        exit(1);
    }

    // Create a sock
    int socket_fd = create_TCP_IPv4_socket();
    if (socket_fd == -1)
    {
        perror("Failed to create socket");
        return 1;
    }

    // Set up server addr struct
    struct sockaddr_in server_addrress;
    server_addrress.sin_family = AF_INET;
    server_addrress.sin_port = htons(PORT);

    // Convert ip addr from string to binary
    if (inet_pton(AF_INET, argv[1], &server_addrress.sin_addr) <= 0)
    {
        perror("Invalid address/ Address not supported");
        return 1;
    }

    // connect to server
    if (connect(socket_fd, (struct sockaddr *)&server_addrress, sizeof(server_addrress)) != 0)
    {
        perror("Failed to connect to server");
        return 1;
    }

    // Create a thread to listen for incoming messages
    pthread_t listen_thread;
    pthread_create(&listen_thread, NULL, listen_and_print, &socket_fd);

    // Main loop to send messages
    char *text_buff = NULL;
    size_t text_buff_size = 0;
    printf("Type here ('exit' to quit):\n");

    while (1)
    {
        ssize_t char_count = getline(&text_buff, &text_buff_size, stdin);

        if (char_count > 0)
        {
            if (strcmp(text_buff, "exit\n") == 0)
                break;
            if (send(socket_fd, text_buff, char_count, 0) < 0)
            {
                perror("Failed to send message");
                break;
            }
        }
    }

    // Clean up
    pthread_cancel(listen_thread);
    pthread_join(listen_thread, NULL);
    free(text_buff);
    close(socket_fd);

    return 0;
}
