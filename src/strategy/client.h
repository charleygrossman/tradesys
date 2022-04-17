#ifndef Client_H
#define Client_H

struct Client{};

int Client_start(struct Client *, char *port);

int Client_recvQuote(struct Client *, struct Quote *quote);

#endif