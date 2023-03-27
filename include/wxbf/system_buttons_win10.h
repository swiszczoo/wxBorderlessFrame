#pragma once
#include "borderless_frame_common.h"
#include "system_buttons_base.h"

class BFDLLEXPORT wxWin10SystemButtons : public wxSystemButtonsBase
{
public:
    explicit wxWin10SystemButtons(wxFrame* frame);

    virtual bool AreButtonsRightAligned() const wxOVERRIDE;
    virtual wxSize GetPreferredButtonSize() const wxOVERRIDE;

private:
    void InitColourTable();
};
