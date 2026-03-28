#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "response.h"

#define WWW_ROOT "./www"
#define BUFFER_SIZE 4096

static const char *get_content_type(const char *path) {
    if (strstr(path, ".html")) return "text/html";
    if (strstr(path, ".css"))  return "text/css";
    if (strstr(path, ".js"))   return "application/javascript";
    if (strstr(path, ".png"))  return "image/png";
    if (strstr(path, ".jpg"))  return "image/jpeg";
    return "text/plain";
}

void send_response(int client_fd, const char *path) {
    char filepath[1024];

    // Default to index.html for root
    if (strcmp(path, "/") == 0)
        snprintf(filepath, sizeof(filepath), "%s/index.html", WWW_ROOT);
    else
        snprintf(filepath, sizeof(filepath), "%s%s", WWW_ROOT, path);

    FILE *f = fopen(filepath, "rb");

    if (!f) {
        // 404
        const char *body = "<html><body><h1>404 Not Found</h1></body></html>";
        char header[256];
        snprintf(header, sizeof(header),
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %zu\r\n"
            "Connection: close\r\n\r\n",
            strlen(body));
        write(client_fd, header, strlen(header));
        write(client_fd, body, strlen(body));
        return;
    }

    // Get file size
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    // Send 200 header
    char header[256];
    snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n"
        "Connection: close\r\n\r\n",
        get_content_type(filepath), size);
    write(client_fd, header, strlen(header));

    // Send file body in chunks
    char buf[BUFFER_SIZE];
    size_t bytes;
    while ((bytes = fread(buf, 1, sizeof(buf), f)) > 0)
        write(client_fd, buf, bytes);

    fclose(f);
}
