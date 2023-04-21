/////////////////////////////////////////////////////////////////////////////
// Name:        window_gripper_msw.cpp
// Purpose:     wxWindowGripper implementation for MSW
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2022-12-29
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32

#include <wxbf/window_gripper_msw.h>

#define SC_SIZELEFT 0xF001
#define SC_SIZERIGHT 0xF002
#define SC_SIZETOP 0xF003
#define SC_SIZETOPLEFT 0xF004
#define SC_SIZETOPRIGHT 0xF005
#define SC_SIZEBOTTOM 0xF006
#define SC_SIZEBOTTOMLEFT 0xF007
#define SC_SIZEBOTTOMRIGHT 0xF008
#define SC_DRAGMOVE 0xF012

bool wxWindowGripperMSW::StartDragMove(wxWindow* window)
{
    return ::SendMessage(window->GetHWND(), WM_SYSCOMMAND, SC_DRAGMOVE, NULL) == 0;
}

bool wxWindowGripperMSW::StartDragResize(wxWindow* window, wxDirection which)
{
    if (window->GetWindowStyle() & wxRESIZE_BORDER) {
        return ::SendMessage(window->GetHWND(), WM_SYSCOMMAND, GetResizeMsgId(which), NULL) == 0;
    }

    return false;
}

int wxWindowGripperMSW::GetResizeMsgId(wxDirection which) const
{
    switch (which)
    {
    case wxTOP: return SC_SIZETOP;
    case wxBOTTOM: return SC_SIZEBOTTOM;
    case wxLEFT: return SC_SIZELEFT;
    case wxRIGHT: return SC_SIZERIGHT;
    case wxTOP | wxLEFT: return SC_SIZETOPLEFT;
    case wxTOP | wxRIGHT: return SC_SIZETOPRIGHT;
    case wxBOTTOM | wxLEFT: return SC_SIZEBOTTOMLEFT;
    case wxBOTTOM | wxRIGHT: return SC_SIZEBOTTOMRIGHT;
    default: return SC_SIZE;
    }
}

#endif
