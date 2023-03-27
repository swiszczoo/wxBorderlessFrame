#include <wxbf/borderless_frame.h>
#include <wxbf/system_buttons_base.h>

wxSystemButtonsBase::wxSystemButtonsBase(wxFrame* frame)
    : m_buttonSize(-1, -1)
{
    InitColourTable();

    frame->Bind(wxEVT_DESTROY, &wxSystemButtonsBase::OnDestroy, this);
    frame->Bind(wxEVT_UPDATE_SYSTEM_BUTTONS, &wxSystemButtonsBase::OnUpdateSystemButtons, this);
}

void wxSystemButtonsBase::SetButtonSize(wxSize size)
{
    m_buttonSize = size;
}

wxSize wxSystemButtonsBase::GetButtonSize() const
{
    wxSize result = m_buttonSize;

    if (result.x < 0 || result.y < 0) {
        wxSize preferred = GetPreferredButtonSize();

        if (result.x < 0) {
            result.x = preferred.x;
        }
        
        if (result.y < 0) {
            result.y = preferred.y;
        }
    }

    return result;
}

void wxSystemButtonsBase::OnDestroy(wxWindowDestroyEvent& evnt)
{
    delete this;
}

void wxSystemButtonsBase::OnUpdateSystemButtons(wxCommandEvent& evnt)
{
    UpdateSystemButtons();
}

void wxSystemButtonsBase::InitColourTable()
{
    for (int which = 0; which < 4; which++) {
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_NORMAL, 
            wxSB_COLOUR_BACKGROUND, wxColour(255, 255, 255));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_NORMAL, 
            wxSB_COLOUR_FOREGROUND, wxColour(0, 0, 0));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_HOVER, 
            wxSB_COLOUR_BACKGROUND, wxColour(220, 220, 220));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_HOVER, 
            wxSB_COLOUR_FOREGROUND, wxColour(0, 0, 0));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_PRESSED, 
            wxSB_COLOUR_BACKGROUND, wxColour(168, 168, 168));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_PRESSED, 
            wxSB_COLOUR_FOREGROUND, wxColour(0, 0, 0));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_DISABLED, 
            wxSB_COLOUR_BACKGROUND, wxColour(255, 255, 255));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_DISABLED, 
            wxSB_COLOUR_FOREGROUND, wxColour(128, 128, 128));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_INACTIVE,
            wxSB_COLOUR_BACKGROUND, wxColour(255, 255, 255));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_INACTIVE,
            wxSB_COLOUR_FOREGROUND, wxColour(64, 64, 64));
    }
}

void wxSystemButtonsBase::UpdateSystemButtons()
{

}
