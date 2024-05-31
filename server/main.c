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
    char serv_addr[] = "192.168.0.1";
    int sock, client_sock;
    char buffer[1024];
    char response[18384];
    struct sockaddr_in server_address, client_address, target_address;
    int i=0;
    int optval = 1;
    socklen_t client_length;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        printf("Error Setting TCP Socket Options!\n");
        return 1;
    }
    
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(serv_addr);
    server_address.sin_port = htons(50004);
    if(bind(sock, (struct  sockaddr *) &server_address, sizeof(server_address)) < 0) {
        printf("Error binding socket!\n");
        return 1;
    }
    
    listen(sock, 5);
    client_length = sizeof(client_address);
    client_sock = accept(sock, (struct sockaddr *) &client_address, &client_length);
}
