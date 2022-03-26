#ifndef Server_H
#define Server_H

struct Server{};

int Server_start(struct Server *, char *port);

int Server_sendQuote(struct Server *, struct Quote quote);

#endif