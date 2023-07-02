/////////////////////////////////////////////////////////////////////////////
// Name:        system_buttons_base.cpp
// Purpose:     wxSystemButtonsBase
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2023-03-27
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include <wxbf/borderless_frame.h>
#include <wxbf/system_buttons_base.h>

#include <wx/dcbuffer.h>

wxSystemButtonsBase::wxSystemButtonsBase(wxBorderlessFrameBase* frame)
    : m_owner(frame)
    , m_ownerActive(frame->IsActive())
    , m_buttonSize(-1, -1)
    , m_pressedButton(-1)
{
    InitColourTable();

    for (size_t i = 0; i < 4; ++i)
    {
        m_buttonVisible[i] = false;
        m_lastButtonState[i] = wxSB_STATE_NORMAL;
        m_buttonState[i] = wxSB_STATE_NORMAL;
    }

    frame->Bind(wxEVT_DESTROY, &wxSystemButtonsBase::OnDestroy, this);
    frame->Bind(wxEVT_UPDATE_SYSTEM_BUTTONS, &wxSystemButtonsBase::OnUpdateSystemButtons, this);
    frame->Bind(wxEVT_SIZE, &wxSystemButtonsBase::OnSize, this);
    frame->Bind(wxEVT_LEFT_DOWN, &wxSystemButtonsBase::OnMouse, this);
    frame->Bind(wxEVT_LEFT_UP, &wxSystemButtonsBase::OnMouse, this);
    frame->Bind(wxEVT_MOTION, &wxSystemButtonsBase::OnMouse, this);
    frame->Bind(wxEVT_MOUSE_CAPTURE_LOST, &wxSystemButtonsBase::OnMouseCaptureLost, this);
    frame->Bind(wxEVT_NC_LEFT_DOWN, &wxSystemButtonsBase::OnMouse, this);
    frame->Bind(wxEVT_NC_LEFT_UP, &wxSystemButtonsBase::OnMouse, this);
    frame->Bind(wxEVT_NC_MOTION, &wxSystemButtonsBase::OnMouse, this);
    frame->Bind(wxEVT_NC_LEAVE, &wxSystemButtonsBase::OnMouse, this);
    frame->Bind(wxEVT_ACTIVATE, &wxSystemButtonsBase::OnActivate, this);
    frame->Bind(wxEVT_MAXIMIZE, &wxSystemButtonsBase::OnMaximize, this);
    frame->Bind(wxEVT_PAINT, &wxSystemButtonsBase::OnPaint, this);
#ifdef __WXGTK__
    frame->Bind(wxEVT_LEAVE_WINDOW, &wxSystemButtonsBase::OnMouse, this);
#endif
}

wxSystemButtonsBase::~wxSystemButtonsBase()
{
}

void wxSystemButtonsBase::SetButtonSize(wxSize size)
{
    m_buttonSize = size;
}

wxSize wxSystemButtonsBase::GetButtonSize() const
{
    wxSize result = m_buttonSize;

    if (result.x < 0 || result.y < 0) {
        wxSize preferred = GetPreferredButtonSize();

        if (result.x < 0) {
            result.x = preferred.x;
        }
        
        if (result.y < 0) {
            result.y = preferred.y;
        }
    }

    return result;
}

wxWindowPart wxSystemButtonsBase::GetWindowPart(wxPoint mousePos) const
{
#define IS_HOVER(i) (m_buttonState[i] == wxSB_STATE_HOVER || m_buttonState[i] == wxSB_STATE_PRESSED)

    if (IS_HOVER(wxSB_MINIMIZE)) return wxWP_MINIMIZE_BUTTON;
    if (IS_HOVER(wxSB_MAXIMIZE)) return wxWP_MAXIMIZE_BUTTON;
    if (IS_HOVER(wxSB_RESTORE)) return wxWP_MAXIMIZE_BUTTON;
    if (IS_HOVER(wxSB_CLOSE)) return wxWP_CLOSE_BUTTON;
    int y = m_owner->ScreenToClient(mousePos).y;
    if (0 <= y && y <= m_totalRect.height) return wxWP_TITLEBAR;
    return wxWP_CLIENT_AREA;
}

void wxSystemButtonsBase::UpdateState()
{
    UpdateSystemButtons();
}

void wxSystemButtonsBase::OnDestroy(wxWindowDestroyEvent& evnt)
{
    delete this;
    evnt.Skip();
}

void wxSystemButtonsBase::OnUpdateSystemButtons(wxCommandEvent& evnt)
{
    UpdateSystemButtons();
    evnt.Skip();
}

void wxSystemButtonsBase::OnSize(wxSizeEvent& evnt)
{
    Redraw(true);
    evnt.Skip();
}

void wxSystemButtonsBase::OnMouseCaptureLost(wxMouseCaptureLostEvent& evnt)
{
    m_pressedButton = -1;
    Redraw();
}

void wxSystemButtonsBase::OnMouse(wxMouseEvent& evnt)
{
    wxPoint pos = evnt.GetPosition();

    if ((evnt.GetEventType() == wxEVT_LEAVE_WINDOW || evnt.GetEventType() == wxEVT_NC_LEAVE)
        && !m_owner->HasCapture()) {
            
        pos = wxPoint(-100, -100);
    }

    bool stateChanged = false;
    wxSystemButtonState normalState = m_ownerActive ? wxSB_STATE_NORMAL : wxSB_STATE_INACTIVE;

    if (evnt.LeftUp()) {
        for (size_t i = 0; i < 4; ++i) {
            if (m_buttonState[i] == wxSB_STATE_PRESSED) {
                m_owner->ReleaseMouse();
                m_owner->ExecSystemCommand(static_cast<wxSystemCommand>(i));
                break;
            }
        }
    }

    if (!evnt.LeftIsDown() || evnt.GetEventType() == wxEVT_NC_LEFT_UP) {
        if (m_pressedButton != -1) {
            m_pressedButton = -1;

            if (m_owner->HasCapture()) {
                m_owner->ReleaseMouse();
            }
        }
    }

    if (evnt.GetEventType() == wxEVT_NC_LEFT_DOWN) {
        for (size_t i = 0; i < 4; ++i) {
            if (m_buttonState[i] == wxSB_STATE_DISABLED) {
                continue;
            }

            if (m_buttonRects[i].Contains(pos)) {
                m_pressedButton = i;
                m_buttonState[i] = wxSB_STATE_PRESSED;
                stateChanged = true;
                m_owner->CaptureMouse();
                break;
            }
        }
    }

    for (size_t i = 0; i < 4; ++i) {
        if (m_buttonState[i] == wxSB_STATE_DISABLED) {
            continue;
        }

        wxSystemButtonState targetState;
        if (m_buttonRects[i].Contains(pos)) {
            if (m_pressedButton == i) targetState = wxSB_STATE_PRESSED;
            else if (m_pressedButton == -1) targetState = wxSB_STATE_HOVER;
            else targetState = normalState;
        }
        else {
            targetState = normalState;
        }

        if (m_buttonState[i] != targetState) {
            m_buttonState[i] = targetState;
            stateChanged = true;
        }
    }

    if (stateChanged) {
        Redraw();
    }

    evnt.Skip();
}

void wxSystemButtonsBase::OnActivate(wxActivateEvent& evnt)
{
    m_ownerActive = evnt.GetActive();

    UpdateDisabledActiveState();
    Redraw();
    evnt.Skip();
}

void wxSystemButtonsBase::OnMaximize(wxMaximizeEvent& evnt)
{
    UpdateDisabledActiveState();
    Redraw(true);
    evnt.Skip();
}

void wxSystemButtonsBase::OnPaint(wxPaintEvent& evnt)
{
    wxPaintDC dc(m_owner);
    wxRegion rgn = m_owner->GetUpdateRegion();
    wxRect bbox = rgn.GetBox();

    for (size_t i = 0; i < 4; ++i) {
        if (!m_buttonVisible[i]) {
            continue;
        }

        if (m_buttonRects[i].Intersects(bbox)) {
            DrawButton(dc, static_cast<wxSystemButton>(i), m_buttonState[i], m_buttonRects[i]);
        }
    }

    evnt.Skip();
}

void wxSystemButtonsBase::InitColourTable()
{
    for (int which = 0; which < 4; which++) {
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_NORMAL, 
            wxSB_COLOUR_BACKGROUND, wxColour(255, 255, 255));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_NORMAL, 
            wxSB_COLOUR_FOREGROUND, wxColour(64, 64, 64));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_HOVER, 
            wxSB_COLOUR_BACKGROUND, wxColour(220, 220, 220));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_HOVER, 
            wxSB_COLOUR_FOREGROUND, wxColour(64, 64, 64));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_PRESSED, 
            wxSB_COLOUR_BACKGROUND, wxColour(168, 168, 168));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_PRESSED, 
            wxSB_COLOUR_FOREGROUND, wxColour(64, 64, 64));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_DISABLED, 
            wxSB_COLOUR_BACKGROUND, wxColour(255, 255, 255));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_DISABLED, 
            wxSB_COLOUR_FOREGROUND, wxColour(128, 128, 128));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_INACTIVE,
            wxSB_COLOUR_BACKGROUND, wxColour(255, 255, 255));
        SetColourTableEntry(static_cast<wxSystemButton>(which), wxSB_STATE_INACTIVE,
            wxSB_COLOUR_FOREGROUND, wxColour(64, 64, 64));
    }
}

void wxSystemButtonsBase::UpdateSystemButtons()
{
    UpdateDisabledActiveState();
    Redraw(true);
}

void wxSystemButtonsBase::UpdateDisabledActiveState()
{
    long ownerState = m_owner->GetWindowStyle();
    bool ownerMaximized = m_owner->IsMaximized();
    bool ownerActive = m_ownerActive;

    bool buttonEnabled[4];
    buttonEnabled[wxSB_MINIMIZE] = ownerState & wxMINIMIZE_BOX;
    buttonEnabled[wxSB_MAXIMIZE] = (ownerState & wxMAXIMIZE_BOX) && !ownerMaximized;
    buttonEnabled[wxSB_RESTORE] = (ownerState & wxMAXIMIZE_BOX) && ownerMaximized;
    buttonEnabled[wxSB_CLOSE] = (ownerState & wxCLOSE_BOX);

    if (m_pressedButton >= 0 && !buttonEnabled[m_pressedButton]) {
        m_pressedButton = -1;
    }

    for (size_t i = 0; i < 4; ++i) {
        if (!buttonEnabled[i]) {
            m_buttonState[i] = wxSB_STATE_DISABLED;
        }
        else {
            if (m_buttonState[i] == wxSB_STATE_DISABLED) {
                m_buttonState[i] = wxSB_STATE_NORMAL;
            }

            if (ownerActive && m_buttonState[i] == wxSB_STATE_INACTIVE) {
                m_buttonState[i] = wxSB_STATE_NORMAL;
            }
            if (!ownerActive && m_buttonState[i] == wxSB_STATE_NORMAL) {
                m_buttonState[i] = wxSB_STATE_INACTIVE;
            }
        }
    }
}

void wxSystemButtonsBase::Layout()
{
    UpdateDisabledActiveState();

    wxSize ownerSize = m_owner->GetClientSize();
    long ownerState = m_owner->GetWindowStyle();
    bool ownerMaximized = m_owner->IsMaximized();
    wxRect totalRect;

    m_buttonVisible[wxSB_MINIMIZE] = ownerState & (wxMINIMIZE_BOX | wxMAXIMIZE_BOX);
    m_buttonVisible[wxSB_MAXIMIZE] = (ownerState & (wxMINIMIZE_BOX | wxMAXIMIZE_BOX)) && !ownerMaximized;
    m_buttonVisible[wxSB_RESTORE] = (ownerState & (wxMINIMIZE_BOX | wxMAXIMIZE_BOX)) && ownerMaximized;
    m_buttonVisible[wxSB_CLOSE] = ownerState & (wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX);


    if (AreButtonsRightAligned()) {
        wxCoord x = ownerSize.x;

        for (int i = 3; i >= 0; --i) {
            if (!m_buttonVisible[i]) {
                continue;
            }

            wxCoord margin = 0;
            wxSize btnSize = m_owner->FromDIP(MeasureButton(static_cast<wxSystemButton>(i), margin));
            m_buttonRects[i] = wxRect(x - btnSize.x, 0, btnSize.x, btnSize.y);
            AppendToRect(totalRect, m_buttonRects[i]);
            x -= btnSize.x + margin;
        }
    }
    else {
        wxCoord x = 0;

        for (int i = 0; i < 4; ++i) {
            if (!m_buttonVisible[i]) {
                continue;
            }

            wxCoord margin = 0;
            wxSize btnSize = m_owner->FromDIP(MeasureButton(static_cast<wxSystemButton>(i), margin));
            m_buttonRects[i] = wxRect(x, 0, btnSize.x, btnSize.y);
            AppendToRect(totalRect, m_buttonRects[i]);
            x += btnSize.x + margin;
        }
    }

    m_totalRect = totalRect;
}

void wxSystemButtonsBase::Redraw(bool withLayout)
{
    wxRect redrawRect;

    if (withLayout) {
        if (!m_totalRect.IsEmpty()) {
            redrawRect = m_totalRect;
        }

        Layout();

        AppendToRect(redrawRect, m_totalRect);
    }
    else {
        for (size_t i = 0; i < 4; ++i) {
            if (m_lastButtonState[i] != m_buttonState[i]) {
                AppendToRect(redrawRect, m_buttonRects[i]);
                m_lastButtonState[i] = m_buttonState[i];
            }
        }
    }

    if (!redrawRect.IsEmpty()) {
        m_owner->RefreshRect(redrawRect);
    }
}
