#pragma once
#include "borderless_frame_common.h"
#include "drop_shadow_frame_msw.h"
#include "resizer_drop_shadow_frame.h"

#include <wx/wx.h>

class BFDLLEXPORT wxBorderlessFrameMSW : public wxFrame
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

    virtual WXLRESULT MSWWindowProc(WXUINT message,
        WXWPARAM wParam,
        WXLPARAM lParam) wxOVERRIDE;

private:
    wxColour m_borderColour;
    int m_borderThickness;

    wxResizerDropShadowFrame<wxDropShadowFrameMSW>* m_shadow[4];

    void Init();
    void UpdateNcArea();
};
