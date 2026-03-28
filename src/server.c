#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "request.h"
#include "response.h"

#define BACKLOG   10
#define BUF_SIZE  4096

void run_server(int port) {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    char buffer[BUF_SIZE];

    // 1. Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); exit(1); }

    // Allow port reuse (avoids "Address already in use" on restart)
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 2. Bind to port
    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(port);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind"); exit(1);
    }

    // 3. Listen for connections
    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen"); exit(1);
    }

    printf("chttp listening on http://localhost:%d\n", port);

    // 4. Accept loop
    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
        if (client_fd < 0) { perror("accept"); continue; }

        // Read HTTP request
        memset(buffer, 0, BUF_SIZE);
        read(client_fd, buffer, BUF_SIZE - 1);

        // Parse and respond
        HttpRequest req;
        if (parse_request(buffer, &req) == 0) {
            printf("[%s] %s\n", req.method, req.path);
            send_response(client_fd, req.path);
        }

        close(client_fd);
    }

    close(server_fd);
}
