#ifndef Server_H
#define Server_H

struct Server {
    const char *address, *port;
    struct addrinfo *info;
    int fd;
};

int Server_start(struct Server *, const char *address, const char *port);

int Server_sendQuote(struct Server *, struct Quote quote);

#endif