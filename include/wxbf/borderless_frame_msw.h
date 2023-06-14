/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/borderless_frame_msw.h
// Purpose:     wxBorderlessFrame implementation for MSW
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2022-12-26
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
 * \file
 * \brief wxBorderlessFrame implementation for MSW.
 */

#ifndef _WXBF_BORDERLESS_FRAME_MSW_H_
#define _WXBF_BORDERLESS_FRAME_MSW_H_

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

    wxByte GetShadowAlpha() const
    {
        return m_shadow[0]->GetShadowAlpha();
    }

    void SetShadowAlpha(wxByte newAlpha)
    {
        for (size_t i = 0; i < 4; ++i) m_shadow[i]->SetShadowAlpha(newAlpha);
    }

    int GetShadowSize() const
    {
        return m_shadow[0]->GetShadowSize();
    }

    void SetShadowSize(int shadowSize)
    {
        for (size_t i = 0; i < 4; ++i) m_shadow[i]->SetShadowSize(shadowSize);
    }

    wxPoint GetShadowOffset() const
    {
        return m_shadow[0]->GetShadowOffset();
    }

    void SetShadowOffset(wxPoint newOffset)
    {
        for (size_t i = 0; i < 4; ++i) m_shadow[i]->SetShadowOffset(newOffset);
    }

    bool IsShadowDisabledOnInactiveWindow() const
    {
        return m_shadow[0]->IsDisableShadowOnInactiveWindow();
    }

    void SetShadowDisabledOnInactiveWindow(bool disable)
    {
        for (size_t i = 0; i < 4; ++i) m_shadow[i]->SetDisableShadowOnInactiveWindow(disable);
    }

    void PopupSystemMenu();

    virtual WXLRESULT MSWWindowProc(WXUINT message,
        WXWPARAM wParam,
        WXLPARAM lParam) wxOVERRIDE;

    virtual void ExecSystemCommand(wxSystemCommand command) wxOVERRIDE;

    void SetWindowStyleFlag(long style) wxOVERRIDE;

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
    void OnIconize(wxIconizeEvent& evnt);
    void OnSize(wxSizeEvent& evnt);

    WXLRESULT HandleNcMessage(WXUINT message, WXWPARAM wParam, WXLPARAM lParam); 
    void TrackNcLeave();
};

#endif
