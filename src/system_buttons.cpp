#pragma once
#include <wxbf/system_buttons.h>
#include <wxbf/system_buttons_win10.h>

#include <wx/fontenum.h>

wxSystemButtonsBase* wxSystemButtonsFactory::CreateSystemButtons(wxFrame* frame)
{
    wxPlatformInfo platformInfo;
    
    if (platformInfo.GetOperatingSystemId() == wxOS_WINDOWS_NT) {
        if (wxFontEnumerator::IsValidFacename(wxWin10SystemButtons::ICON_FAMILY_NAME)) {
            return new wxWin10SystemButtons(frame);
        }
    }

    // We couldn't find any sufficient implementation
    return nullptr;
}
