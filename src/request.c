#include <stdio.h>
#include <string.h>
#include "request.h"

int parse_request(const char *raw, HttpRequest *req) {
    // Expects: "GET /path HTTP/1.1\r\n..."
    int matched = sscanf(raw, "%15s %1023s", req->method, req->path);
    return matched == 2 ? 0 : -1;
}
