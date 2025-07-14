#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/vldwm-socket"

int main(int argc, char *argv[]) {
    int sock_fd;
    struct sockaddr_un addr;
    
    // Create socket
    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        return 1;
    }
    
    // Connect to server
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
    
    if (connect(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(sock_fd);
        return 1;
    }
    
    printf("Connected to VLDWM display server\n");
    
    // Simple message exchange
    const char *message = "Hello from client!";
    if (send(sock_fd, message, strlen(message), 0) < 0) {
        perror("send");
    }
    
    close(sock_fd);
    return 0;
}
