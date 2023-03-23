// wxWidgets "Hello world" Program
// modified to show the usage of wxBorderlessFrame

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wxbf/borderless_frame.h>

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};
class MyFrame : public wxBorderlessFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    wxDECLARE_EVENT_TABLE();
};
enum
{
    ID_Hello = 1
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_Hello, MyFrame::OnHello)
EVT_MENU(wxID_EXIT, MyFrame::OnExit)
EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
EVT_KEY_DOWN(MyFrame::OnKeyDown)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
#ifdef _WIN32
    SetProcessDPIAware();
#endif

    MyFrame* frame = new MyFrame("Hello World", wxPoint(50, 50), wxSize(450, 340));
    frame->SetBorderThickness(1);
    frame->SetBorderColour(wxColour(255, 128, 0));
    frame->SetDoubleBuffered(true);
    frame->Show(true);
    return true;
}
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxBorderlessFrame(NULL, wxID_ANY, title, pos, size)
{
    SetBackgroundColour(wxColour(255, 255, 255));

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    CenterOnScreen();
}
void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets' Hello world sample",
        "About Hello World", wxOK | wxICON_INFORMATION);
}
void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}

void MyFrame::OnKeyDown(wxKeyEvent& event)
{
    if (event.GetKeyCode() == WXK_F2) {
        PopupSystemMenu();
    }

    event.Skip();
}
