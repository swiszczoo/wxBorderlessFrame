#ifdef _WIN32
#pragma comment(lib, "uxtheme.lib")

#include <wxbf/borderless_frame_msw.h>
#include <wxbf/window_gripper_msw.h>

#include <Uxtheme.h>

wxDEFINE_EVENT(wxEVT_UPDATE_SYSTEM_BUTTONS, wxCommandEvent);

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

    Bind(wxEVT_MAXIMIZE, &wxBorderlessFrameMSW::OnMaximize, this);
    Bind(wxEVT_SIZE, &wxBorderlessFrameMSW::OnSize, this);

    return true;
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
        if (IsMaximized()) {
            return 0;
        }

        wxWindowDC dc(this);
        if (m_borderThickness > 0) {
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
    default:
        return wxFrame::MSWWindowProc(message, wParam, lParam);
    }

    return wxFrame::MSWWindowProc(message, wParam, lParam);
}

void wxBorderlessFrameMSW::Init()
{
    wxFrame::Init();
    m_borderThickness = 1;
    m_borderColour = *wxRED;
    m_maximizedTheme = true;
}

wxWindowPart wxBorderlessFrameMSW::GetWindowPart(wxPoint mousePosition) const
{
    return wxWindowPart::CLIENT_AREA;
}

void wxBorderlessFrameMSW::UpdateNcArea()
{
    ::SetWindowPos(GetHWND(), nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
}

void wxBorderlessFrameMSW::UpdateSystemMenu(::HMENU sysMenu)
{
#define ITEM_STATE(pred) (pred) ? MF_ENABLED : MFS_GRAYED

    bool maximized = IsMaximized();
    long styles = GetWindowStyle();

    ::EnableMenuItem(sysMenu, SC_RESTORE, ITEM_STATE(maximized && (styles & wxMAXIMIZE_BOX)));
    ::EnableMenuItem(sysMenu, SC_MOVE, ITEM_STATE(!maximized));
    ::EnableMenuItem(sysMenu, SC_SIZE, ITEM_STATE(!maximized && (styles & wxRESIZE_BORDER)));
    ::EnableMenuItem(sysMenu, SC_MINIMIZE, ITEM_STATE(styles & wxMINIMIZE_BOX));
    ::EnableMenuItem(sysMenu, SC_MAXIMIZE, ITEM_STATE(!maximized && (styles & wxMAXIMIZE_BOX)));
    ::EnableMenuItem(sysMenu, SC_CLOSE, ITEM_STATE(styles & wxCLOSE_BOX));
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
        SetWindowTheme(GetHWND(), NULL, NULL);
    }
    else {
        SetWindowTheme(GetHWND(), L"", L"");
    }

    m_maximizedTheme = desiredTheme;
}

void wxBorderlessFrameMSW::OnMaximize(wxMaximizeEvent& evnt)
{
    UpdateTheme();
    evnt.Skip();
}

void wxBorderlessFrameMSW::OnSize(wxSizeEvent& evnt)
{
    UpdateTheme();
    evnt.Skip();
}

#endif
