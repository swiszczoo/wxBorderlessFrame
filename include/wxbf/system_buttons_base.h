#pragma once
#include "borderless_frame_base.h"
#include "borderless_frame_common.h"

enum wxSystemButton {
    wxSB_MINIMIZE,
    wxSB_MAXIMIZE,
    wxSB_RESTORE,
    wxSB_CLOSE,
};

enum wxSystemButtonColourKind {
    wxSB_COLOUR_BACKGROUND,
    wxSB_COLOUR_FOREGROUND,
};

enum wxSystemButtonState {
    wxSB_STATE_INACTIVE,
    wxSB_STATE_DISABLED,
    wxSB_STATE_NORMAL,
    wxSB_STATE_HOVER,
    wxSB_STATE_PRESSED,
};

class BFDLLEXPORT wxSystemButtonsBase {
public:
    explicit wxSystemButtonsBase(wxBorderlessFrameBase* frame);
    virtual ~wxSystemButtonsBase();

    virtual bool AreButtonsRightAligned() const = 0;
    virtual wxSize GetPreferredButtonSize() const = 0;

    void SetColourTableEntry(wxSystemButton which, wxSystemButtonState state,
        wxSystemButtonColourKind kind, const wxColour& colour)
    {
        m_colourTable[which][state][kind] = colour;
    }

    wxColour& GetColourTableEntry(wxSystemButton which, wxSystemButtonState state,
        wxSystemButtonColourKind kind)
    {
        return m_colourTable[which][state][kind];
    }

    void SetButtonSize(wxSize size);
    wxSize GetButtonSize() const;

    wxSize GetTotalSize() const
    {
        return m_totalRect.GetSize();
    }

    wxWindowPart GetWindowPart(wxPoint mousePos) const;
    virtual void UpdateState();

protected:
    virtual wxSize MeasureButton(wxSystemButton which, wxCoord& margin) const = 0;
    virtual void DrawButton(wxDC& dc, wxSystemButton which,
        wxSystemButtonState state, const wxRect& rect) = 0;

    virtual void OnDestroy(wxWindowDestroyEvent& evnt);
    virtual void OnUpdateSystemButtons(wxCommandEvent& evnt);
    virtual void OnSize(wxSizeEvent& evnt);
    virtual void OnMouseCaptureLost(wxMouseCaptureLostEvent& evnt);
    virtual void OnMouse(wxMouseEvent& evnt);
    virtual void OnActivate(wxActivateEvent& evnt);
    virtual void OnMaximize(wxMaximizeEvent& evnt);
    virtual void OnPaint(wxPaintEvent& evnt);

private:
    wxBorderlessFrameBase* m_owner;
    bool m_ownerActive;

    wxColour m_colourTable[4][5][2];
    wxSize m_buttonSize;
    wxRect m_totalRect;
    wxRect m_buttonRects[4];
    wxSystemButtonState m_lastButtonState[4];
    wxSystemButtonState m_buttonState[4];
    bool m_buttonVisible[4];
    int m_pressedButton;

    void InitColourTable();
    void UpdateSystemButtons();
    void UpdateDisabledActiveState();
    void Layout();
    void Redraw(bool withLayout = false);

    static void AppendToRect(wxRect& rect, wxRect toAppend) 
    {
        if (rect.IsEmpty()) {
            rect = toAppend;
        }
        else {
            rect += toAppend;
        }
    }
};
