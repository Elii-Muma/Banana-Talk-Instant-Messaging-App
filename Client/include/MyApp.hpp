#pragma once
#include <wx/wx.h>


//this class will give us the entrypoint method
//we pass it to wxIMPLEMENT_APP
//class extends wxApp
class MyApp : public wxApp
{
public:
    bool OnInit() override;
};
