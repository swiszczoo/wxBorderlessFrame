/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/drop_shadow_frame_msw.h
// Purpose:     wxDropShadowFrame implementation for MSW
// Author:      £ukasz Œwiszcz
// Modified by:
// Created:     2022-12-27
// Copyright:   (c) £ukasz Œwiszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXBF_DROP_SHADOW_FRAME_MSW_H_
#define _WXBF_DROP_SHADOW_FRAME_MSW_H_

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
};

#endif
