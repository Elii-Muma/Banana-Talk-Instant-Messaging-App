#pragma once
#include <cstring>
#include <string>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <thread>
#include <atomic>
#include <vector>

class MyClient{
    /*CLASS DOES ALL THE SERVER FUNCTIONALITIES*/
    public:

        //PROTOCOLS
        struct chat_protocol{
            char id[2];
            char name[10];
            char message[200];
        };
        
        chat_protocol cp;
        chat_protocol cp_recv;
        std::atomic<bool> isName{false};

        MyClient(SOCKET &s);
        //void setChat_Protocol(ChatProtocol &cp);
        int startServer();
        int createAndBind(char* ip);
        void setNick(char* name);
        char* getNickName();
        void setSenderNick(std::string name);
        char *getSenderNick();
        int sendTo(char *msg);
        int sendPlainMsg(char * plainM);
        std::vector<std::string> getClientNames();
        bool getIsName();
        bool getIsNewConnection();
        char *receiveFrom();

    private:

        std::vector<std::string> clientNames;
        WSAData wsaData;
        SOCKET clientS;
//        ChatProtocol chatPrt;
        int port;
        int cID;

        char recvBuffer[200];
        char errBuffer[100]{"ERR_RECV"};
        char recErrBuffer[100]{"ERR_REC_OUT"};
        


};