#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "borderless_frame_base.h"
#include "borderless_frame_common.h"
#include "system_buttons_base.h"

class BFDLLEXPORT wxSystemButtonsFactory {
public:
    static wxSystemButtonsBase* CreateSystemButtons(wxBorderlessFrameBase* frame);
};
