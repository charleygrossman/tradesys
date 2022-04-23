#ifndef Client_H
#define Client_H

struct Client {
    const char *address, *port;
    struct addrinfo *info;
    int fd;
};

int Client_start(struct Client *, const char *address, const char *port);

int Client_recvQuote(struct Client *, struct Quote *quote);

#endif