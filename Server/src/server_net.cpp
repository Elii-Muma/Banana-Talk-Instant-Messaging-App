#include "server_net.hpp"



MyServer::MyServer() : port(55555), cID(1000){

    //maybe ill change calling these here
    startServer();
}


int MyServer::startServer(){

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

int MyServer::createSocket(){

    /*------------------------------------------------------------------------------*/
    //WE THE CREATE THE SOCKET
    serverS = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serverS == INVALID_SOCKET){
        std::cout<<"Error at socket(): " <<WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }else{
        std::cout<<"socket() is OK!"<<std::endl;
        return 0;
    }

    return 69;

}

int MyServer::bindSocket(){

    /*
    set the options for the server socket
    Allows the server to bind to a socket address even if:
    The socket address is currently in use by a previous instance of the server 
    The socket address is in the TIME_WAIT state after a previous connection was closed.
    */
    int i = 1;
    setsockopt(serverS, SOL_SOCKET, SO_REUSEADDR, (char *)&i, sizeof(i));

    /*------------------------------------------------------------------------------*/
    //BIND THE SOCKET WITH IP ADDY AND PORT NUMBER
    //struct to specify a local remote endpoint addy to which to connect a socket
    //has family, port, and address
    service.sin_family = AF_INET;
    char ipAddress[16];
    std::cout <<"enter the server address" << std::endl;
    std::cin>>ipAddress;

    if(InetPtonA(AF_INET, ipAddress, &service.sin_addr.s_addr) != 1){
        printf("failed to create server on that address");
        return -1;
    }else{
        //htons converts the port nummber form host byte(if its in that format),
        //to the correct format(ive forgotten which it is, but big number first)
        service.sin_port = htons(port);
        if(bind(serverS, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR){
            std::cout<<"bind() failed! " << WSAGetLastError() << std::endl;
            closesocket(serverS);
            WSACleanup();
            return -1;
        }else{
            std::cout<<"bind() is OK!"<<std::endl;
            return 0;
        }

    }

    return 69;

}

int MyServer::listenIn(){
    /*------------------------------------------------------------------------------*/
    //NOW THE SOCKET WILL BE TRYING TO LISTEN FOR INCOMING CONNECTIONS
    //WE USE THE LISTEN FUNCTION
    /*takes socket and max number of connections*/
    int lMsg = listen(serverS, 5);

    unsigned long b = 1;
    ioctlsocket(serverS, FIONBIO, &b);

    if(lMsg == SOCKET_ERROR){
        std::cout<<"listen() failed! " << WSAGetLastError() << std::endl;
        return -1;

    }else{
        std::cout<<"listen() is OK!\nWaiting for connections..." << std::endl;
        return 0;
    }

    return 69;
}

SOCKET MyServer::acceptClient(){

    int len = sizeof(addr);
    return accept(serverS, (struct sockaddr*)& addr, &len);
}

sockaddr_in& MyServer::get_cAddr(){
    return addr;
}

char* MyServer::getNick(){
    return cp.name;
}

int MyServer::sendTo(SOCKET cs, char* msg){
    
    char msgBuffer[200];
    sprintf(msgBuffer, "%s", msg);
    return send(cs, msgBuffer, sizeof(msgBuffer), 0);
}

char* MyServer::receiveFrom(SOCKET cs){

    char rBuff[200];
    int recErr = recv(cs, (char*)&cp, sizeof(cp), 0); 
    std::sprintf(recvBuffer, "/n%s/m%s", cp.name, cp.message);

    if(recErr == 0){
        printf("connection closed.\n");
        return errBuffer;
    }else if(recErr > 0){
        return recvBuffer;
    }

    return errBuffer;
}