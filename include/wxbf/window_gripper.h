/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/window_gripper.h
// Purpose:     wxWindowGripper abstract class
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2022-12-29
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXBF_WINDOW_GRIPPER_H_
#define _WXBF_WINDOW_GRIPPER_H_

#include "borderless_frame_common.h"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class BFDLLEXPORT wxWindowGripper
{
public:
    virtual bool StartDragMove(wxWindow* window) = 0;
    virtual bool StartDragResize(wxWindow* window, wxDirection which) = 0;

    static wxWindowGripper* Create();
};

#endif
