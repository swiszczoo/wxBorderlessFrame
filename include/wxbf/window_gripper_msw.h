/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/window_gripper_msw.h
// Purpose:     wxWindowGripper implementation for MSW
// Author:      �ukasz �wiszcz
// Modified by:
// Created:     2022-12-29
// Copyright:   (c) �ukasz �wiszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXBF_WINDOW_GRIPPER_MSW_H_
#define _WXBF_WINDOW_GRIPPER_MSW_H_

#include "borderless_frame_common.h"
#include "window_gripper.h"

class BFDLLEXPORT wxWindowGripperMSW : public wxWindowGripper
{
public:
    virtual bool StartDragMove(wxWindow* window) wxOVERRIDE;
    virtual bool StartDragResize(wxWindow* window, wxDirection which) wxOVERRIDE;

private:
    int GetResizeMsgId(wxDirection dir) const;
};

#endif
