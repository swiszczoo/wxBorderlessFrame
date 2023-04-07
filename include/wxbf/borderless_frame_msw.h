#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "borderless_frame_base.h"
#include "borderless_frame_common.h"
#include "drop_shadow_frame_msw.h"
#include "resizer_drop_shadow_frame.h"

class BFDLLEXPORT wxBorderlessFrameMSW : public wxBorderlessFrameBase
{
public:
    wxBorderlessFrameMSW() { Init(); }

    wxBorderlessFrameMSW(wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr));

    bool Create(wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr));

    void SetBorderColour(wxColour borderColour)
    {
        m_borderColour = borderColour;
        UpdateNcArea();
    }

    wxColour GetBorderColour() const { return m_borderColour; }

    void SetBorderThickness(int thicknessPx)
    {
        wxASSERT_MSG(thicknessPx >= 0, "thicknessPx must be non-negative");

        m_borderThickness = thicknessPx;
        UpdateNcArea();
    }

    int GetBorderThickness() const
    {
        return m_borderThickness;
    }

    void PopupSystemMenu();

    virtual WXLRESULT MSWWindowProc(WXUINT message,
        WXWPARAM wParam,
        WXLPARAM lParam) wxOVERRIDE;

    virtual void RunSystemCommand(wxSystemCommand command) wxOVERRIDE;

protected:
    virtual wxWindowPart GetWindowPart(wxPoint mousePosition) const;

private:
    wxColour m_borderColour;
    int m_borderThickness;
    bool m_maximizedTheme;
    bool m_leaveTrackArmed;

    wxResizerDropShadowFrame<wxDropShadowFrameMSW>* m_shadow[4];

    void Init();
    void UpdateNcArea();
    void UpdateSystemMenu(::HMENU sysMenu);
    void AdjustMaximizedClientRect(::HWND window, RECT& rect);
    void UpdateTheme();

    void OnMaximize(wxMaximizeEvent& evnt);
    void OnSize(wxSizeEvent& evnt);

    WXLRESULT HandleNcMessage(WXUINT message, WXWPARAM wParam, WXLPARAM lParam); 
    void TrackNcLeave();
};
