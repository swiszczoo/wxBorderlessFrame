#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class wxWindowGripper
{
public:
    virtual bool StartDragMove(wxWindow* window) = 0;
    virtual bool StartDragResize(wxWindow* window, wxDirection which) = 0;
};
