/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/borderless_frame_base.h
// Purpose:     wxBorderlessFrameBase class
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2023-03-29
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXBF_BORDERLESS_FRAME_BASE_H_
#define _WXBF_BORDERLESS_FRAME_BASE_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "borderless_frame_common.h"

wxDECLARE_EXPORTED_EVENT(BFDLLEXPORT, wxEVT_UPDATE_SYSTEM_BUTTONS, wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(BFDLLEXPORT, wxEVT_NC_LEFT_DOWN, wxMouseEvent);
wxDECLARE_EXPORTED_EVENT(BFDLLEXPORT, wxEVT_NC_LEFT_UP, wxMouseEvent);
wxDECLARE_EXPORTED_EVENT(BFDLLEXPORT, wxEVT_NC_RIGHT_DOWN, wxMouseEvent);
wxDECLARE_EXPORTED_EVENT(BFDLLEXPORT, wxEVT_NC_RIGHT_UP, wxMouseEvent);
wxDECLARE_EXPORTED_EVENT(BFDLLEXPORT, wxEVT_NC_MOTION, wxMouseEvent);
wxDECLARE_EXPORTED_EVENT(BFDLLEXPORT, wxEVT_NC_LEAVE, wxMouseEvent);

enum wxSystemCommand
{
    wxSC_MINIMIZE,
    wxSC_MAXIMIZE,
    wxSC_RESTORE,
    wxSC_CLOSE
};

enum wxWindowPart {
    wxWP_CLIENT_AREA,
    wxWP_TITLEBAR,
    wxWP_MINIMIZE_BUTTON,
    wxWP_MAXIMIZE_BUTTON,
    wxWP_CLOSE_BUTTON
};

class BFDLLEXPORT wxBorderlessFrameBase : public wxFrame
{
public:
    using wxFrame::wxFrame;

    virtual void RunSystemCommand(wxSystemCommand command) = 0;
};

#endif
