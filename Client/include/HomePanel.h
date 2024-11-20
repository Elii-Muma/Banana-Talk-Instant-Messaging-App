#pragma once
#include <wx/wx.h>
#include <wx/simplebook.h>
#include <wx/collpane.h>
#include <string>

#include <iostream>

class HomeP: public wxPanel{
    public:
        wxButton *logoutButton, *sendButton;
        wxTextCtrl *recvTxt, *inputTxt;
        wxCollapsiblePane *onMembers;

        char nickName[10];
        char msg[200];
        char ERR_EMPTY[200]{"null_field"};

        wxSizer *paneSz;
        wxWindow *win;
        std::vector<wxStaticText> *memberNames;

        //CONSTRUCTOR
        HomeP(wxSimplebook* parent);
        void addMember(wxString member);
        void reset();
        void onButton(wxCommandEvent& event);
        void OnSendB(wxCommandEvent& event);
        void addRawMessage(wxString msg);
        void addMsg(wxString msg, wxString nickName);
        char *getMsg();
        void setNickName(char *name);
};

enum{
    ID_LOGOUTB = 23411,
    ID_SENDB =23412 
};