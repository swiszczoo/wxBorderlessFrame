// wxWidgets "Hello world" Program
// modified to show the usage of wxBorderlessFrame

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wxbf/borderless_frame.h>
#include <wxbf/system_buttons.h>
#include <wxbf/window_gripper_msw.h>

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};
class MyFrame : public wxBorderlessFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

protected:
    virtual wxWindowPart GetWindowPart(wxPoint mousePosition) const wxOVERRIDE;

private:
    wxWindowGripperMSW m_gripper;
    wxFont m_titleFont;
    wxSystemButtonsBase* m_buttons;
    wxButton* m_btnSystemMenu;
    wxButton* m_btnDragMove;
    wxButton* m_btnDragResize;
    bool m_titlebarRightDown;

    void SetupGui();
    void OnPaint(wxPaintEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnNcMouse(wxMouseEvent& event);
    void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);
    void OnSize(wxSizeEvent& event);

    void OnOpenSystemMenu(wxCommandEvent& event);
    void OnStartDragMove(wxMouseEvent& event);
    void OnStartDragResize(wxMouseEvent& event);
};
enum
{
    ID_Hello = 1
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
#ifdef _WIN32
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
#endif
    wxInitAllImageHandlers();

    MyFrame* frame = new MyFrame("wxBorderlessFrame demo", wxPoint(50, 50), wxSize(640, 480));
    frame->SetBorderThickness(1);
    frame->SetBorderColour(wxColour(232, 17, 35));
    frame->SetDoubleBuffered(true);
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxBorderlessFrame(NULL, wxID_ANY, title, pos, size)
    , m_titlebarRightDown(false)
{
    SetBackgroundColour(wxColour(255, 255, 255));

    CreateStatusBar();

    static const int statusWidths[] = {-1, 150};
    GetStatusBar()->SetFieldsCount(2, statusWidths);

    SetStatusText("Welcome to wxBorderlessFrame!", 0);

    CenterOnScreen();

    m_buttons = wxSystemButtonsFactory::CreateSystemButtons(this);
    m_buttons->SetColourTableEntry(wxSB_CLOSE, wxSB_STATE_NORMAL, 
        wxSB_COLOUR_FOREGROUND, wxColour(232, 17, 35));
    
    SetupGui();

    // Event bindings

    Bind(wxEVT_PAINT, &MyFrame::OnPaint, this);
    Bind(wxEVT_KEY_DOWN, &MyFrame::OnKeyDown, this);
    Bind(wxEVT_NC_RIGHT_DOWN, &MyFrame::OnNcMouse, this);
    Bind(wxEVT_NC_RIGHT_UP, &MyFrame::OnNcMouse, this);
    Bind(wxEVT_RIGHT_UP, &MyFrame::OnNcMouse, this);
    Bind(wxEVT_MOUSE_CAPTURE_LOST, &MyFrame::OnMouseCaptureLost, this);
    Bind(wxEVT_SIZE, &MyFrame::OnSize, this);
    m_btnSystemMenu->Bind(wxEVT_BUTTON, &MyFrame::OnOpenSystemMenu, this);
    m_btnDragMove->Bind(wxEVT_LEFT_DOWN, &MyFrame::OnStartDragMove, this);
    m_btnDragResize->Bind(wxEVT_LEFT_DOWN, &MyFrame::OnStartDragResize, this);
}

wxWindowPart MyFrame::GetWindowPart(wxPoint mousePosition) const
{
    wxWindowPart sysButtonPart = m_buttons->GetWindowPart(mousePosition);
    MyFrame* cthis = const_cast<MyFrame*>(this);

    switch (sysButtonPart) {
    case wxWP_TITLEBAR:
        cthis->SetStatusText("last part: Titlebar", 1);
        break;
    case wxWP_MINIMIZE_BUTTON:
        cthis->SetStatusText("last part: Minimize", 1);
        break;
    case wxWP_MAXIMIZE_BUTTON:
        cthis->SetStatusText("last part: Maximize", 1);
        break;
    case wxWP_CLOSE_BUTTON:
        cthis->SetStatusText("last part: Close", 1);
        break;
    case wxWP_CLIENT_AREA:
        cthis->SetStatusText("last part: ClientArea", 1);
        break;
    }

    return sysButtonPart;
}

void MyFrame::SetupGui()
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->AddSpacer(FromDIP(30));

    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    m_btnSystemMenu = new wxButton(this, wxID_ANY, "PopupSystemMenu");
    btnSizer->Add(m_btnSystemMenu, 0, wxALL, FromDIP(10));
    
    m_btnDragMove = new wxButton(this, wxID_ANY, "StartDragMove");
    btnSizer->Add(m_btnDragMove, 0, wxALL, FromDIP(10));

    m_btnDragResize = new wxButton(this, wxID_ANY, "StartDragResize");
    btnSizer->Add(m_btnDragResize, 0, wxALL, FromDIP(10));

    sizer->Add(btnSizer, 0, wxEXPAND);

    SetSizer(sizer);

    m_titleFont.Create(13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, 
        wxFONTWEIGHT_BOLD, false, "Calibri");
    m_titleFont.MakeBold();
}

void MyFrame::OnPaint(wxPaintEvent& event)
{
    static const wxString TITLE = "wxBorderlessFrame demo";

    wxPaintDC dc(this);
    dc.SetFont(m_titleFont);
    dc.SetTextForeground(GetBorderColour());
    wxCoord width = dc.GetTextExtent(TITLE).x;

    if (width + FromDIP(10) < dc.GetSize().x - m_buttons->GetTotalSize().x) {
        dc.DrawText(TITLE, FromDIP(10), FromDIP(5));
    }

    event.Skip();
}

void MyFrame::OnKeyDown(wxKeyEvent& event)
{
    if (event.GetKeyCode() == WXK_F2) {
        PopupSystemMenu();
    }

    event.Skip();
}

void MyFrame::OnNcMouse(wxMouseEvent& event)
{
    if (event.GetEventType() == wxEVT_NC_RIGHT_DOWN) {
        wxWindowPart part = GetWindowPart(wxGetMousePosition());
        if (part == wxWP_TITLEBAR) {
            m_titlebarRightDown = true;
            CaptureMouse();
        }
    }
    else if (event.GetEventType() == wxEVT_NC_RIGHT_UP || event.RightUp()) {
        wxWindowPart part = GetWindowPart(wxGetMousePosition());
        bool hasCapture = HasCapture();
        ReleaseCapture();

        if (part == wxWP_TITLEBAR && hasCapture) {
            PopupSystemMenu();
            m_titlebarRightDown = false;
        }
    }

    event.Skip();
}

void MyFrame::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
    m_titlebarRightDown = false;
}

void MyFrame::OnSize(wxSizeEvent& event)
{
    RefreshRect(wxRect(0, 0, event.GetSize().x, m_buttons->GetTotalSize().y));
    event.Skip();
}

void MyFrame::OnOpenSystemMenu(wxCommandEvent& event)
{
    PopupSystemMenu();
}

void MyFrame::OnStartDragMove(wxMouseEvent& event)
{
    m_gripper.StartDragMove(this);
}

void MyFrame::OnStartDragResize(wxMouseEvent& event)
{
    m_gripper.StartDragResize(this, static_cast<wxDirection>(wxNORTH | wxWEST));
}
