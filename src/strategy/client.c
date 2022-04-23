#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "feed/quote.h"

struct Client {
    const char *address, *port;
    struct addrinfo *info;
    int fd;
};

int Client_start(struct Client *client, const char *address, const char *port) {
    memset(client, 0, sizeof(struct Client));

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
    bool reuseAddr = true;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(int)) != 0) {
        return EXIT_FAILURE;
    }
    if (bind(fd, info->ai_addr, info->ai_addrlen) != 0) {
        return EXIT_FAILURE;
    }
    client->address = address;
    client->port = port;
    client->info = info;
    client->fd = fd;

    return EXIT_SUCCESS;
}

int Client_recvQuote(struct Client *client, struct Quote *quote) {
    size_t size = sizeof(struct Quote);
    unsigned char data[size];
    if (recvfrom(client->fd, data, size-1, 0, client->info->ai_addr, &client->info->ai_addrlen) == -1) {
        return EXIT_FAILURE;
    }
    memset(quote, 0, sizeof(struct Quote));
    unmarshalQuote(quote, data);
    return EXIT_SUCCESS;
}