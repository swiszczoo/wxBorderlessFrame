/////////////////////////////////////////////////////////////////////////////
// Name:        system_buttons_fallback.cpp
// Purpose:     wxSystemButtons fallback implementation
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2023-04-05
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include <wxbf/system_buttons_fallback.h>

#include <wx/mstream.h>

const wxCoord wxFallbackSystemButtons::BITMAP_DIMENSION = 64;

wxFallbackSystemButtons::wxFallbackSystemButtons(wxBorderlessFrameBase* frame,
    const void* bitmapData, size_t size)
    : wxSystemButtonsBase(frame)
{
    wxMemoryInputStream mis(bitmapData, size);
    m_iconImg.LoadFile(mis, wxBITMAP_TYPE_PNG);
}

bool wxFallbackSystemButtons::AreButtonsRightAligned() const
{
    return true;
}

wxSize wxFallbackSystemButtons::GetPreferredButtonSize() const
{
    return wxSize(40, 30);
}

wxSize wxFallbackSystemButtons::MeasureButton(wxSystemButton which, wxCoord& margin) const
{
    margin = 1;
    return GetPreferredButtonSize();
}

void wxFallbackSystemButtons::DrawButton(wxDC& dc, wxSystemButton which,
    wxSystemButtonState state, const wxRect& rect)
{
    const wxColour fg = GetColourTableEntry(which, state, wxSB_COLOUR_FOREGROUND);
    const wxColour bg = GetColourTableEntry(which, state, wxSB_COLOUR_BACKGROUND);

    int dimension = std::min(rect.width, rect.height) / 2;

    if (!m_buttonBitmapsCache[which].IsOk()
        || m_buttonBitmapsCache[which].GetWidth() != dimension
        || fg != m_bitmapCacheColours[which])
    {
        TintButtonBitmap(m_buttonBitmapsCache[which], which, dimension, fg);
        m_bitmapCacheColours[which] = fg;
    }

    if (bg.Alpha() > 0) { // Do not draw background if transparent colour is selected
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(wxBrush(bg));
        dc.DrawRectangle(rect);
    }

    wxRect iconRect(0, 0, dimension, dimension);
    iconRect = iconRect.CenterIn(rect);

    dc.DrawBitmap(m_buttonBitmapsCache[which], iconRect.GetPosition());
}

void wxFallbackSystemButtons::TintButtonBitmap(wxBitmap& out,
    int index, wxCoord dimension, wxColour foregroundColour)
{
    wxImage img(wxSize(BITMAP_DIMENSION, BITMAP_DIMENSION), false);
    img.InitAlpha();

    unsigned char* inData = m_iconImg.GetData();
    unsigned char* inAlpha = m_iconImg.GetAlpha();
    unsigned char* outData = img.GetData();
    unsigned char* outAlpha = img.GetAlpha();

    int inOffset = index * BITMAP_DIMENSION;
    int inWidth = m_iconImg.GetWidth();
    for (size_t y = 0; y < BITMAP_DIMENSION; ++y) {
        for (size_t x = 0; x < BITMAP_DIMENSION; ++x) {
            outAlpha[y * BITMAP_DIMENSION + x] = inData[(y * inWidth + (inOffset + x)) * 3];
            outData[(y * BITMAP_DIMENSION + x) * 3 + 0] = foregroundColour.Red();
            outData[(y * BITMAP_DIMENSION + x) * 3 + 1] = foregroundColour.Green();
            outData[(y * BITMAP_DIMENSION + x) * 3 + 2] = foregroundColour.Blue();
        }
    }

    out = img.Scale(dimension, dimension, wxIMAGE_QUALITY_BILINEAR);
}
