/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/system_buttons.h
// Purpose:     wxSystemButtonsFactory
// Author:      £ukasz Œwiszcz
// Modified by:
// Created:     2023-03-27
// Copyright:   (c) £ukasz Œwiszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXBF_SYSTEM_BUTTONS_H_
#define _WXBF_SYSTEM_BUTTONS_H_

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

#endif
