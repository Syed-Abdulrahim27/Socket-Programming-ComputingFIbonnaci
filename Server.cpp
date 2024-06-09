#include <iostream>
#include <winsock2.h>
#include<vector>
#include<string>
#include<queue>
#include<fstream>
using namespace std;
const int MAX_CLIENTS = 10;

int fibonacci(int n);

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

    int Serversocket = socket(AF_INET, SOCK_STREAM, 0);//open a socket listener

    // Check if the server socket creation was successful
    if (Serversocket < 0)
    {
        std::cerr << "Failed to initialize listener socket" << endl;
        WSACleanup(); // Clean up Winsock
        return -1;
    }


    // Create a sockaddr_in structure to define server details 
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Bind to any available network interface
    serverAddr.sin_port = htons(12345); // Bind to port 12345
    memset(&(serverAddr.sin_zero), 0, 8);

    //Bind the socket to the specified address and port
    nret = bind(Serversocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (nret < 0)
    {
        std::cerr << "Bind failed" << std::endl;
        //closesocket(serverSocket); // Close the socket 
        WSACleanup(); // Clean up Winsock
        return -1;
    }

    //Start listening for incoming connections with a maximum backlog queue size
    nret = listen(Serversocket, 5);
    if (nret < 0)
    {
        cout << "Listen failed" << endl;
        //closesocket(serverSocket); // Close the socket
        WSACleanup(); // Clean up Winsock
        return -1;
    }
    else
        cout << "Server is listening on port 12345..." << endl;

    //accepting client request
    wprintf(L"Waiting for client to connect...\n");
    queue<SOCKET> clientsockets;
    while (true)
    {
        SOCKADDR ClientAddr;
        int Clientaddrlen = sizeof(ClientAddr);
        SOCKET Clientsocket = accept(Serversocket, (struct sockaddr*)&ClientAddr, &Clientaddrlen);
        if (Clientsocket == INVALID_SOCKET)
        {
            cout << "accept() failed: " << endl;
            //closesocket(serverSocket);
            WSACleanup();
            return -1;
        }
        cout << "Client connected" << endl;
        const char* responseMessage = "Hello from the server";
        int sendResult = send(Clientsocket, responseMessage, strlen(responseMessage), 0);
        if (sendResult == SOCKET_ERROR) {
            cout << "send() failed: " << endl;
        }
        else {
            cout << "Hello message sent" << endl;
        }
        char buffer[1024];
        memset(buffer, 0, 1024);
        clientsockets.push(Clientsocket);
        int bytesReceived = recv(Clientsocket,buffer, sizeof(buffer) - 1, 0);
        int n = atoi(buffer);
        string str = to_string(fibonacci(n));
        const char* charPtr = str.c_str();
        send(clientsockets.front(), charPtr, sizeof(buffer) - 1, 0);
        clientsockets.pop();
    }
}
int fibonacci(int n) {
    if (n <= 1) 
    {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}