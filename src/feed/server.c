#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "feed/quote.h"

struct Server {
    const char *address, *port;
    struct addrinfo *info;
    int fd;
};

int Server_start(struct Server *server, const char *address, const char *port) {
    memset(server, 0, sizeof(struct Server));

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_DGRAM;

    struct addrinfo *info;
    if (getaddrinfo(address, port, &hints, &info) != 0) {
        return EXIT_FAILURE;
    }
    int fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    if (fd == -1) {
        return EXIT_FAILURE;
    }
    server->address = address;
    server->port = port;
    server->info = info;
    server->fd = fd;

    return EXIT_SUCCESS;
}

int Server_sendQuote(struct Server *server, struct Quote quote) {
    unsigned char *data;
    data = (unsigned char*)malloc(sizeof(struct Quote));
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    marshalQuote(data, &quote);
    if (sendto(server->fd, data, sizeof(struct Quote), 0, server->info->ai_addr, server->info->ai_addrlen) == -1) {
        free(data);
        return EXIT_FAILURE; 
    }
    free(data);
    return EXIT_SUCCESS;
}