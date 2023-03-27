#pragma once
#include "borderless_frame.h"
#include "borderless_frame_common.h"

enum wxSystemButton {
    wxSB_MINIMIZE,
    wxSB_MAXIMIZE,
    wxSB_RESTORE,
    wxSB_CLOSE,
};

enum wxSystemButtonColourKind {
    wxSB_COLOUR_BACKGROUND,
    wxSB_COLOUR_FOREGROUND,
};

enum wxSystemButtonState {
    wxSB_STATE_INACTIVE,
    wxSB_STATE_DISABLED,
    wxSB_STATE_NORMAL,
    wxSB_STATE_HOVER,
    wxSB_STATE_PRESSED,
};

class BFDLLEXPORT wxSystemButtonsBase {
public:
    explicit wxSystemButtonsBase(wxFrame* frame);

    virtual bool AreButtonsRightAligned() const = 0;
    virtual wxSize GetPreferredButtonSize() const = 0;

    void SetColourTableEntry(wxSystemButton which, wxSystemButtonState state,
        wxSystemButtonColourKind kind, const wxColour& colour)
    {
        m_colorTable[which][state][kind] = colour;
    }

    wxColour& GetColourTableEntry(wxSystemButton which, wxSystemButtonState state,
        wxSystemButtonColourKind kind)
    {
        return m_colorTable[which][state][kind];
    }

protected:
    virtual void OnDestroy(wxWindowDestroyEvent& evnt);
    virtual void OnUpdateSystemButtons(wxCommandEvent& evnt);

private:
    wxColour m_colorTable[4][4][2];

    void InitColourTable();
    void UpdateSystemButtons();
};
