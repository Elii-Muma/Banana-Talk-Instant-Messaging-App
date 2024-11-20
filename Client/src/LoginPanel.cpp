#include "LoginPanel.h"

LoginP::LoginP(wxSimplebook* parent) : wxPanel(parent){
    this->SetBackgroundColour(wxColor(102, 62, 38));

    wxStaticText *headerTxt = new wxStaticText(this, wxID_ANY, "BANANA TALK"); 
    headerTxt->SetForegroundColour(wxColor(255, 251, 201));
    headerTxt->SetFont(wxFont(32, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_EXTRABOLD));

    wxStaticText *welcomeTxt = new wxStaticText(this, wxID_ANY, "Welcome!"); 
    welcomeTxt->SetForegroundColour(wxColor(255, 251, 201));
    welcomeTxt->SetFont(wxFont(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_SEMIBOLD));

    wxStaticText *nameTxt = new wxStaticText(this, wxID_ANY, "Name"); 
    nameTxt->SetForegroundColour(wxColor(255, 251, 201));
    emailCtrl = new wxTextCtrl(this, wxID_ANY);
    emailCtrl->SetHint(wxT("enter nickname"));

    wxStaticText *passTxt = new wxStaticText(this, wxID_ANY, "IP Address"); 
    passTxt->SetForegroundColour(wxColor(255, 251, 201));
    passCtrl = new wxTextCtrl(this, wxID_ANY);
    passCtrl->SetHint(wxT("enter IP Address"));
    passCtrl->SetLabel(wxT("localhost"));


    wxStaticText *hostTxt = new wxStaticText(this, wxID_ANY, "Host"); 
    hostTxt->SetForegroundColour(wxColor(255, 251, 201));
    hostCtrl = new wxTextCtrl(this, wxID_ANY);
    hostCtrl->SetHint(wxT("enter Host"));
    hostCtrl->SetLabel("55555");

    loginB = new wxButton(this, ID_B1, "connect"); 

    wxBoxSizer *widgS= new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *mainS = new wxBoxSizer(wxVERTICAL);
    mainS->AddStretchSpacer();

    widgS->Add(headerTxt, 0, wxALIGN_CENTER| wxBOTTOM, 5);
    widgS->Add(welcomeTxt, 0, wxALIGN_CENTER| wxBOTTOM, 30);
    widgS->Add(nameTxt, 0, wxALIGN_LEFT| wxLEFT, 185);
    widgS->Add(emailCtrl, 0, wxALIGN_CENTER | wxALL, 10);
    widgS->Add(passTxt, 0, wxALIGN_LEFT| wxLEFT, 185);
    widgS->Add(passCtrl, 0, wxALIGN_CENTER | wxALL, 10);
    widgS->Add(hostTxt, 0, wxALIGN_LEFT | wxLEFT, 185);
    widgS->Add(hostCtrl, 0, wxALIGN_CENTER | wxALL, 10);
    widgS->Add(loginB, 0, wxALIGN_CENTER | wxALL, 10);

    mainS->Add(widgS, 0, wxEXPAND | wxCENTER, 10);
    mainS->AddStretchSpacer();
    this->SetSizer(mainS);
}

void LoginP::OnLoginButton(wxCommandEvent&){
    if(!emailCtrl->GetLineText(0).IsEmpty()){
        isGoingIn = true;
    }
}

char* LoginP::getName(){
    std::string text = emailCtrl->GetValue().ToStdString();
    sprintf(str, "%s", text.c_str());
    return str;
}

char* LoginP::getAddress(){
    std::string text = passCtrl->GetValue().ToStdString();
    sprintf(str, "%s", text.c_str());
    return str;
}

char* LoginP::getHost(){
    std::string text = hostCtrl->GetValue().ToStdString();
    sprintf(str, "%s", text.c_str());
    return str;
}

void LoginP::setIsGoingIn(bool value){
    isGoingIn = value;
}

