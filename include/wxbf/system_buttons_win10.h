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

/**
 * \brief A class that provides wxSystemButtonsBase implementation that
 *        mimicks look-and-feel of Windows 10 system buttons.
 * 
 * Although this implementation contains a hardcoded colour table, it still
 * utilizes a Windows-only font called *Segoe MDL2 Assets* 
 * (see \ref ICON_FAMILY_NAME attribute), so it can't be directly 
 * used on other platforms. 
 * 
 * This is the default implementation constructed by
 * \ref wxSystemButtonsFactory::CreateSystemButtons() if running on Windows 10+
 */
class BFDLLEXPORT wxWin10SystemButtons : public wxSystemButtonsBase
{
public:
    static const wxString ICON_FAMILY_NAME; //!< Name of the font family that
                                            //!< contains system button icons
    
    /**
     * \brief Creates a wxWin10SystemButtons instance.
     * 
     * \param frame a borderlessframe to create the system buttons for
     */
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
