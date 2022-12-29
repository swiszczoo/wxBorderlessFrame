#pragma once
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
