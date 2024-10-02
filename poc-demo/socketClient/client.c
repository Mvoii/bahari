#include "../socketUtils/sock_utils.h"

/* int create_TCP_Ipv4_socket();
struct sockaddr_in *create_Ipv4_addr(char *ip, int port); */
/* int create_TCP_Ipv4_socket()
{
    return (socket(AF_INET, SOCK_STREAM, 0));
}

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
    inet_pton(AF_INET, ip, &address->sin_addr.s_addr);
    return address;
} */

void *listen_and_print(void *arg);
// void listen_print(int socketfd);

int main()
{
    int socketFD = create_TCP_Ipv4_socket();

    char *ip = "127.0.0.1";

    struct sockaddr_in *address = create_Ipv4_addr(ip, 2000);
    

    int result = connect(socketFD, (struct sockaddr *)address, sizeof(*address));

    if (result == 0)
    {
        printf("connection successful\n");
    }
    else
    {
        printf("Failed to connect\n");
        return (0);
    }

    pthread_t listen_thread;
    pthread_create(&listen_thread, NULL, listen_and_print, &socketFD);

    // send stuff to the sock
    char *text_buff = NULL;
    size_t text_buff_size = 0;
    printf("Enter message to send (type 'exit' to quit):\n");

    // start_listening_and_print_messages_on_thread(socketFD);

    while(true)
    {
        ssize_t char_count = getline(&text_buff, &text_buff_size, stdin);
        if (char_count > 0)
        {
            if (strcmp(text_buff, "exit\n") == 0)
                break;

            ssize_t amount_sent = send(socketFD, text_buff, char_count, 0);
            if (amount_sent < 0)
            {
                perror("failed to send message");
                break;
            }
        }
    }

    pthread_cancel(listen_thread);
    pthread_join(listen_thread, NULL);
    free(text_buff);

    close(socketFD);
    free(address);

    return (0);
}


void *listen_and_print(void *arg)
{
    int socketfd = *(int *)arg;
    char buffer[1024];

    while(true)
    {
        ssize_t amount_received = recv(socketfd, buffer, sizeof(buffer) - 1, 0);

        if (amount_received > 0)
        {
            buffer[amount_received] = 0;
            printf("Response was:\n%s\n", buffer);

            // send_received_message_to_other_clients(buffer, socketfd);
        }

        if (amount_received == 0)
        {
            printf("server disconnected\n");
            break;
        }
        if (amount_received < 0)
        {
            perror("Error receiving messages");
            break;
        }
    }
    return NULL;
}
