/////////////////////////////////////////////////////////////////////////////
// Name:        borderless_frame_msw.cpp
// Purpose:     wxBorderlessFrame implementation for MSW
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2022-12-26
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "uxtheme.lib")

#include <wxbf/borderless_frame_msw.h>
#include <wxbf/window_gripper_msw.h>

#include <CommCtrl.h>
#include <wx/msw/private.h>
#include <Uxtheme.h>

// This undocumented messages are drawing garbage
#ifndef WM_NCUAHDRAWCAPTION
#define WM_NCUAHDRAWCAPTION (0x00AE)
#endif
#ifndef WM_NCUAHDRAWFRAME
#define WM_NCUAHDRAWFRAME (0x00AF)
#endif

wxBorderlessFrameMSW::wxBorderlessFrameMSW(wxWindow* parent,
    wxWindowID id, const wxString& title, const wxPoint& pos,
    const wxSize& size, long style, const wxString& name)
{
    Init();
    Create(parent, id, title, pos, size, style, name);
}

bool wxBorderlessFrameMSW::Create(wxWindow* parent,
    wxWindowID id, const wxString& title, const wxPoint& pos,
    const wxSize& size, long style, const wxString& name)
{
    if (!wxFrame::Create(parent, id, title, pos, size, style, name)) {
        return false;
    }

    m_shadow[0] = new wxResizerDropShadowFrame<wxDropShadowFrameMSW>(
        new wxWindowGripperMSW, NULL, wxID_ANY, wxSHADOW_TOP);
    m_shadow[1] = new wxResizerDropShadowFrame<wxDropShadowFrameMSW>(
        new wxWindowGripperMSW, NULL, wxID_ANY, wxSHADOW_BOTTOM);
    m_shadow[2] = new wxResizerDropShadowFrame<wxDropShadowFrameMSW>(
        new wxWindowGripperMSW, NULL, wxID_ANY, wxSHADOW_LEFT);
    m_shadow[3] = new wxResizerDropShadowFrame<wxDropShadowFrameMSW>(
        new wxWindowGripperMSW, NULL, wxID_ANY, wxSHADOW_RIGHT);

    for (int i = 0; i < 4; i++) {
        m_shadow[i]->SetCornerRadius(16);
        m_shadow[i]->AttachToWindow(this);
    }

    m_leaveTrackArmed = false;

    Bind(wxEVT_MAXIMIZE, &wxBorderlessFrameMSW::OnMaximize, this);
    Bind(wxEVT_ICONIZE, &wxBorderlessFrameMSW::OnIconize, this);
    Bind(wxEVT_SIZE, &wxBorderlessFrameMSW::OnSize, this);

    return true;
}

wxBorderlessFrameMSW::~wxBorderlessFrameMSW()
{
    for (int i = 0; i < 4; i++) {
        delete m_shadow[i];
    }
}

void wxBorderlessFrameMSW::PopupSystemMenu()
{
    ::HMENU hSysMenu = GetSystemMenu(GetHWND(), FALSE);
    wxPoint mousePos = wxGetMousePosition();

    UpdateSystemMenu(hSysMenu);

    int cmd = ::TrackPopupMenu(hSysMenu, TPM_RETURNCMD,
        mousePos.x, mousePos.y, 0, GetHWND(), NULL);

    SendMessage(GetHWND(), WM_SYSCOMMAND, static_cast<WPARAM>(cmd), NULL);
}

WXLRESULT wxBorderlessFrameMSW::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
    switch (message) {
    case WM_CREATE:
        UpdateTheme();
        break;
    case WM_NCACTIVATE:
        return TRUE;
    case WM_NCCALCSIZE:
        if (wParam) {
            NCCALCSIZE_PARAMS* csp = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
            int thickness = GetBorderThickness();

            if (!IsMaximized()) {
                csp->rgrc[0].top += thickness;
                csp->rgrc[0].left += thickness;
                csp->rgrc[0].right -= thickness;
                csp->rgrc[0].bottom -= thickness;
                return 0;
            }
            else {
                AdjustMaximizedClientRect(GetHWND(), csp->rgrc[0]);
                return 0;
            }
        }
        break;
    case WM_NCHITTEST:
    {
        static const int RETURN_VALUES[] = {
            HTCLIENT, HTCAPTION, HTMINBUTTON, HTMAXBUTTON, HTCLOSE };

        wxPoint mousePos = wxGetMousePosition();
        wxRect wndRect = GetScreenRect();
        int thickness = GetBorderThickness();

        if (!IsMaximized()) {
            if (mousePos.x - wndRect.GetLeft() <= thickness) {
                if (mousePos.y - wndRect.GetTop() <= thickness) {
                    return HTTOPLEFT;
                }
                if (wndRect.GetBottom() - mousePos.y <= thickness) {
                    return HTBOTTOMLEFT;
                }
                return HTLEFT;
            }
            if (wndRect.GetRight() - mousePos.x <= thickness) {
                if (mousePos.y - wndRect.GetTop() <= thickness) {
                    return HTTOPRIGHT;
                }
                if (wndRect.GetBottom() - mousePos.y <= thickness) {
                    return HTBOTTOMRIGHT;
                }
                return HTRIGHT;
            }
            if (mousePos.y - wndRect.GetTop() <= thickness) {
                return HTTOP;
            }
            if (wndRect.GetBottom() - mousePos.y <= thickness) {
                return HTBOTTOM;
            }
        }

        return RETURN_VALUES[static_cast<int>(GetWindowPart(mousePos))];
    }
    case WM_NCPAINT:
    {
        wxWindowDC dc(this);

        if (IsMaximized()) {
            // It must be called here, otherwise Aero Snap suddenly stops working,
            // at least on Win10, because it uses some kind of a global state 
            // (great job whoever wrote the default NCPAINT handler)
            // If it starts drawing something we don't want, 
            // we may use ExcludeClipRect or something else to prevent it

            return wxFrame::MSWWindowProc(message, wParam, lParam);
        }

        if (m_borderThickness > 0) {
            ::SelectClipRgn(dc.GetHDC(), NULL);
            dc.SetBrush(*wxTRANSPARENT_BRUSH);
            dc.SetPen(wxPen(m_borderColour, m_borderThickness));

            wxRect targetRect(GetSize());
            targetRect.x += m_borderThickness / 2;
            targetRect.y += m_borderThickness / 2;
            targetRect.width -= m_borderThickness - 1;
            targetRect.height -= m_borderThickness - 1;

            dc.DrawRectangle(targetRect);
        }

        return 0;
    }
    case WM_NCUAHDRAWCAPTION:
    case WM_NCUAHDRAWFRAME:
        return 0;
    case WM_NCLBUTTONDOWN:
    case WM_NCLBUTTONUP:
    case WM_NCRBUTTONDOWN:
    case WM_NCRBUTTONUP:
    case WM_NCMOUSEMOVE:
    case WM_NCMOUSELEAVE:
        return HandleNcMessage(message, wParam, lParam);
    default:
        return wxFrame::MSWWindowProc(message, wParam, lParam);
    }

    return wxFrame::MSWWindowProc(message, wParam, lParam);
}

void wxBorderlessFrameMSW::Init()
{
    wxFrame::Init();
    m_borderThickness = 1;
    m_borderColour = wxColour(168, 168, 168);
    m_maximizedTheme = true;

    for (int i = 0; i < 4; i++) {
        m_shadow[i] = nullptr;
    }
}

wxWindowPart wxBorderlessFrameMSW::GetWindowPart(wxPoint mousePosition) const
{
    return wxWP_CLIENT_AREA;
}

void wxBorderlessFrameMSW::ExecSystemCommand(wxSystemCommand command)
{
    switch (command) {
    case wxSC_MINIMIZE:
        ::SendMessage(GetHWND(), WM_SYSCOMMAND, SC_MINIMIZE, NULL);
        return;
    case wxSC_MAXIMIZE:
        ::SendMessage(GetHWND(), WM_SYSCOMMAND, SC_MAXIMIZE, NULL);
        return;
    case wxSC_RESTORE:
        ::SendMessage(GetHWND(), WM_SYSCOMMAND, SC_RESTORE, NULL);
        return;
    case wxSC_CLOSE:
        ::SendMessage(GetHWND(), WM_SYSCOMMAND, SC_CLOSE, NULL);
        return;
    }
}

void wxBorderlessFrameMSW::SetWindowStyleFlag(long style)
{
    long oldStyle = GetWindowStyleFlag();
    static const long MASK = wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxMAXIMIZE | wxCLOSE_BOX;

    wxFrame::SetWindowStyleFlag(style);

    if ((oldStyle & MASK) != (style & MASK)) {
        wxPostEvent(this, wxCommandEvent(wxEVT_UPDATE_SYSTEM_BUTTONS));
    }
}

void wxBorderlessFrameMSW::UpdateNcArea()
{
    ::SetWindowPos(GetHWND(), nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
}

void wxBorderlessFrameMSW::UpdateSystemMenu(::HMENU sysMenu)
{
    MENUITEMINFO mii;
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_STATE;
    mii.fType = 0;

    // update the options
    mii.fState = MF_ENABLED;
    SetMenuItemInfo(sysMenu, SC_RESTORE, FALSE, &mii);
    SetMenuItemInfo(sysMenu, SC_SIZE, FALSE, &mii);
    SetMenuItemInfo(sysMenu, SC_MOVE, FALSE, &mii);
    SetMenuItemInfo(sysMenu, SC_MAXIMIZE, FALSE, &mii);
    SetMenuItemInfo(sysMenu, SC_MINIMIZE, FALSE, &mii);

    mii.fState = MF_GRAYED;

    WINDOWPLACEMENT wp;
    GetWindowPlacement(GetHWND(), &wp);

    switch (wp.showCmd)
    {
    case SW_SHOWMAXIMIZED:
        SetMenuItemInfo(sysMenu, SC_SIZE, FALSE, &mii);
        SetMenuItemInfo(sysMenu, SC_MOVE, FALSE, &mii);
        SetMenuItemInfo(sysMenu, SC_MAXIMIZE, FALSE, &mii);
        SetMenuDefaultItem(sysMenu, SC_CLOSE, FALSE);
        break;
    case SW_SHOWMINIMIZED:
        SetMenuItemInfo(sysMenu, SC_MINIMIZE, FALSE, &mii);
        SetMenuDefaultItem(sysMenu, SC_RESTORE, FALSE);
        break;
    case SW_SHOWNORMAL:
        SetMenuItemInfo(sysMenu, SC_RESTORE, FALSE, &mii);
        SetMenuDefaultItem(sysMenu, SC_CLOSE, FALSE);
        break;
    }
}

void wxBorderlessFrameMSW::AdjustMaximizedClientRect(::HWND window, RECT& rect)
{
    ::HMONITOR monitor = ::MonitorFromWindow(window, MONITOR_DEFAULTTONULL);
    if (!monitor) {
        return;
    }

    ::MONITORINFO monitorInfo = {};
    monitorInfo.cbSize = sizeof(monitorInfo);
    if (!::GetMonitorInfo(monitor, &monitorInfo)) {
        return;
    }

    rect = monitorInfo.rcWork;
}

void wxBorderlessFrameMSW::UpdateTheme()
{
    bool desiredTheme = IsMaximized();

    if (desiredTheme == m_maximizedTheme) {
        return;
    }

    if (desiredTheme) {
        ::SetWindowTheme(GetHWND(), NULL, NULL);
        UpdateNcArea();
        ::RedrawWindow(GetHWND(), NULL, NULL, RDW_FRAME | RDW_UPDATENOW);
    }
    else {
        ::SetWindowTheme(GetHWND(), L"", L"");
        UpdateNcArea();
        ::RedrawWindow(GetHWND(), NULL, NULL, RDW_FRAME | RDW_UPDATENOW);
    }

    m_maximizedTheme = desiredTheme;
}

void wxBorderlessFrameMSW::OnMaximize(wxMaximizeEvent& evnt)
{
    UpdateTheme();
    evnt.Skip();
}

void wxBorderlessFrameMSW::OnIconize(wxIconizeEvent& evnt)
{
    if (!evnt.IsIconized() && IsMaximized()) {
        RECT rect;
        AdjustMaximizedClientRect(GetHWND(), rect);
        ::SetWindowPos(GetHWND(), NULL, rect.left, rect.top, 
            rect.right - rect.left, rect.bottom - rect.top, 
            SWP_NOSENDCHANGING | SWP_NOZORDER);
        ::RedrawWindow(GetHWND(), NULL, NULL, RDW_FRAME | RDW_UPDATENOW);
    }

    evnt.Skip();
}

void wxBorderlessFrameMSW::OnSize(wxSizeEvent& evnt)
{
    UpdateTheme();
    evnt.Skip();
}

#include <windowsx.h>
WXLRESULT wxBorderlessFrameMSW::HandleNcMessage(
    WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
    wxMouseEvent evnt;
    int state = 0;
    if (wxIsShiftDown())
        state |= MK_SHIFT;
    if (wxIsCtrlDown())
        state |= MK_CONTROL;

    // Only the high-order bit should be tested
    if (::GetKeyState(VK_LBUTTON) & (1 << 15)) {
        state |= MK_LBUTTON;
    }
    if (::GetKeyState(VK_MBUTTON) & (1 << 15)) {
        state |= MK_MBUTTON;
    }
    if (::GetKeyState(VK_RBUTTON) & (1 << 15)) {
        state |= MK_RBUTTON;
    }

    int x, y;

    if (message != WM_NCMOUSELEAVE) {
        switch (message) {
        case WM_NCLBUTTONDOWN: evnt.SetEventType(wxEVT_NC_LEFT_DOWN); break;
        case WM_NCLBUTTONUP: evnt.SetEventType(wxEVT_NC_LEFT_UP); break;
        case WM_NCRBUTTONDOWN:  evnt.SetEventType(wxEVT_NC_RIGHT_DOWN); break;
        case WM_NCRBUTTONUP: evnt.SetEventType(wxEVT_NC_RIGHT_UP); break;
        case WM_NCMOUSEMOVE: evnt.SetEventType(wxEVT_NC_MOTION); break;
        }

        if (message == WM_NCMOUSEMOVE) {
            TrackNcLeave();
        }

        x = GET_X_LPARAM(lParam);
        y = GET_Y_LPARAM(lParam);
        wxRect rect = GetClientRect();
        rect.SetPosition(ClientToScreen(rect.GetPosition()));
        x -= rect.x;
        y -= rect.y;

        InitMouseEvent(evnt, x, y, state);
    }
    else {
        m_leaveTrackArmed = false;

        POINT pt;
        wxGetCursorPosMSW(&pt);

        wxRect rect = GetClientRect();
        rect.SetPosition(ClientToScreen(rect.GetPosition()));
        pt.x -= rect.x;
        pt.y -= rect.y;

        evnt.SetEventType(wxEVT_NC_LEAVE);
        InitMouseEvent(evnt, pt.x, pt.y, state);
    }

    wxPostEvent(this, evnt);

    if (wParam == HTMINBUTTON || wParam == HTMAXBUTTON || wParam == HTCLOSE) {
        return 0;
    }

    return wxFrame::MSWWindowProc(message, wParam, lParam);
}

void wxBorderlessFrameMSW::TrackNcLeave()
{
    if (m_leaveTrackArmed) {
        return;
    }

    ::TRACKMOUSEEVENT eventTrack;
    eventTrack.cbSize = sizeof(::TRACKMOUSEEVENT);
    eventTrack.dwFlags = TME_NONCLIENT | TME_LEAVE;
    eventTrack.hwndTrack = GetHWND();
    eventTrack.dwHoverTime = HOVER_DEFAULT;
    if (!_TrackMouseEvent(&eventTrack)) {
        wxLogWarning("_TrackMouseEvent failed");
    }

    m_leaveTrackArmed = true;
}

#endif
