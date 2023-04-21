/////////////////////////////////////////////////////////////////////////////
// Name:        system_buttons.cpp
// Purpose:     wxSystemButtonsFactory
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2023-03-27
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "system_menu_png.h"

#include <wxbf/system_buttons.h>
#include <wxbf/system_buttons_fallback.h>
#include <wxbf/system_buttons_win10.h>

#include <wx/fontenum.h>

wxSystemButtonsBase* wxSystemButtonsFactory::CreateSystemButtons(wxBorderlessFrameBase* frame)
{
    wxPlatformInfo platformInfo;
    
    if (platformInfo.GetOperatingSystemId() == wxOS_WINDOWS_NT) {
        if (wxFontEnumerator::IsValidFacename(wxWin10SystemButtons::ICON_FAMILY_NAME)) {
            return new wxWin10SystemButtons(frame);
        }
    }
    
    // If everything failed, use fallback implementation
    return new wxFallbackSystemButtons(frame, SYSTEM_MENU_DATA, SYSTEM_MENU_SIZE);
}
