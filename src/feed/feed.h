#ifndef Feed_H
#define Feed_H

struct Feed{};

int Feed_new(struct Feed *, const char *rateFilepath);
int Feed_start(struct Feed *);

#endif