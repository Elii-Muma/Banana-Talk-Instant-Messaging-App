#pragma once
#include <wx/wx.h>
#include <wx/simplebook.h>
#include <wx/utils.h>
#include "MyApp.hpp"
#include "LoginPanel.h"
#include "server_net.hpp"
#include "ThreadPool.hpp"
#include "HomePanel.h"
#include <unordered_map>
#include <unordered_set>
#include <mutex>

//wxFrame is the main window we use 
//we extend it 
//with custom class
class MyFrame : public wxFrame
{

public:

    SOCKET clientSocket;
    MyClient *cNtwrk;
    MyPool tPool{4};

    char sendBuffer[200];
    const char *exitMessage{"/d"};
    wxApp *myApp;
    std::unordered_map<std::string, std::string> cNames;
    std::mutex m;

    MyFrame(wxApp &app);
    ~MyFrame();
    wxSimplebook *book; 
    LoginP *lpanel;
    HomeP *hpanel;
 
private:
    //these take in an event
    void OnHello(wxCommandEvent& event);
    void menuItems();
    void OnB1(wxCommandEvent& event);
    void OnB2(wxCommandEvent& event);
    void OnSendB(wxCommandEvent& event);
    void recvMsg();
    void sendMsg();
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};

//unique ID that allows us to react to a menu command

enum
{
    ID_Hello = 1,
};