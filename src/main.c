#include <stdio.h>
#include <stdlib.h>
#include "server.h"

#define DEFAULT_PORT 8080

int main(int argc, char *argv[]) {
    int port = DEFAULT_PORT;

    if (argc == 2)
        port = atoi(argv[1]);

    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Usage: %s [port]\n", argv[0]);
        return 1;
    }

    run_server(port);
    return 0;
}
