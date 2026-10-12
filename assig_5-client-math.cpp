#include<iostream>
#include<string>
#include<cstring>
#include<winsock2.h>
#include<ws2tcpip.h>
#define port 8080

using namespace std;

// Structure to send data to server
struct OperationPacket {
    double num1;
    double num2;
    char op;
};

class CalculatorClient
{
public:
    void sendRequest(SOCKET sockfd, OperationPacket pkt)
    {
        send(sockfd, (char*)&pkt, sizeof(pkt), 0);
    }

    void receiveResult(SOCKET sockfd, double &result)
    {
        recv(sockfd, (char*)&result, sizeof(result), 0);
    }
};

int main()
{
    // Step 1: Initialize winsock using WSAStartup
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa); 

    // Step 2: Create a socket
    SOCKET sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sockfd == INVALID_SOCKET)
    {
        cout<<"\nSocket creation failed";
        return 1;
    }

    // Step 3: Declare server address
    sockaddr_in serverAddr;
    serverAddr = {};

    // Step 4: Configure server's IP address settings and port number
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost
    serverAddr.sin_port = htons(port);

    // Step 5: Connect the socket with IP address and port no
    if (connect(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        cout<<"\nConnection failed";
        return 1;
    }
    cout<<"\nConnected to Server successfully!";

    // Step 6: Mathematical Operations Interaction
    CalculatorClient calc;
    OperationPacket pkt;

    while(true)
    {
        cout<<"\n\n--- Calculator Menu ---";
        cout<<"\nEnter first number: ";
        cin >> pkt.num1;
        
        cout<<"Enter operator (+, -, *, /) or '#' to exit: ";
        cin >> pkt.op;

        if (pkt.op == '#')
        {
            calc.sendRequest(sockfd, pkt);
            cout<<"\nClient ended the communication!";
            break;
        }

        cout<<"Enter second number: ";
        cin >> pkt.num2;

        calc.sendRequest(sockfd, pkt);

        double result;
        calc.receiveResult(sockfd, result);
        cout<<"\nServer Response -> Result: " << result;
    }

    // Cleanup
    closesocket(sockfd);
    WSACleanup();
    return 0;
}
