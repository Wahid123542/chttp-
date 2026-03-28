# chttp

A minimal HTTP/1.1 server built from scratch in C using raw POSIX TCP sockets — no frameworks, no libraries, just syscalls.

---

## Features

| Feature | Description |
|---|---|
| **TCP socket server** | Full connection lifecycle via `socket()`, `bind()`, `listen()`, `accept()` |
| **HTTP/1.1 parsing** | Parses `GET` requests and extracts method and path |
| **Static file serving** | Serves HTML, CSS, JS, PNG, and JPEG from a `www/` directory |
| **Content-type negotiation** | Automatically sets correct `Content-Type` header per file extension |
| **404 handling** | Returns a proper `404 Not Found` response for missing files |
| **Port reuse** | Uses `SO_REUSEADDR` to avoid "Address already in use" on restart |

---

## Getting Started

### Prerequisites

- GCC or Clang
- Linux or macOS (POSIX-compliant system)
- Make

### Build & Run

```bash
git clone https://github.com/Wahid123542/chttp.git
cd chttp
make
./chttp          # defaults to port 8080
./chttp 3000     # or specify a port
```

Then open `http://localhost:8080` in your browser.

---

## Project Structure

```
chttp/
├── src/
│   ├── main.c          # Entry point and argument parsing
│   ├── server.c        # Socket setup and accept loop
│   ├── server.h
│   ├── request.c       # HTTP request parser
│   ├── request.h
│   ├── response.c      # Response builder and file sender
│   └── response.h
├── www/
│   └── index.html      # Default served page
├── Makefile
└── README.md
```

---

## Syscall Trace

Every HTTP request follows this exact sequence of POSIX syscalls:

```
accept(server_fd, &addr, &len)   →  client_fd
read(client_fd, buffer, 4096)    →  raw HTTP request
parse_request()                  →  method + path
fopen("www/<path>", "rb")        →  file handle (or NULL → 404)
write(client_fd, header, ...)    →  HTTP response header
fread() + write()                →  response body in chunks
close(client_fd)                 →  connection closed
```

---

## How It Works

**Socket setup** — the server creates a TCP socket with `socket(AF_INET, SOCK_STREAM, 0)`, binds it to a port with `bind()`, and begins listening with `listen()`. The `SO_REUSEADDR` option is set so the server can restart immediately without waiting for the OS to release the port.

**Accept loop** — `accept()` blocks until a client connects, returning a new file descriptor for that connection. The raw HTTP request is read into a buffer with `read()`.

**Parsing** — the request line (`GET /path HTTP/1.1`) is extracted using `sscanf`, giving the method and path.

**Response** — the path is mapped to a file under `www/`. If found, the file size is computed with `fseek`/`ftell`, a `200 OK` header is written, and the file body is streamed in chunks. If not found, a `404` response is sent instead.

---

## Technical Concepts Demonstrated

- TCP connection lifecycle (`socket` → `bind` → `listen` → `accept` → `read`/`write` → `close`)
- HTTP/1.1 request parsing at the byte level
- File I/O with `fopen`, `fseek`, `fread`
- Content-type negotiation
- `SO_REUSEADDR` socket option
- Makefile-based C build system

---

## Roadmap

- [ ] Multi-client support via `fork()` per connection
- [ ] Non-blocking I/O with `select()`
- [ ] Request logging to file with timestamps
- [ ] `HEAD` and `POST` method support

---

## Tested On

- Ubuntu 22.04 (gcc 11)
- Compatible with the University of Arkansas turing server

---

## License

MIT
