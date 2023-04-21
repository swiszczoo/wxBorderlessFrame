/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/system_buttons_fallback.h
// Purpose:     wxSystemButtons fallback implementation (platform-independent)
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2023-04-05
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXBF_SYSTEM_BUTTONS_FALLBACK_H_
#define _WXBF_SYSTEM_BUTTONS_FALLBACK_H_

#include "borderless_frame_common.h"
#include "system_buttons_base.h"

class BFDLLEXPORT wxFallbackSystemButtons : public wxSystemButtonsBase {
public:
    explicit wxFallbackSystemButtons(wxBorderlessFrameBase* frame, 
        const void* bitmapData, size_t size);

    virtual bool AreButtonsRightAligned() const wxOVERRIDE;
    virtual wxSize GetPreferredButtonSize() const wxOVERRIDE;

protected:
    virtual wxSize MeasureButton(wxSystemButton which, wxCoord& margin) const wxOVERRIDE;
    virtual void DrawButton(wxDC& dc, wxSystemButton which,
        wxSystemButtonState state, const wxRect& rect) wxOVERRIDE;

private:
    static const wxCoord BITMAP_DIMENSION;

    wxImage m_iconImg;

    wxBitmap m_buttonBitmapsCache[4];
    wxColour m_bitmapCacheColours[4];

    void TintButtonBitmap(wxBitmap& out, int index, 
        wxCoord dimension, wxColour foregroundColour);
};

#endif
