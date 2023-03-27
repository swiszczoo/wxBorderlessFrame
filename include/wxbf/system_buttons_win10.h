#pragma once
#include "borderless_frame_common.h"
#include "system_buttons_base.h"

class BFDLLEXPORT wxWin10SystemButtons : public wxSystemButtonsBase
{
public:
    static const wxString ICON_FAMILY_NAME;
    
    explicit wxWin10SystemButtons(wxFrame* frame);

    virtual bool AreButtonsRightAligned() const wxOVERRIDE;
    virtual wxSize GetPreferredButtonSize() const wxOVERRIDE;

protected:
    virtual wxSize MeasureButton(wxSystemButton which) const wxOVERRIDE;

private:
    wxFont m_systemIconsFont;

    void InitColourTable();
};
