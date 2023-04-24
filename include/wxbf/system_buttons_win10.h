/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/system_buttons_win10.h
// Purpose:     wxSystemButtons implementation for Windows 10+
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2023-03-27
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
 * \file
 * \brief wxSystemButtonsBase implementation for Windows 10+.
 */

#ifndef _WXBF_SYSTEM_BUTTONS_WIN10_H_
#define _WXBF_SYSTEM_BUTTONS_WIN10_H_

#include "borderless_frame_common.h"
#include "system_buttons_base.h"

class BFDLLEXPORT wxWin10SystemButtons : public wxSystemButtonsBase
{
public:
    static const wxString ICON_FAMILY_NAME;
    
    explicit wxWin10SystemButtons(wxBorderlessFrameBase* frame);

    virtual bool AreButtonsRightAligned() const wxOVERRIDE;
    virtual wxSize GetPreferredButtonSize() const wxOVERRIDE;

protected:
    virtual wxSize MeasureButton(wxSystemButton which, wxCoord& margin) const wxOVERRIDE;
    virtual void DrawButton(wxDC& dc, wxSystemButton which,
        wxSystemButtonState state, const wxRect& rect) wxOVERRIDE;

private:
    static const wchar_t ICON_MINIMIZE;
    static const wchar_t ICON_MAXIMIZE;
    static const wchar_t ICON_RESTORE;
    static const wchar_t ICON_CLOSE;

    wxFont m_systemIconsFont;

    void InitColourTable();
};

#endif
