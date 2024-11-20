#include "server_net.hpp"
#include <unordered_map>

#define MAX_CLIENTS 10;

//CODE TO HAVE THE SERVER RECEIVE A MESSAGE

struct clients_tb{
    int ID;
    char name[10];
    bool connected;
    SOCKET ss;
    sockaddr_in addr;
};

struct chat_protocol{
    char id[2];
    char name[10];
    char message[200];
};

clients_tb *newClient;
std::vector<clients_tb> cclients;
std::vector<std::string> cNames; 


int connected_clients, clientID, recvdData;

int main(){

    MyServer server;


    if(server.createSocket() == 0){
        int loop = -1;
        do{
            loop = server.bindSocket();
            if(loop != 0){
                printf("\ntry again\n\n");
            }
        }while(loop != 0);
    }
    server.listenIn();

    SOCKET acceptSocket;
    char recvBuffer[200];
    char message[200];
    char confBuffer[200] = "/csent";
    clientID = 1000;

    char pingBuffer[200] = "/cping";
    std::thread pingThread([&pingBuffer, &server](){
        /*constantly ping the user to check if they're alive*/
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::cout<<"started ping thread!"<<std::endl;

            while (1){

                if(!cclients.empty()){

                    std::this_thread::sleep_for(std::chrono::milliseconds(500));

                    //send message if it errors, remove that bastard from the list
                    for(int i=0; i<cclients.size(); i++){

                        if(cclients[i].connected){

                            int bCount = server.sendTo(cclients[i].ss, pingBuffer);

                            if(bCount == SOCKET_ERROR){

                                std::cout<< cclients[i].name <<" has disconnected. err: " << WSAGetLastError() <<std::endl;
                                cclients.erase(cclients.begin()+i);
                                connected_clients--;
                                // printf("Client failed to send %ld.\n", WSAGetLastError);

                            }

                        }
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(50));        
            }

    });




    //TRY TO MAKE THE CHATTING CONSTANT UNTIL YOU TYPE "LETMEOUT"
    bool exit = false;
    const char *exitMessage = "dc_me";

    while(!exit){

        acceptSocket = server.acceptClient();

        if(acceptSocket != INVALID_SOCKET){

            newClient = new clients_tb;
            newClient->connected = true;
            newClient->ss = acceptSocket;
            newClient->ID = clientID;
            newClient->addr = server.get_cAddr();
            cclients.push_back(*newClient);

            // clients[connected_clients].connected = true;
            // clients[connected_clients].ss = acceptSocket;
            // clients[connected_clients].ID = clientID;

            clientID++;
            connected_clients++;

            std::cout<< "\nnew client connected!" << std::endl;
            std::cout<< "connected clients: " << cclients.size() << std::endl;

        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));        

        if(connected_clients > 0){

            for(int cpos=0; cpos<connected_clients; cpos++){

                //something with the receiving buffer
                if(cclients[cpos].connected){

                    sprintf(recvBuffer, "%s", server.receiveFrom(cclients[cpos].ss));
                    int recvID = cclients[cpos].ID;

                    std::string text(recvBuffer);
                    char name[10];

                    for(int k=0; k<text.length(); k++){
                        if(std::string(recvBuffer).substr(k, 2) == "/N"){
                            //theyve sent over theire name and now you can send it over to everyone
                            //first store the name in a container, iterate through the container appending some string
                            //convert that string to some array, send over that array to the peoples
                            // '/l' for list
                            char nameList[16];
                            sprintf(cclients[cpos].name, "%s", text.substr(k+2).c_str());
                            std::cout<<"connected client: "<< cclients[cpos].name << "\n" <<std::endl;

                            //send all the connected clients names to the one new client only

                            for(int j=0; j<connected_clients; j++){
                                if(cclients[j].connected){
                                    sprintf(nameList, "/i%d/l%s", cclients[j].ID, cclients[j].name);
                                    for(int i=0; i<connected_clients; i++){
                                        server.sendTo(cclients[i].ss, nameList); 
                                    }
                                }
                            }
                            break;
                        }
                    }

                    if(std::string(recvBuffer) == exitMessage){

                        printf("client %s has disconnected.\n", cclients[cpos].name);
                        
                        cclients[cpos].connected = false;
                        closesocket(cclients[cpos].ss);
                        cclients.erase(cclients.begin()+cpos);
                        connected_clients--;
                        
                    }else if(std::string(recvBuffer) != "ERR_RECV"){

                        //SEND DATA RECEIVED TO EVERYONE
                        for(int j=0; j<connected_clients; j++){

                            if(cclients[j].connected && (j != cpos)){

                                std::sprintf(message, "%s", recvBuffer);

                                int bCount = server.sendTo(cclients[j].ss, message);

                                if(bCount == SOCKET_ERROR){

                                    std::cout<<"server failed to send to "<< cclients[j].name <<" with err: " << WSAGetLastError() <<std::endl;
                                    cclients[cpos].connected = false;
                                    connected_clients--;
                                    // printf("Client failed to send %ld.\n", WSAGetLastError);

                                }else{

                                    //printf("Server sent: %ld bytes.\n", bCount); 
                                    printf("sent conf to %s\n\n", cclients[cpos].name);

                                }

                            }else{

                                //only send a delivered message to the sender
                                if(server.sendTo(cclients[j].ss, confBuffer) == SOCKET_ERROR){

                                    std::cout<<"server failed to send to "<< cclients[j].name<<" with err: " << WSAGetLastError() <<std::endl;
                                    cclients[j].connected = false;
                                    connected_clients--;

                                }

                            }
                        }

                        std::cout<<recvBuffer<<std::endl;
                    }
                } 
            } 
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));        

    }


    pingThread.join();
    //closesocket(serverSocket);
    system("pause");
    WSACleanup();

    return 0;
}