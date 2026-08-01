#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
typedef SOCKET socket_t;
#define CLOSE_SOCKET(s) closesocket(s)
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
typedef int socket_t;
#define CLOSE_SOCKET(s) close(s)
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif

static const char HTTP_RESPONSE[] = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: application/json\r\n"
    "Content-Length: 27\r\n"
    "Connection: keep-alive\r\n"
    "\r\n"
    "{\"message\":\"Hello, World!\"}";

int main(int argc, char** argv) {
    int port = 8081;
    if (argc > 1) port = atoi(argv[1]);

#if defined(_WIN32)
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    socket_t server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        perror("Socket creation failed");
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        perror("Bind failed");
        return 1;
    }

    if (listen(server_fd, 4096) == SOCKET_ERROR) {
        perror("Listen failed");
        return 1;
    }

    printf("Vito Server listening on port %d...\n", port);
    fflush(stdout);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        socket_t client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        if (client_fd == INVALID_SOCKET) continue;

        char buffer[2048];
        while (1) {
            int bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
            if (bytes_read <= 0) break;
            send(client_fd, HTTP_RESPONSE, (int)strlen(HTTP_RESPONSE), 0);
        }
        CLOSE_SOCKET(client_fd);
    }

#if defined(_WIN32)
    WSACleanup();
#endif
    return 0;
}
