#include<iostream>
#include<string>
#include<cstring>
#include<winsock2.h>
#include<ws2tcpip.h>
#define port 8080

using namespace std;

// Structure to hold data received from client
struct OperationPacket {
    double num1;
    double num2;
    char op; // Operator: '+', '-', '*', '/'
};

class CalculatorServer
{
public:
    void receiveRequest(SOCKET sockfd, OperationPacket &pkt)
    {
        recv(sockfd, (char*)&pkt, sizeof(pkt), 0);
    }

    void sendResult(SOCKET sockfd, double result)
    {
        send(sockfd, (char*)&result, sizeof(result), 0);
    }

    double calculate(double a, double b, char op)
    {
        switch(op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': 
                if(b != 0) return a / b;
                else return 0; // Division by zero handling
            default: return 0;
        }
    }
};

int main()
{
    // Step 1: Initialize winsock using WSAStartup
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa); 

    // Step 2: Create a socket
    SOCKET sockfd, connfd;
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sockfd == INVALID_SOCKET)
    {
        cout<<"\nSocket creation failed";
        return 1;
    }

    // Step 3: Declare server and client addresses
    sockaddr_in serverAddr, clientAddr;
    serverAddr = {};
    clientAddr = {};

    // Step 4: Configure server's IP address settings and port number
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    // Step 5: Bind the socket with IP address and port no
    if (bind(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        cout<<"\nBind failed";
        return 1;
    }

    // Step 6: Listen for connection from client
    if (listen(sockfd, 5) == SOCKET_ERROR)
    {
        cout<<"\nListen failed";
        return 1;
    }
    cout<<"\nServer started...";
    cout<<"\nWaiting for the client...";

    // Step 7: Accept the client connection
    int len = sizeof(clientAddr);
    connfd = accept(sockfd, (sockaddr*)&clientAddr, &len);
    
    if (connfd == INVALID_SOCKET)
    {
        cout<<"\nAccept failed";
        return 1;
    }

    cout<<"\nClient connected!";

    // Step 8: Perform Mathematical Operations
    CalculatorServer calc;
    OperationPacket pkt;

    while(true)
    {
        calc.receiveRequest(connfd, pkt);
        
        // Check if client wants to exit (e.g., operator '#')
        if (pkt.op == '#')
        {
            cout<<"\nClient ended communication!";
            break;
        }

        cout<<"\nReceived: " << pkt.num1 << " " << pkt.op << " " << pkt.num2;
        
        double result = calc.calculate(pkt.num1, pkt.num2, pkt.op);
        cout << "\nCalculated Result: " << result;

        calc.sendResult(connfd, result);
    }

    // Cleanup
    closesocket(connfd);
    closesocket(sockfd);
    WSACleanup();
    return 0;
}
