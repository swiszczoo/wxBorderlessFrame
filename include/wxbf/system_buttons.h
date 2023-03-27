#pragma once
#include "borderless_frame_common.h"
#include "system_buttons_base.h"

#include <wx/wx.h>

class BFDLLEXPORT wxSystemButtonsFactory {
public:
    static wxSystemButtonsBase* CreateSystemButtons(wxFrame* frame);
};