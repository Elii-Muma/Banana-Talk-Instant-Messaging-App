#pragma once
#include <wx/wx.h>
#include <wx/simplebook.h>
#include <atomic>
#include <iostream>

class LoginP: public wxPanel{
    public:

        wxButton *loginB;
        wxTextCtrl *emailCtrl, *passCtrl, *hostCtrl;
        char str[10];

        std::atomic<bool> isGoingIn{false};

        //CONSTRUCTOR
        LoginP(wxSimplebook* parent);
        void OnLoginButton(wxCommandEvent& event);
        char* getName();
        char* getAddress();
        char* getHost();
        void setIsGoingIn(bool value);
};

enum{
    ID_B1 = 12355,
    ID_ECTRL = 234,
    ID_PASSCTRL = 345,
    ID_HOSTCTRL = 456

};