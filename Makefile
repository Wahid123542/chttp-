CC = gcc
CFLAGS = -Wall -Wextra -g
SRC = src/main.c src/server.c src/request.c src/response.c
TARGET = chttp

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
