/////////////////////////////////////////////////////////////////////////////
// Name:        example/main.cpp
// Purpose:     Example usage of wxBorderlessFrame
// Author:      £ukasz Œwiszcz
// Modified by:
// Created:     2022-12-26
// Copyright:   (c) £ukasz Œwiszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// wxWidgets "Hello world" Program
// modified to show the usage of wxBorderlessFrame

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/clrpicker.h>
#include <wx/spinctrl.h>

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
    std::unique_ptr<wxWindowGripper> m_gripper;
    wxFont m_titleFont;
    wxSystemButtonsBase* m_buttons;
    wxButton* m_btnSystemMenu;
    wxButton* m_btnDragMove;
    wxButton* m_btnDragResize;
    wxCheckBox* m_chkResizable;
    wxCheckBox* m_chkMinimizeBox;
    wxCheckBox* m_chkMaximizeBox;
    wxCheckBox* m_chkCloseBox;
    wxCheckBox* m_chkStayOnTop;
    wxCheckBox* m_chkNoTaskbar;
    wxSpinCtrl* m_spinBorderSize;
    wxSpinCtrl* m_spinShadowSize;
    wxSpinCtrl* m_spinShadowAlpha;
    wxSpinCtrl* m_spinShadowX;
    wxSpinCtrl* m_spinShadowY;
    wxColourPickerCtrl* m_colBorder;
    wxCheckBox* m_chkShadowOnInactive;
    wxColourPickerCtrl* m_colMatrix[4][5][2];

    bool m_titlebarRightDown;

    void SetupGui();
    void ReadWindowStyle();
    void RedrawTitlebar();

    void OnPaint(wxPaintEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnNcMouse(wxMouseEvent& event);
    void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);
    void OnSize(wxSizeEvent& event);

    void OnOpenSystemMenu(wxCommandEvent& event);
    void OnStartDragMove(wxMouseEvent& event);
    void OnStartDragResize(wxMouseEvent& event);
    void OnWindowStyleChange(wxCommandEvent& event);
    void OnChangeBorderSize(wxSpinEvent& event);
    void OnChangeShadowSize(wxSpinEvent& event);
    void OnChangeShadowAlpha(wxSpinEvent& event);
    void OnChangeShadowOffset(wxSpinEvent& event);
    void OnChangeBorderColour(wxColourPickerEvent& event);
    void OnChangeInactiveShadow(wxCommandEvent& event);
};
enum
{
    ID_Hello = 1
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
#if (defined _WIN32 && WINVER >= 0x0605)
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
#endif
    wxInitAllImageHandlers();

    MyFrame* frame = new MyFrame("wxBorderlessFrame demo", wxPoint(50, 50), wxSize(640, 480));
    //frame->SetDoubleBuffered(true);
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxBorderlessFrame(NULL, wxID_ANY, title, pos, size)
    , m_titlebarRightDown(false)
    , m_gripper(wxWindowGripper::Create())
{
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
    SetMinSize(FromDIP(wxSize(150, 500)));
    SetBorderThickness(1);
    SetBorderColour(wxColour(232, 17, 35));

    CreateStatusBar();

    static const int statusWidths[] = { -1, 150 };
    GetStatusBar()->SetFieldsCount(2, statusWidths);

    SetStatusText("Welcome to wxBorderlessFrame!", 0);

    CenterOnScreen();

    m_buttons = wxSystemButtonsFactory::CreateSystemButtons(this);

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
    m_chkResizable->Bind(wxEVT_CHECKBOX, &MyFrame::OnWindowStyleChange, this);
    m_chkMinimizeBox->Bind(wxEVT_CHECKBOX, &MyFrame::OnWindowStyleChange, this);
    m_chkMaximizeBox->Bind(wxEVT_CHECKBOX, &MyFrame::OnWindowStyleChange, this);
    m_chkCloseBox->Bind(wxEVT_CHECKBOX, &MyFrame::OnWindowStyleChange, this);
    m_chkStayOnTop->Bind(wxEVT_CHECKBOX, &MyFrame::OnWindowStyleChange, this);
    m_chkNoTaskbar->Bind(wxEVT_CHECKBOX, &MyFrame::OnWindowStyleChange, this);
    m_spinBorderSize->Bind(wxEVT_SPINCTRL, &MyFrame::OnChangeBorderSize, this);
    m_spinShadowSize->Bind(wxEVT_SPINCTRL, &MyFrame::OnChangeShadowSize, this);
    m_spinShadowAlpha->Bind(wxEVT_SPINCTRL, &MyFrame::OnChangeShadowAlpha, this);
    m_spinShadowX->Bind(wxEVT_SPINCTRL, &MyFrame::OnChangeShadowOffset, this);
    m_spinShadowY->Bind(wxEVT_SPINCTRL, &MyFrame::OnChangeShadowOffset, this);
    m_colBorder->Bind(wxEVT_COLOURPICKER_CHANGED, &MyFrame::OnChangeBorderColour, this);
    m_chkShadowOnInactive->Bind(wxEVT_CHECKBOX, &MyFrame::OnChangeInactiveShadow, this);
    for (size_t colourKind = 0; colourKind < 2; ++colourKind) {
        for (size_t buttonKind = 0; buttonKind < 4; ++buttonKind) {
            for (size_t stateKind = 0; stateKind < 5; ++stateKind) {
                m_colMatrix[buttonKind][stateKind][colourKind]->Bind(
                    wxEVT_COLOURPICKER_CHANGED, [this, colourKind, buttonKind, stateKind](wxColourPickerEvent& event) {
                        m_buttons->SetColourTableEntry(
                            static_cast<wxSystemButton>(buttonKind),
                            static_cast<wxSystemButtonState>(stateKind),
                            static_cast<wxSystemButtonColourKind>(colourKind),
                            event.GetColour());
                        m_buttons->UpdateState();
                    });
            }
        }
    }
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

    wxStaticBoxSizer* btnSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Functions");
    m_btnSystemMenu = new wxButton(this, wxID_ANY, "PopupSystemMenu");
    m_btnDragMove = new wxButton(this, wxID_ANY, "StartDragMove");
    m_btnDragResize = new wxButton(this, wxID_ANY, "StartDragResize");
    btnSizer->Add(m_btnSystemMenu, 0, wxALL, FromDIP(10));
    btnSizer->Add(m_btnDragMove, 0, wxALL, FromDIP(10));
    btnSizer->Add(m_btnDragResize, 0, wxALL, FromDIP(10));

    sizer->Add(btnSizer, 0, wxALIGN_CENTRE | wxALL, FromDIP(10));

    wxBoxSizer* options = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* styleSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Window style");
    m_chkResizable = new wxCheckBox(this, wxID_ANY, "wxRESIZE_BORDER");
    m_chkMinimizeBox = new wxCheckBox(this, wxID_ANY, "wxMINIMIZE_BOX");
    m_chkMaximizeBox = new wxCheckBox(this, wxID_ANY, "wxMAXIMIZE_BOX");
    m_chkCloseBox = new wxCheckBox(this, wxID_ANY, "wxCLOSE_BOX");
    m_chkStayOnTop = new wxCheckBox(this, wxID_ANY, "wxSTAY_ON_TOP");
    m_chkNoTaskbar = new wxCheckBox(this, wxID_ANY, "wxFRAME_NO_TASKBAR");
    styleSizer->Add(m_chkResizable, 0, wxLEFT | wxRIGHT, FromDIP(10));
    styleSizer->Add(m_chkMinimizeBox, 0, wxLEFT | wxRIGHT, FromDIP(10));
    styleSizer->Add(m_chkMaximizeBox, 0, wxLEFT | wxRIGHT, FromDIP(10));
    styleSizer->Add(m_chkCloseBox, 0, wxLEFT | wxRIGHT, FromDIP(10));
    styleSizer->Add(m_chkStayOnTop, 0, wxLEFT | wxRIGHT, FromDIP(10));
    styleSizer->Add(m_chkNoTaskbar, 0, wxLEFT | wxRIGHT, FromDIP(10));
    styleSizer->AddSpacer(FromDIP(5));

    ReadWindowStyle();
    options->Add(styleSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(10));

    wxStaticBoxSizer* borderSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Border and shadow");
    m_spinBorderSize = new wxSpinCtrl(this);
    m_spinBorderSize->SetMin(0);
    m_spinBorderSize->SetMax(10);
    m_spinBorderSize->SetValue(GetBorderThickness());
    m_spinShadowSize = new wxSpinCtrl(this);
    m_spinShadowSize->SetMin(0);
    m_spinShadowSize->SetMax(50);
    m_spinShadowSize->SetValue(GetShadowSize());
    m_spinShadowAlpha = new wxSpinCtrl(this);
    m_spinShadowAlpha->SetMin(0);
    m_spinShadowAlpha->SetMax(255);
    m_spinShadowAlpha->SetValue(GetShadowAlpha());
    m_spinShadowX = new wxSpinCtrl(this);
    m_spinShadowX->SetMin(-16);
    m_spinShadowX->SetMax(16);
    m_spinShadowX->SetValue(GetShadowOffset().x);
    m_spinShadowY = new wxSpinCtrl(this);
    m_spinShadowY->SetMin(-16);
    m_spinShadowY->SetMax(16);
    m_spinShadowY->SetValue(GetShadowOffset().y);
    m_colBorder = new wxColourPickerCtrl(this, wxID_ANY, GetBorderColour());
    m_chkShadowOnInactive = new wxCheckBox(this, wxID_ANY, "No shadow if inactive");
    m_chkShadowOnInactive->SetValue(IsShadowDisabledOnInactiveWindow());
    borderSizer->Add(new wxStaticText(this, wxID_ANY, "Border thickness (px):"));
    borderSizer->Add(m_spinBorderSize, 0, wxEXPAND | wxBOTTOM, FromDIP(2));
    borderSizer->Add(new wxStaticText(this, wxID_ANY, "Shadow size (px):"));
    borderSizer->Add(m_spinShadowSize, 0, wxEXPAND | wxBOTTOM, FromDIP(2));
    borderSizer->Add(new wxStaticText(this, wxID_ANY, "Shadow alpha (0-255):"));
    borderSizer->Add(m_spinShadowAlpha, 0, wxEXPAND | wxBOTTOM, FromDIP(2));
    borderSizer->Add(new wxStaticText(this, wxID_ANY, "Shadow X offset (px):"));
    borderSizer->Add(m_spinShadowX, 0, wxEXPAND | wxBOTTOM, FromDIP(2));
    borderSizer->Add(new wxStaticText(this, wxID_ANY, "Shadow Y offset (px):"));
    borderSizer->Add(m_spinShadowY, 0, wxEXPAND | wxBOTTOM, FromDIP(2));
    borderSizer->Add(new wxStaticText(this, wxID_ANY, "Border colour:"));
    borderSizer->Add(m_colBorder, 0, wxEXPAND | wxBOTTOM, FromDIP(2));
    borderSizer->Add(m_chkShadowOnInactive, 0, wxEXPAND | wxBOTTOM, FromDIP(2));

    options->Add(borderSizer, 0, wxEXPAND | wxRIGHT | wxBOTTOM, FromDIP(10));
    sizer->Add(options, 0, wxALIGN_CENTRE);

    wxStaticBoxSizer* matrixBoxSizer = new wxStaticBoxSizer(wxVERTICAL, this, "System menu color matrix");
    wxFlexGridSizer* matrixSizer = new wxFlexGridSizer(6, FromDIP(wxSize(4, 4)));
    matrixSizer->AddSpacer(0);
    matrixSizer->Add(new wxStaticText(this, wxID_ANY, "Inactive"));
    matrixSizer->Add(new wxStaticText(this, wxID_ANY, "Disabled"));
    matrixSizer->Add(new wxStaticText(this, wxID_ANY, "Normal"));
    matrixSizer->Add(new wxStaticText(this, wxID_ANY, "Hover"));
    matrixSizer->Add(new wxStaticText(this, wxID_ANY, "Pressed"));

    static const wxString BTN_LABELS[] = {
        "Minimize BG",
        "Maximize BG",
        "Restore BG",
        "Close BG",
        "Minimize FG",
        "Maximize FG",
        "Restore FG",
        "Close FG",
    };

    int labelIdx = 0;
    for (size_t colourKind = 0; colourKind < 2; ++colourKind) {
        for (size_t buttonKind = 0; buttonKind < 4; ++buttonKind) {
            matrixSizer->Add(new wxStaticText(this, wxID_ANY, BTN_LABELS[labelIdx++]));

            for (size_t stateKind = 0; stateKind < 5; ++stateKind) {
                m_colMatrix[buttonKind][stateKind][colourKind] = new wxColourPickerCtrl(
                    this, wxID_ANY, m_buttons->GetColourTableEntry(
                        static_cast<wxSystemButton>(buttonKind),
                        static_cast<wxSystemButtonState>(stateKind),
                        static_cast<wxSystemButtonColourKind>(colourKind)));
                matrixSizer->Add(m_colMatrix[buttonKind][stateKind][colourKind]);
            }
        }
    }

    matrixBoxSizer->Add(matrixSizer, 0, wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(10));
    sizer->Add(matrixBoxSizer, 0, wxALIGN_CENTRE | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(10));

    SetSizerAndFit(sizer);
    SetMinSize(wxSize(-1, -1)); // disable min size

    m_titleFont = GetFont();
    m_titleFont.SetPointSize(12);
    m_titleFont.MakeBold();
}

void MyFrame::ReadWindowStyle()
{
    long style = GetWindowStyle();
    m_chkResizable->SetValue(style & wxRESIZE_BORDER);
    m_chkMinimizeBox->SetValue(style & wxMINIMIZE_BOX);
    m_chkMaximizeBox->SetValue(style & wxMAXIMIZE_BOX);
    m_chkCloseBox->SetValue(style & wxCLOSE_BOX);
    m_chkStayOnTop->SetValue(style & wxSTAY_ON_TOP);
    m_chkNoTaskbar->SetValue(style & wxFRAME_NO_TASKBAR);
}

void MyFrame::RedrawTitlebar()
{
    RefreshRect(wxRect(0, 0, GetSize().x, m_buttons->GetTotalSize().y));
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
    RedrawTitlebar();
    event.Skip();
}

void MyFrame::OnOpenSystemMenu(wxCommandEvent& event)
{
    PopupSystemMenu();
}

void MyFrame::OnStartDragMove(wxMouseEvent& event)
{
    m_gripper->StartDragMove(this);
}

void MyFrame::OnStartDragResize(wxMouseEvent& event)
{
    m_gripper->StartDragResize(this, static_cast<wxDirection>(wxNORTH | wxWEST));
}

void MyFrame::OnWindowStyleChange(wxCommandEvent& event)
{
    long styleMask = wxRESIZE_BORDER | wxMINIMIZE_BOX | wxMAXIMIZE_BOX
        | wxCLOSE_BOX | wxSTAY_ON_TOP | wxFRAME_NO_TASKBAR;

    long newStyle = 0;
    newStyle |= (m_chkResizable->GetValue()) ? wxRESIZE_BORDER : 0;
    newStyle |= (m_chkMinimizeBox->GetValue()) ? wxMINIMIZE_BOX : 0;
    newStyle |= (m_chkMaximizeBox->GetValue()) ? wxMAXIMIZE_BOX : 0;
    newStyle |= (m_chkCloseBox->GetValue()) ? wxCLOSE_BOX : 0;
    newStyle |= (m_chkStayOnTop->GetValue()) ? wxSTAY_ON_TOP : 0;
    newStyle |= (m_chkNoTaskbar->GetValue()) ? wxFRAME_NO_TASKBAR : 0;

    SetWindowStyle(newStyle | (GetWindowStyle() & ~(styleMask)));
    m_buttons->UpdateState();
}

void MyFrame::OnChangeBorderSize(wxSpinEvent& event)
{
    SetBorderThickness(event.GetInt());
}

void MyFrame::OnChangeShadowSize(wxSpinEvent& event)
{
    SetShadowSize(event.GetInt());
}

void MyFrame::OnChangeShadowAlpha(wxSpinEvent& event)
{
    SetShadowAlpha(event.GetInt());
}

void MyFrame::OnChangeShadowOffset(wxSpinEvent& event)
{
    SetShadowOffset(wxPoint(m_spinShadowX->GetValue(), m_spinShadowY->GetValue()));
}

void MyFrame::OnChangeBorderColour(wxColourPickerEvent& event)
{
    SetBorderColour(event.GetColour());
    RedrawTitlebar();
}

void MyFrame::OnChangeInactiveShadow(wxCommandEvent& event)
{
    SetShadowDisabledOnInactiveWindow(m_chkShadowOnInactive->GetValue());
}
