/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/system_buttons_fallback.h
// Purpose:     wxSystemButtons fallback implementation (platform-independent)
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2023-04-05
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
 * \file
 * \brief wxSystemButtonsBase fallback implementation (platform-independent).
 */

#ifndef _WXBF_SYSTEM_BUTTONS_FALLBACK_H_
#define _WXBF_SYSTEM_BUTTONS_FALLBACK_H_

#include "borderless_frame_common.h"
#include "system_buttons_base.h"

/**
 * \brief A class that provides fallback implementation of wxSystemButtonsBase.
 * 
 * This implementation is completely platform-independent as it makes use of the
 * bitmap icons embedded into the library. You can still customize all the
 * colors by simply calling SetColourTableEntry().
 */
class BFDLLEXPORT wxFallbackSystemButtons : public wxSystemButtonsBase {
public:
    /**
     * \brief Creates a wxFallbackSystemButtons instance for the provided
     *        frame using passed bitmap data.
     * 
     * \param frame a borderlessframe to create the system buttons for
     * \param bitmapData pointer to a PNG atlas containing system icons
     * \param size size of the passed PNG data
     */
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
