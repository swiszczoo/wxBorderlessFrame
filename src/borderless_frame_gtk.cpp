/////////////////////////////////////////////////////////////////////////////
// Name:        borderless_frame_gtk.cpp
// Purpose:     wxBorderlessFrame implementation for GTK
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2023-07-02
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __WXGTK__

#include <wxbf/borderless_frame_gtk.h>
#include <wxbf/window_gripper_gtk.h>

#include <gdk/gdk.h>
#include <gtk/gtk.h>

wxBorderlessFrameGTK::wxBorderlessFrameGTK(wxWindow* parent,
    wxWindowID id, const wxString& title, const wxPoint& pos,
    const wxSize& size, long style, const wxString& name)
{
    Init();
    Create(parent, id, title, pos, size, style, name);
}

wxBorderlessFrameGTK::~wxBorderlessFrameGTK()
{
    delete m_gripper;
}

bool wxBorderlessFrameGTK::Create(wxWindow* parent,
    wxWindowID id, const wxString& title, const wxPoint& pos,
    const wxSize& size, long style, const wxString& name)
{
    style |= wxNO_BORDER;

    if (!wxFrame::Create(parent, id, title, pos, size, style, name)) {
        return false;
    }

    

    Bind(wxEVT_LEFT_DOWN, &wxBorderlessFrameGTK::OnMouse, this);
    Bind(wxEVT_LEFT_UP, &wxBorderlessFrameGTK::OnMouse, this);
    Bind(wxEVT_LEFT_DCLICK, &wxBorderlessFrameGTK::OnMouse, this);
    return true;
}

void wxBorderlessFrameGTK::PopupSystemMenu()
{
    // It appears we need to emulate this menu
}

wxWindowPart wxBorderlessFrameGTK::GetWindowPart(wxPoint mousePosition) const
{
    return wxWP_CLIENT_AREA;
}

void wxBorderlessFrameGTK::ExecSystemCommand(wxSystemCommand command)
{
    GtkWindow* window = reinterpret_cast<GtkWindow*>(GetHandle());

    switch (command) {
    case wxSC_MINIMIZE:
        gtk_window_iconify(window);
        return;
    case wxSC_MAXIMIZE:
        gtk_window_maximize(window);
        return;
    case wxSC_RESTORE:
        gtk_window_unmaximize(window);
        return;
    case wxSC_CLOSE:
        gtk_window_close(window);
        return;
    }
}

void wxBorderlessFrameGTK::SetWindowStyleFlag(long style)
{
    long oldStyle = GetWindowStyleFlag();
    static const long MASK = wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxMAXIMIZE | wxCLOSE_BOX;

    wxFrame::SetWindowStyleFlag(style | wxNO_BORDER);

    if ((oldStyle & MASK) != (style & MASK)) {
        wxPostEvent(this, wxCommandEvent(wxEVT_UPDATE_SYSTEM_BUTTONS));
    }
}

void wxBorderlessFrameGTK::Init()
{
    //wxFrame::Init(); // we can't call it here because it's private O_O
    m_borderThickness = 1;
    m_borderColour = *wxRED;
    m_gripper = wxWindowGripper::Create();
}

void wxBorderlessFrameGTK::OnMouse(wxMouseEvent& evnt)
{
    if (evnt.GetEventType() == wxEVT_LEFT_DOWN) {
        wxWindowPart part = GetWindowPart(ClientToScreen(evnt.GetPosition()));
        if (part == wxWP_TITLEBAR) {
            m_gripper->StartDragMove(this);
        }

        if (part != wxWP_CLIENT_AREA) {
            return;
        }
    }
    
    if (evnt.GetEventType() == wxEVT_LEFT_UP) {
        wxWindowPart part = GetWindowPart(ClientToScreen(evnt.GetPosition()));

        if (part != wxWP_CLIENT_AREA) {
            return;
        }
    }

    if (evnt.GetEventType() == wxEVT_LEFT_DCLICK) {
        if (GetWindowPart(ClientToScreen(evnt.GetPosition())) == wxWP_TITLEBAR) {
            ExecSystemCommand(IsMaximized() ? wxSC_RESTORE : wxSC_MAXIMIZE);
            return;
        }
    }

    evnt.Skip();
}

#endif
