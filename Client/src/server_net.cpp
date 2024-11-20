#include "server_net.hpp"
#include "MyFrame.h"


MyClient::MyClient(SOCKET &s) : port(55555), cID(1000), clientS(s){

    //maybe ill change calling these here
    startServer();

}

int MyClient::startServer(){

    int wsaerr;
    WORD wxVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wxVersionRequested, &wsaData);
    if(wsaerr != 0){
        std::cout<< "The Winsock dll not found" << std::endl;
        return -1;
    }else{
        printf("The winsock dll found!\n");
        std::cout<<"The Status: " << wsaData.szSystemStatus <<std::endl;
        return 0;
    }

    return 69;
}

int MyClient::createAndBind(char* ip){

    clientS = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clientS == INVALID_SOCKET){
        std::cout<<"Error at socket(): " <<WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }else{
        std::cout<<"socket() is OK!"<<std::endl;
    }

    //BIND THE SOCKET WITH IP ADDY AND PORT NUMBER
    //struct to specify a server to connect to's addy and port 
    //has family, port, and address
    sockaddr_in service;
    service.sin_family = AF_INET;
    char IpAddress[16];
    sprintf(IpAddress, "%s", ip);

    if(std::string(IpAddress) == "localhost"){
        char localhost[16] = "127.0.0.1";
        sprintf(IpAddress, "%s", localhost);
    }

    if(InetPtonA(AF_INET, IpAddress, &service.sin_addr.s_addr) != 1){
        printf("invalid IpAddy type!\n");
        return -1;
    }else{
        service.sin_port = htons(port);

        int conCheck = connect(clientS, (SOCKADDR*)&service, sizeof(service)); 
        if(conCheck == SOCKET_ERROR){
            std::cout<<"client Connect() failed to connect! " << std::endl;
            return -1;
        }else{
            std::cout<<"connect() is OK!"<<std::endl;
            std::cout<<"Client can start sending and receiving data!\n\n"<<std::endl;
            return 7;
        }
    }


    return 69;

}


void MyClient::setNick(char* name){
    sprintf(cp.name, "%s", name);
}

char* MyClient::getNickName(){
    return cp.name;
}

void MyClient::setSenderNick(std::string name){
    sprintf(cp_recv.name, "%s", name.c_str());
}

char* MyClient::getSenderNick(){
    return cp_recv.name;
}

// void MyClient::setChat_Protocol(ChatProtocol &cp){
//     this->chatPrt = cp;
//     chatPrt.getName();
// }

int MyClient::sendTo(char* msg){

    // sprintf(cpPtr->message, "%s", msg);
    sprintf(cp.message, "%s", msg);
    return send(clientS, (char*)&cp, sizeof(cp), 0);
}

int MyClient::sendPlainMsg(char* plainM){


    // sprintf(cpPtr->message, "%s", msg);
    char msg[200];
    sprintf(msg, "%s", plainM);
    return send(clientS, msg, sizeof(msg), 0);

}

std::vector<std::string> MyClient::getClientNames(){
   return clientNames;
}

bool MyClient::getIsName(){
    return isName;
}

char* MyClient::receiveFrom(){

    char rBuff[200];

    int recErr = recv(clientS, rBuff, 200, 0); 

    std::string cipher(rBuff);
    std::string _nSym = "/n";
    std::string _mSym = "/m";
    std::string _cSym = "/c";
    std::string _lSym = "/i";
    std::string _NSym = "/N";
    bool isCheck = false;

    int nn = 0, ms = 0;
    for(int i = 0; i<cipher.length(); i++){

        if((cipher.substr(i,2) == _NSym) || cipher.substr(i,2) == _cSym){
            isCheck = true;
            break;
        }

        if(cipher.substr(i,2) == _nSym){
            nn = i;
        }

        if(cipher.substr(i,2) == _mSym){
            ms = i;
            break;
        }

        if(cipher.substr(i,2) == _lSym){
            isName = true;
            break;
        }

    }

    if(!isCheck){
        if(!isName){
            std::string nName(cipher.substr(nn+2, ms-2));
            setSenderNick(nName);
            std::string recvdMsg(cipher.substr(ms+2));

            sprintf(recvBuffer, "%s", recvdMsg.c_str());

            if(recErr == 0){
                return errBuffer;
            }else if(recErr > 0){
                return recvBuffer;
            }else if(recErr < 0){
                return recErrBuffer;
            }

        }else{
            //its a name being sent over
            // std::string nID(cipher.substr(2, 4));
            // std::string nName(cipher.substr(8));
            // clientNames.push_back(nName);
            sprintf(recvBuffer, "%s", rBuff);

            if(recErr == 0){
                return errBuffer;
            }else if(recErr > 0){
                return recvBuffer;
            }else if(recErr < 0){
                return recErrBuffer;
            }
        }

        std::cout<<"exiting recv"<<std::endl;
    }
    // else{
    //     printf("\nserver failed with err: %ld.\n", WSAGetLastError());
    //     return errBuffer;
    // }

    return errBuffer;
}