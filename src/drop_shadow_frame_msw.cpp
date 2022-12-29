#ifdef _WIN32
#include <wxbf/drop_shadow_frame_msw.h>

#include <wx/msw/private/winstyle.h>

bool wxDropShadowFrameMSW::Create(wxWindow* parent,
    wxWindowID id, wxDropShadowWindowPart part, const wxPoint& pos,
    const wxSize& size, long style, const wxString& name)
{
    if (!wxDropShadowFrameBase::Create(parent, id, part, pos, size, style, name)) {
        return false;
    }

    wxMSWWinExStyleUpdater updateExStyle(GetHWND());
    updateExStyle.TurnOn(WS_EX_LAYERED).Apply();
    return true;
}

bool wxDropShadowFrameMSW::SetTransparent(wxByte alpha)
{
    return false;
}

void wxDropShadowFrameMSW::AttachToWindow(wxFrame* attachedFrame)
{
    wxDropShadowFrameBase::AttachToWindow(attachedFrame);

    ::SetWindowLongPtr(GetHWND(), GWLP_HWNDPARENT, reinterpret_cast<LONG_PTR>(attachedFrame->GetHWND()));
}

void wxDropShadowFrameMSW::DetachFromWindow()
{
    wxDropShadowFrameBase::DetachFromWindow();

    ::SetWindowLongPtr(GetHWND(), GWLP_HWNDPARENT, NULL);
}

void wxDropShadowFrameMSW::UpdateWindowContents(wxDC& winDc, wxMemoryDC& dc)
{
    static ::POINT pnt = {0};

    POINT pptDst = { GetPosition().x, GetPosition().y };
    SIZE psize = { GetSize().x, GetSize().y };

    BLENDFUNCTION bf = { 0 };
    bf.BlendOp = AC_SRC_OVER;
    bf.SourceConstantAlpha = 255;
    bf.AlphaFormat = AC_SRC_ALPHA;

    HDC windowHdc = winDc.GetHDC();
    HDC sourceHdc = dc.GetHDC();

    BOOL result = ::UpdateLayeredWindow(GetHWND(), 
        windowHdc, &pptDst, &psize, sourceHdc, &pnt, 0, &bf, ULW_ALPHA);
    DWORD error = GetLastError();
    error;
}

WXLRESULT wxDropShadowFrameMSW::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
    WINDOWPOS* wpos;

    switch (message) {
    case WM_WINDOWPOSCHANGING:
        return wxDropShadowFrameBase::MSWWindowProc(message, wParam, lParam);
        if (GetAttachedWindow()) {
            wpos = reinterpret_cast<WINDOWPOS*>(lParam);
            wpos->hwndInsertAfter = GetAttachedWindow()->GetHWND();
            wpos->flags &= ~SWP_NOZORDER;
            return 0;
        }
    }

    return wxDropShadowFrameBase::MSWWindowProc(message, wParam, lParam);
}

#endif
