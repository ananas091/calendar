#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket failed\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9191);
    if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0) {
        perror("Set ip failed\n");
        exit(EXIT_FAILURE);
    }

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof addr) == -1) {
        perror("Bind failed\n");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) == -1) {
        perror("Listen failed\n");
        exit(EXIT_FAILURE);
    }

    socklen_t addrlen = sizeof addr;

    while (1) {
        int client_fd = accept(server_fd, (struct sockaddr *)&addr, &addrlen);
        if (client_fd == -1) {
            perror("Accept failed\n");
            exit(EXIT_FAILURE);
        }

        if (write(client_fd, "Hello world!\n", sizeof("Hello world!\n")) == -1) {
            perror("Write failed\n");
        }

        close(client_fd);
    }
    close(server_fd);
    return 0;
}