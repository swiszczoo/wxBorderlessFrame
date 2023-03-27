#include <wxbf/system_buttons_win10.h>

const wxString wxWin10SystemButtons::ICON_FAMILY_NAME = "Segoe MDL2 Assets";

wxWin10SystemButtons::wxWin10SystemButtons(wxFrame* frame)
    : wxSystemButtonsBase(frame)
{
    InitColourTable();

    m_systemIconsFont.Create(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
        wxFONTWEIGHT_SEMIBOLD, false, ICON_FAMILY_NAME);
}

bool wxWin10SystemButtons::AreButtonsRightAligned() const
{
    return true;
}

wxSize wxWin10SystemButtons::GetPreferredButtonSize() const
{
    return wxSize(45, 29);
}

wxSize wxWin10SystemButtons::MeasureButton(wxSystemButton which) const
{
    return GetPreferredButtonSize();
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
