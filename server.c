#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "server.h"
#include <fcntl.h>
extern char *global_directory;

struct Server server_Constructor(int domain, int port, int service, int protocol, int backlog, u_long interface, void (*launch)(struct Server *server)) {
    struct Server server;

    server.domain = domain;
    server.service = service;
    server.port = port;
    server.protocol = protocol;
    server.backlog = backlog;

    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = htonl(interface);

    server.socket = socket(domain, service, protocol);
    if (server.socket < 0) {
        perror("Failed to initialize/connect to socket...\n");
        exit(EXIT_FAILURE);
    }

    if (bind(server.socket, (struct sockaddr*)&server.address, sizeof(server.address)) < 0) {
        perror("Failed to bind socket...\n");
        exit(EXIT_FAILURE);
    }

    if (listen(server.socket, server.backlog) < 0) {
        perror("Failed to start listening...\n");
        exit(EXIT_FAILURE);
    }

    server.launch = launch;
    return server;
}

void launch(struct Server *server) {
    char buffer[BUFFER_SIZE];
    char *serve_directory = global_directory;
    while (1) {
        printf("=== WAITING FOR CONNECTION === \n");
        int addrlen = sizeof(server->address);
        int new_socket = accept(server->socket, (struct sockaddr*)&server->address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("Failed to accept connection");
            continue;
        }
        pid_t childpid = fork();
        if (childpid == 0) {
            close(server->socket);

        ssize_t bytesRead = read(new_socket, buffer, BUFFER_SIZE - 1);
        if (bytesRead < 0) {
            perror("Error reading buffer...\n");
            close(new_socket);
            exit(1);
        }
            buffer[bytesRead] = '\0'; 
            puts(buffer);

            char method[8], path[1024];
            sscanf(buffer, "%s %s", method, path);
            if (strcmp(method, "GET") != 0) {
                const char *response = "HTTP/1.0 405 Method Not Allowed\r\n\r\n";
                write(new_socket, response, strlen(response));
                close(new_socket);
                exit(0);
            }

            if (strstr(path, "..")) {
                const char *response = "HTTP/1.0 403 Forbidden\r\n\r\n";
                write(new_socket, response, strlen(response));
                close(new_socket);
                exit(0);
            }

            char full_path[2048];
            if (strcmp(path, "/") == 0)
                snprintf(full_path, sizeof(full_path), "%s/index.html", serve_directory);
            else
                snprintf(full_path, sizeof(full_path), "%s%s", serve_directory, path);

            int file_fd = open(full_path, O_RDONLY);
            if (file_fd < 0) {
                const char *response = "HTTP/1.0 404 Not Found\r\n\r\n";
                write(new_socket, response, strlen(response));
                close(new_socket);
                exit(0);
            }
            const char *ok_header = "HTTP/1.0 200 OK\r\n\r\n";
            write(new_socket, ok_header, strlen(ok_header));

            while ((bytesRead = read(file_fd, buffer, sizeof(buffer))) > 0) {
                write(new_socket, buffer, bytesRead);
            }

            close(file_fd);
            close(new_socket);
            exit(0); 
        }
        close(new_socket);
    }
}

