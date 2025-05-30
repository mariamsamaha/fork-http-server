#include "server.h"
#include <stdio.h>
#include <stdlib.h>
char *global_directory = NULL;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <port> <directory>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    global_directory = argv[2];

    struct Server server = server_Constructor(AF_INET, port, SOCK_STREAM, 0, 10, INADDR_ANY, launch);
    server.launch(&server);
    return 0;
}