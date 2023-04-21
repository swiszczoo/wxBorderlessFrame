/////////////////////////////////////////////////////////////////////////////
// Name:        system_buttons_win10.cpp
// Purpose:     wxSystemButtons implementation for Windows 10+
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2023-03-27
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include <wxbf/system_buttons_win10.h>

const wxString wxWin10SystemButtons::ICON_FAMILY_NAME = "Segoe MDL2 Assets";

const wchar_t wxWin10SystemButtons::ICON_MINIMIZE = L'\xE921'; // ChromeMinimize
const wchar_t wxWin10SystemButtons::ICON_MAXIMIZE = L'\xE922'; // ChromeMaximize
const wchar_t wxWin10SystemButtons::ICON_RESTORE = L'\xE923'; // ChromeRestore
const wchar_t wxWin10SystemButtons::ICON_CLOSE = L'\xE8BB'; // ChromeClose

wxWin10SystemButtons::wxWin10SystemButtons(wxBorderlessFrameBase* frame)
    : wxSystemButtonsBase(frame)
{
    InitColourTable();

    // We use this ugly function because we set antialiasing mode to 4 (grayscale)
    // to prevent cleartype from badly doing its job
    m_systemIconsFont.SetNativeFontInfo("1;7.8;0;0;0;0;500;0;0;0;1;0;0;4;32;" + ICON_FAMILY_NAME);
}

bool wxWin10SystemButtons::AreButtonsRightAligned() const
{
    return true;
}

wxSize wxWin10SystemButtons::GetPreferredButtonSize() const
{
    return wxSize(46, 30);
}

wxSize wxWin10SystemButtons::MeasureButton(wxSystemButton which, wxCoord& margin) const
{
    return GetButtonSize();
}

void wxWin10SystemButtons::DrawButton(wxDC& dc, wxSystemButton which,
    wxSystemButtonState state, const wxRect& rect)
{
    static const wchar_t CHAR_LUT[] = { ICON_MINIMIZE,
        ICON_MAXIMIZE, ICON_RESTORE, ICON_CLOSE };

    wxColour bgColor = GetColourTableEntry(which, state, wxSB_COLOUR_BACKGROUND);
    if (bgColor.Alpha() > 0) { // Do not draw background if transparent colour is selected
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(wxBrush(bgColor));
        dc.DrawRectangle(rect);
    }

    dc.SetTextForeground(GetColourTableEntry(which, state, wxSB_COLOUR_FOREGROUND));
    dc.SetFont(m_systemIconsFont);
    wxString text = CHAR_LUT[which];
    wxSize extent = dc.GetTextExtent(ICON_MAXIMIZE);

    // Draw icons twice because they're much clearer
    dc.DrawText(text, wxRect(extent).CenterIn(rect).GetTopLeft());
    dc.DrawText(text, wxRect(extent).CenterIn(rect).GetTopLeft());
}

void wxWin10SystemButtons::InitColourTable()
{
    for (int which = 0; which < 4; which++) {
        if (which == wxSB_CLOSE) {
            continue;
        }

        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_NORMAL,
            wxSB_COLOUR_BACKGROUND, wxColour(255, 255, 255));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_NORMAL,
            wxSB_COLOUR_FOREGROUND, wxColour(0, 0, 0));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_HOVER,
            wxSB_COLOUR_BACKGROUND, wxColour(229, 229, 229));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_HOVER,
            wxSB_COLOUR_FOREGROUND, wxColour(0, 0, 0));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_PRESSED,
            wxSB_COLOUR_BACKGROUND, wxColour(204, 204, 204));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_PRESSED,
            wxSB_COLOUR_FOREGROUND, wxColour(0, 0, 0));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_DISABLED,
            wxSB_COLOUR_BACKGROUND, wxColour(255, 255, 255));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_DISABLED,
            wxSB_COLOUR_FOREGROUND, wxColour(204, 204, 204));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_INACTIVE,
            wxSB_COLOUR_BACKGROUND, wxColour(255, 255, 255));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_INACTIVE,
            wxSB_COLOUR_FOREGROUND, wxColour(153, 153, 153));
    }

    SetColourTableEntry(wxSB_CLOSE, wxSB_STATE_NORMAL,
        wxSB_COLOUR_BACKGROUND, wxColour(255, 255, 255));
    SetColourTableEntry(wxSB_CLOSE, wxSB_STATE_NORMAL,
        wxSB_COLOUR_FOREGROUND, wxColour(0, 0, 0));
    SetColourTableEntry(wxSB_CLOSE, wxSB_STATE_HOVER,
        wxSB_COLOUR_BACKGROUND, wxColour(232, 17, 35));
    SetColourTableEntry(wxSB_CLOSE, wxSB_STATE_HOVER,
        wxSB_COLOUR_FOREGROUND, wxColour(255, 255, 255)); 
    SetColourTableEntry(wxSB_CLOSE, wxSB_STATE_PRESSED,
        wxSB_COLOUR_BACKGROUND, wxColour(241, 112, 122));
    SetColourTableEntry(wxSB_CLOSE, wxSB_STATE_PRESSED,
        wxSB_COLOUR_FOREGROUND, wxColour(255, 255, 255));
    SetColourTableEntry(wxSB_CLOSE, wxSB_STATE_DISABLED,
        wxSB_COLOUR_BACKGROUND, wxColour(255, 255, 255));
    SetColourTableEntry(wxSB_CLOSE, wxSB_STATE_DISABLED,
        wxSB_COLOUR_FOREGROUND, wxColour(204, 204, 204));
    SetColourTableEntry(wxSB_CLOSE, wxSB_STATE_INACTIVE,
        wxSB_COLOUR_BACKGROUND, wxColour(255, 255, 255));
    SetColourTableEntry(wxSB_CLOSE, wxSB_STATE_INACTIVE,
        wxSB_COLOUR_FOREGROUND, wxColour(153, 153, 153));
}
