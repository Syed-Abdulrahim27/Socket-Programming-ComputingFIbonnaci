#include <iostream>
#include<stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
using namespace std;
#include <winsock2.h>
int main()
{
    // Define a structure to hold Winsock data
    WSADATA wsData;
    sockaddr_in serverAddr;


    // Initialize Winsock with version 2.2
    int nret = WSAStartup(MAKEWORD(2, 2), &wsData);
    if (nret < 0)
    {
        cout << "Failed to initialize Winsock" << endl;
        return -1;
    }

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);//open a socket listener

    // Check if the server socket creation was successful
    if (serverSocket < 0)
    {
        cout << "Failed to initialize listener socket" << endl;
        WSACleanup(); // Clean up Winsock
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("172.16.20.217");
    serverAddr.sin_port = htons(12345);
    memset(&(serverAddr.sin_zero), 0, 8);

    nret = connect(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (nret < 0)
    {
        cout << "Cannot connect to server!" << endl;
        return -1;
    }
    char buffer[1024];
    recv(serverSocket, buffer, sizeof(buffer), 0);
    cout << "Server says: " << buffer << endl;
    while (true)
    {
        memset(buffer, 0, 1024);
        cout << "Enter a value to find that fibonnaci number:" << endl;
        memset(buffer, 0, 1024);
        fgets(buffer, 1023, stdin);
        send(serverSocket, buffer, strlen(buffer) - 1, 0);
        recv(serverSocket, buffer, sizeof(buffer), 0);
        cout << "Server says: " << buffer << endl;
    }
}
