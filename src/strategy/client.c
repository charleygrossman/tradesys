#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "feed/quote.h"

struct Client {
    char *port;
    struct addrinfo **info;
    int fd;
};

int Client_start(struct Client *client, char *port) {
    memset(client, 0, sizeof(struct Client));
    client->port = port;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_DGRAM;
    struct addrinfo *info;
    client->info = &info;
    if (getaddrinfo(NULL, client->port, &hints, client->info) != 0) {
        return EXIT_FAILURE;
    }
    int fd = socket(client->info[0]->ai_family, client->info[0]->ai_socktype, client->info[0]->ai_protocol);
    if (fd == -1) {
        return EXIT_FAILURE;
    }
    client->fd = fd;
    bool reuseAddr = true;
    if (setsockopt(client->fd, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(int)) != 0) {
        return EXIT_FAILURE;
    }
    if (bind(client->fd, client->info[0]->ai_addr, client->info[0]->ai_addrlen) != 0) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int Client_recvQuote(struct Client *client, struct Quote *quote) {
    size_t size = sizeof(struct Quote);
    unsigned char data[size];
    if (recvfrom(client->fd, data, size-1, 0, client->info[0]->ai_addr, &client->info[0]->ai_addrlen) == -1) {
        return EXIT_FAILURE;
    }
    memset(quote, 0, sizeof(struct Quote));
    unmarshalQuote(quote, data);
    return EXIT_SUCCESS;
}