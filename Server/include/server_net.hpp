#pragma once
#include <cstring>
#include <string>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <thread>
#include <vector>

class MyServer{
    /*CLASS DOES ALL THE SERVER FUNCTIONALITIES*/
    public:

        //PROTOCOLS
        struct chat_protocol{
            char id[2];
            char name[10];
            char message[200];
        };
        chat_protocol cp;

        MyServer();
        int startServer();
        int createSocket();
        int bindSocket();
        int listenIn();
        char* getNick();
        SOCKET acceptClient();
        sockaddr_in& get_cAddr();
        int sendTo(SOCKET cs, char* msg);
        char* receiveFrom(SOCKET cs);
    private:
        WSAData wsaData;
        SOCKET serverS, acceptS;
        int port;
        int cID;
        char confBuffer[200]{"sent"};
        sockaddr_in service;
        sockaddr_in addr;

        char recvBuffer[200];
        char errBuffer[100]{"ERR_RECV"};


};