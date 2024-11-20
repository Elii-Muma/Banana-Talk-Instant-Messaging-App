// Start of wxWidgets "Hello World" Program
#include "MyApp.hpp"
#include "MyFrame.h"
#include <iostream>
 
 
wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    std::cout<<"we not runnin!!!";
    MyFrame *frame = new MyFrame(wxGetApp());
    frame->Show(true);
    return true;
}
