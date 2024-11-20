#include "HomePanel.h"

HomeP::HomeP(wxSimplebook* parent) : wxPanel(parent){
    this->SetBackgroundColour(wxColor(107, 62, 38));

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *sideSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *optionSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *textSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *tSizer_v1 = new wxBoxSizer(wxHORIZONTAL);


    wxPanel *oppPan = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(100, 500), wxBG_STYLE_TRANSPARENT);
    oppPan->SetBackgroundColour(wxColor(254, 194, 105));

    onMembers = new wxCollapsiblePane(oppPan, wxID_ANY, "Online", wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxTAB_TRAVERSAL);
    onMembers->SetBackgroundColour(oppPan->GetBackgroundColour());

    // now add a test label in the collapsible pane using a sizer to lay it out:
    win = onMembers->GetPane();
    paneSz = new wxBoxSizer(wxVERTICAL);

    win->SetSizer(paneSz);
    paneSz->SetSizeHints(win);

    wxStaticText *nameTxt = new wxStaticText(oppPan, wxID_ANY, "DETAILS:"); 
    nameTxt->SetForegroundColour(wxColor(255, 255, 255));

    recvTxt = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_RICH2 | wxTE_MULTILINE | wxTE_WORDWRAP | wxTE_READONLY);
    recvTxt->SetBackgroundColour(wxColor(255, 251, 201));

    sendButton = new wxButton(this, ID_SENDB, "send"); 

    inputTxt = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 50),  wxTE_MULTILINE | wxTE_WORDWRAP );
    inputTxt->SetBackgroundColour(wxColor(255, 251, 201));
    inputTxt->SetHint(wxT("Type Message"));

    logoutButton = new wxButton(this, ID_LOGOUTB, "logout"); 

    optionSizer->Add(nameTxt, 0, wxALIGN_CENTER);
    optionSizer->Add(onMembers, 0, wxALIGN_CENTER);
    oppPan->SetSizer(optionSizer);

    tSizer_v1->Add(inputTxt, 1, wxEXPAND | wxRIGHT, 3);
    tSizer_v1->Add(sendButton, 0, wxEXPAND);

    textSizer->Add(recvTxt, 1, wxEXPAND | wxALL, 3);
    textSizer->Add(tSizer_v1, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 3);

    sideSizer->Add(textSizer, 1, wxEXPAND | wxALL, 5);
    sideSizer->Add(oppPan, 0, wxEXPAND | wxALL, 5);

    mainSizer->Add(sideSizer, 2, wxEXPAND | wxALL, 5);
    mainSizer->Add(logoutButton,0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 5);

    this->SetSizer(mainSizer);
}

void HomeP::addMember(wxString member){
        wxStaticText *name = new wxStaticText(win, wxID_ANY, member); 
        name->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
        paneSz->Add(name, 1, wxGROW |wxLEFT | wxRIGHT, 5);
        std::cout<<member<<" has been added"<<std::endl;
        onMembers->Refresh();
}

void HomeP::onButton(wxCommandEvent&){
    this->reset();
}

void HomeP::OnSendB(wxCommandEvent& event){
    wxTextAttr nameAttr;
    nameAttr.SetTextColour(*wxRED);
    nameAttr.SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    recvTxt->SetDefaultStyle(nameAttr);
    wxString name(nickName);
    recvTxt->AppendText(name);
    recvTxt->AppendText("\n");

    wxTextAttr msgAttr;
    msgAttr.SetTextColour(*wxBLACK);
    recvTxt->SetDefaultStyle(msgAttr);
    recvTxt->AppendText(inputTxt->GetValue());
    recvTxt->AppendText("\n");

    inputTxt->SetLabel("");

}

void HomeP::addRawMessage(wxString msg){

    wxTextAttr msgAttr;
    msgAttr.SetTextColour(*wxBLACK);
    recvTxt->SetDefaultStyle(msgAttr);
    recvTxt->AppendText(msg);
    recvTxt->AppendText("\n");

}

void HomeP::addMsg(wxString msg, wxString nickName){

    wxTextAttr nameAttr;
    nameAttr.SetTextColour(*wxRED);
    nameAttr.SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    recvTxt->SetDefaultStyle(nameAttr);
    recvTxt->AppendText(nickName);
    recvTxt->AppendText("\n");

    wxTextAttr msgAttr;
    msgAttr.SetTextColour(*wxBLACK);
    recvTxt->SetDefaultStyle(msgAttr);
    recvTxt->AppendText(msg);
    recvTxt->AppendText("\n");

}

char* HomeP::getMsg(){
    std::string text = inputTxt->GetValue().ToStdString();
    sprintf(msg, "%s", text.c_str());
    std::cout<<"sending(source) : " << msg << std::endl;

    if(std::string(msg).length() != 0){
        inputTxt->SetLabel("");
        wxString name(nickName);
        addMsg(msg, name);
        return msg;
    }

    return ERR_EMPTY;
}

void HomeP::setNickName(char* name){
    sprintf(nickName, "%s", name);
}


void HomeP::reset(){
    recvTxt->SetLabel("");
    inputTxt->SetLabel("");
}