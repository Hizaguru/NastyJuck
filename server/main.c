#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int sock, client_sock;
    char buffer[1024];
    char response[18384];
    struct sockaddr_in server_address, client_address, target_address;
    int i=0;
    int optval = 1;
    socklen_t client_length;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error creating socket");
        return 1;
    }

    // Set socket options
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("Error setting TCP socket options");
        close(sock);
        return 1;
    }
    
    // Configure server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Changed to loopback address for testing
    server_address.sin_port = htons(50004);

    // Bind socket
    if (bind(sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("Error binding socket");
        close(sock);
        return 1;
    }
    
    // Listen on socket
    if (listen(sock, 5) < 0) {
        perror("Error listening on socket");
        close(sock);
        return 1;
    }

    // Accept client connections
    client_length = sizeof(client_address);
    client_sock = accept(sock, (struct sockaddr *) &client_address, &client_length);
    if (client_sock < 0) {
        perror("Error accepting connection");
        close(sock);
        return 1;
    }

    // Print client connection details
    printf("Connection accepted from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

    // Close sockets
    close(client_sock);
    close(sock);

    return 0;
}
