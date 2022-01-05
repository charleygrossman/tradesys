#include <stdlib.h>
#include <stdio.h>
#include "feed/feed.h"
#include "feed/quote.h"

int main(void) {
    char *fpath = getenv("QUOTE_FILEPATH");
    if (fpath == NULL) {
        printf("must set QUOTE_FILEPATH\n");
        return EXIT_FAILURE;
    }
    struct Feed feed;
    if (Feed_new(&feed, fpath) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    return Feed_start(&feed);
}
