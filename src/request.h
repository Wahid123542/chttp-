#ifndef REQUEST_H
#define REQUEST_H

#define MAX_PATH 1024
#define MAX_METHOD 16

typedef struct {
    char method[MAX_METHOD];
    char path[MAX_PATH];
} HttpRequest;

int parse_request(const char *raw, HttpRequest *req);

#endif
