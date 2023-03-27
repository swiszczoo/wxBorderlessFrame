#pragma once
#include <wxbf/system_buttons.h>
#include <wxbf/system_buttons_win10.h>

#include <wx/fontenum.h>

wxSystemButtonsBase* wxSystemButtonsFactory::CreateSystemButtons(wxFrame* frame)
{
    wxPlatformInfo platformInfo;
    
    if (platformInfo.GetOperatingSystemId() == wxOS_WINDOWS) {
        const wxString WINDOWS_ICON_FONT_NAME = wxT("Segoe MDL2 Assets");
        if (wxFontEnumerator::IsValidFacename(WINDOWS_ICON_FONT_NAME)) {
            return new wxWin10SystemButtons(frame);
        }
    }

    // We couldn't find any sufficient implementation
    return nullptr;
}
