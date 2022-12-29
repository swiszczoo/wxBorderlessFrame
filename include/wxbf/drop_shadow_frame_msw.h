#pragma once
#include "borderless_frame_common.h"
#include "drop_shadow_frame_base.h"

class BFDLLEXPORT wxDropShadowFrameMSW : public wxDropShadowFrameBase
{
public:
    wxDropShadowFrameMSW() { Init(); }
    wxDropShadowFrameMSW(wxWindow* parent,
        wxWindowID id,
        wxDropShadowWindowPart part,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr))
    {
        Init();
        Create(parent, id, part, pos, size, style, name);
    }

    bool Create(wxWindow* parent,
        wxWindowID id,
        wxDropShadowWindowPart part,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr));

    virtual bool SetTransparent(wxByte alpha) wxOVERRIDE;

    virtual void AttachToWindow(wxFrame* attachedFrame);
    virtual void DetachFromWindow();

protected:
    virtual void UpdateWindowContents(wxDC& winDc, wxMemoryDC& dc) wxOVERRIDE;

private:
    virtual WXLRESULT MSWWindowProc(WXUINT message,
        WXWPARAM wParam,
        WXLPARAM lParam) wxOVERRIDE;
};
