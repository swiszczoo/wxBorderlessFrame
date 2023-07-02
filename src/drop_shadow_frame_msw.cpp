/////////////////////////////////////////////////////////////////////////////
// Name:        drop_shadow_frame_msw.cpp
// Purpose:     wxDropShadowFrame implementation for MSW
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2022-12-28
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

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

    ::UpdateLayeredWindow(GetHWND(), 
        windowHdc, &pptDst, &psize, sourceHdc, &pnt, 0, &bf, ULW_ALPHA);
}

#endif
