#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "calendar_print.h"

#define LISTEN_BACKLOG 5
#define SERVER_PORT 9191
#define SERVER_IP "0.0.0.0"
#define GREETING_MESSAGE "Hello world!\n"

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &addr.sin_addr) <= 0) {
        perror("Set ip failed");
        exit(EXIT_FAILURE);
    }

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof addr) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, LISTEN_BACKLOG) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    socklen_t addrlen = sizeof addr;

    while (1) {
        int client_fd = accept(server_fd, (struct sockaddr *)&addr, &addrlen);
        if (client_fd == -1) {
            perror("Accept failed");
            continue;
        }

        std::string message = PrintMonth(2025, 4);
        ssize_t remaining_bytes = message.size();
        while (remaining_bytes > 0) {
            ssize_t sended_bytes =
                write(client_fd, message.c_str() + (message.size() - remaining_bytes), remaining_bytes);

            if (sended_bytes == -1) {
                perror("Write failed");
                break;
            }
            remaining_bytes -= sended_bytes;
        }

        if (close(client_fd) == -1) {
            perror("Close failed client_fd");
        }
    }

    if (close(server_fd) == -1) {
        perror("Close failed server_fd");
    }
    return 0;
}