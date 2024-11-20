#include "MyFrame.h"

MyFrame::MyFrame(wxApp &app)
    : wxFrame(nullptr, wxID_ANY, "Banana Talk", wxDefaultPosition, wxSize(500, 600)), 
    cNtwrk(new MyClient(clientSocket))
{   
    myApp = &app;

    //menu stuff
    menuItems();

    //the parent panel to the two panels
    book = new wxSimplebook(this, wxID_ANY);

    lpanel = new LoginP(book);
    Bind(wxEVT_BUTTON, &MyFrame::OnB1, this, ID_B1);
    hpanel = new HomeP(book);
    Bind(wxEVT_BUTTON, &MyFrame::OnB2, this, ID_LOGOUTB);
    Bind(wxEVT_BUTTON, &MyFrame::OnSendB, this, ID_SENDB);

    book->AddPage(lpanel, "login");
    book->AddPage(hpanel, "home");

    book->SetSelection(0);

    this->Center();

    //UI LOADED
    //DO NETWORKSTUFF

}

MyFrame::~MyFrame(){
    closesocket(clientSocket);
    WSACleanup();
}

void MyFrame::OnB1(wxCommandEvent& event){

    if( (!lpanel->emailCtrl->GetValue().IsEmpty()) && 
        (!lpanel->passCtrl->GetValue().IsEmpty()) && 
        (!lpanel->hostCtrl->GetValue().IsEmpty())){

        std::string name(lpanel->getName());
        
        printf("name to find %s\n\n", name.c_str());

        if(name.length() > 10){
            wxMessageBox("name too long!", "LONG AHH NAME", 
                            wxOK | wxICON_ERROR);
        }else if(cNtwrk->createAndBind(lpanel->getAddress()) != 7){
            wxMessageBox("failed to connect!", "CONNECTION ERR", 
                            wxOK | wxICON_ERROR);
        }else{
            char nameBuffer[12];
            cNtwrk->setNick(lpanel->getName());
            hpanel->setNickName(lpanel->getName());
            //when you press connect, we send the nickname you entered to the server
            //the server will then store this name and send it to everyone including the number of people connected
            //OR it could store the names in some container and the send over the container?...yes
            //OR store the names in a container BUT send the names as a long ass string with '\m' between each name
            sprintf(nameBuffer, "/N%s", lpanel->getName());
            cNtwrk->sendTo(nameBuffer);
            recvMsg();
            book->SetSelection(1);
            this->SetStatusText(wxString(lpanel->getName()).append("'s Chat'"));
        }
    }
}


void MyFrame::OnB2(wxCommandEvent& event){
    //disconnect from the server
    hpanel->reset();
    closesocket(clientSocket);
    WSACleanup();
    this->Close();
    book->SetSelection(0);
}

void MyFrame::OnSendB(wxCommandEvent& event){
    if(!hpanel->inputTxt->GetValue().IsEmpty()){
        sendMsg();
    }
}

///NETWORK THREADS////
void MyFrame::recvMsg(){
    tPool.enqueue([this]{
        //this received message will need to be displayed on the gui
        char recvBuffer[200];

        do{
            //echo back response
            sprintf(recvBuffer, "%s", cNtwrk->receiveFrom());
            if(std::string(recvBuffer) != "ERR_RECV" && (std::string(recvBuffer) != "ERR_REC_OUT")){
                if(std::string(recvBuffer) != "ping" && std::string(recvBuffer) != "sent"){

                    if(cNtwrk->getIsName()){
                            printf("%s has joined the chat\n", recvBuffer);

                            std::string cipher(recvBuffer);
                            std::string nID = cipher.substr(2, 4);
                            std::string nName = cipher.substr(8);

                            //checking for ID existance cos the list is sent over multiple times with increments
                            if(!cNames.empty() && (cNames.find(nID) == cNames.end())){
                                std::cout << "adding ID: " << nID << " name: " << nName << " to the list" << std::endl;

                                cNames[nID] = nName;

                                myApp->CallAfter([this, &nName]{
                                    hpanel->addMember(wxString(nName));
                                });

                            }else if(cNames.empty()){

                                std::cout << "adding ID: " << nID << " name: " << nName << " to the list" << std::endl;
                                cNames[nID] = nName;
                                myApp->CallAfter([this, &nName]{
                                    hpanel->addMember(wxString(nName));
                                });
                            }else{
                                printf("already exists");
                            }

                            cNtwrk->isName = false;

                    }else{

                        printf("\n%s\n", recvBuffer); 
                        wxString msg(recvBuffer);

                        if(std::string(recvBuffer).substr(0, 2) != "/N"){

                            wxString name(cNtwrk->getSenderNick());
                            hpanel->addMsg(msg, name); 

                        }
                        
                    }
                }
            }

            std::cout<<"exiting recv 1"<<std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }while(std::string(recvBuffer) != "ERR_REC_OUT");

        std::cout<<"exiting recv 2"<<std::endl;

    });
}


void MyFrame::sendMsg(){

    tPool.enqueue([this]{
        //send message
        // clientNet.setMessage();
        sprintf(sendBuffer, "%s", hpanel->getMsg());
        std::cout<<"sending: " << sendBuffer << std::endl;
        int byteCount = cNtwrk->sendTo(sendBuffer);
        if(byteCount == SOCKET_ERROR){
            printf("Client failed to send %ld.\n", WSAGetLastError());
            closesocket(clientSocket);
            WSACleanup();
        }

        //check if they want to leave
        if(std::string(sendBuffer) == exitMessage){
                closesocket(clientSocket);
                WSACleanup();
        }

    });

}


///OTHER MISC///
 
void MyFrame::menuItems(){
        //this is the first hello option on the file thingy
    //it takes an ID for the thing
    //what it shows/title
    //and its tooltip or description
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    //seperator is like...the line thingy thaat just separates it
    menuFile->AppendSeparator();
    //this is the exit thing, dont need custom implementation
    menuFile->Append(wxID_EXIT);
    
    //add another thing to the menubar thing
    //you can create a bunch of these wxMenu items
    //yeah...
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
 
    //add these menu items to a menu bar and give them names
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
 
    SetMenuBar( menuBar );
 
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
 
    Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
 
void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}

//this is a custom method 
//what happens when you interect with something bound to
//it
void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}