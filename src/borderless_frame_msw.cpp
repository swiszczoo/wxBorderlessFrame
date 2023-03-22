#ifdef _WIN32
#pragma comment(lib, "uxtheme.lib")

#include <wxbf/borderless_frame_msw.h>
#include <wxbf/window_gripper_msw.h>

#include <Uxtheme.h>

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

    return true;
}

void wxBorderlessFrameMSW::PopupSystemMenu()
{
    ::HMENU hSysMenu = GetSystemMenu(GetHWND(), FALSE);
    wxPoint mousePos = wxGetMousePosition();
    int cmd = ::TrackPopupMenu(hSysMenu, TPM_RETURNCMD,
        mousePos.x, mousePos.y, 0, GetHWND(), NULL);

    SendMessage(GetHWND(), WM_SYSCOMMAND, static_cast<WPARAM>(cmd), NULL);
}

WXLRESULT wxBorderlessFrameMSW::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
    switch (message) {
    case WM_CREATE:
        SetWindowTheme(GetHWND(), _T(""), _T(""));
        break;
    case WM_NCACTIVATE:
        return TRUE;
    case WM_NCCALCSIZE:
        if (wParam) {
            if (!IsMaximized()) {
                NCCALCSIZE_PARAMS* csp = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
                int thickness = GetBorderThickness();

                csp->rgrc[0].top += thickness;
                csp->rgrc[0].left += thickness;
                csp->rgrc[0].right -= thickness;
                csp->rgrc[0].bottom -= thickness;
                return 0;
            }
            else {
                // TODO: handle maximize case
                return 0;
            }
        }
        break;
    case WM_NCHITTEST:
    {
        static const int RETURN_VALUES[] = {
            HTCLIENT, HTCAPTION, HTMINBUTTON, HTMAXBUTTON, HTCLOSE };

        return RETURN_VALUES[static_cast<int>(GetWindowPart(wxGetMousePosition()))];
    }
    case WM_NCPAINT:
    {
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
}

wxWindowPart wxBorderlessFrameMSW::GetWindowPart(wxPoint mousePosition) const
{
    return wxWindowPart::CLIENT_AREA;
}

void wxBorderlessFrameMSW::UpdateNcArea()
{
    ::SetWindowPos(GetHWND(), nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
}

#endif
