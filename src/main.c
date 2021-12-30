#include <stdlib.h>
#include <stdio.h>
#include "feed/feed.h"
#include "feed/rate.h"

int main(void) {
    char *fpath = getenv("RATE_FILEPATH");
    if (fpath == NULL) {
        printf("must set RATE_FILEPATH\n");
        return EXIT_FAILURE;
    }
    struct Feed feed;
    if (Feed_new(&feed, fpath) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    return Feed_start(&feed);
}
