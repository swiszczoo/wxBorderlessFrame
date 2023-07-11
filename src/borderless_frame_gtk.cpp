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
    if (!wxFrame::Create(parent, id, title, pos, size, style, name)) {
        return false;
    }

    GtkWidget* header = gtk_header_bar_new();
    gtk_window_set_titlebar(GTK_WINDOW(GetHandle()), header);

    // We need to reset window label after replacing titlebar
    gtk_header_bar_set_title(GTK_HEADER_BAR(header), title.utf8_str());

    Bind(wxEVT_LEFT_DOWN, &wxBorderlessFrameGTK::OnMouse, this);
    Bind(wxEVT_LEFT_UP, &wxBorderlessFrameGTK::OnMouse, this);
    Bind(wxEVT_LEFT_DCLICK, &wxBorderlessFrameGTK::OnMouse, this);
    Bind(wxEVT_RIGHT_DOWN, &wxBorderlessFrameGTK::OnMouse, this);
    Bind(wxEVT_RIGHT_UP, &wxBorderlessFrameGTK::OnMouse, this);
    return true;
}

void wxBorderlessFrameGTK::PopupSystemMenu()
{
    GdkWindow* window = gtk_widget_get_window(GetHandle());
    GdkEvent* event = gdk_event_new(GdkEventType::GDK_BUTTON_RELEASE);

    GdkSeat* seat = gdk_display_get_default_seat(gdk_display_get_default());
    GdkDevice* mouse = gdk_seat_get_pointer(seat);
    int x, y;
    gdk_device_get_position(mouse, nullptr, &x, &y);

    event->button.window = window;
    event->button.send_event = true;
    event->button.time = GDK_CURRENT_TIME;
    event->button.axes = NULL;
    event->button.button = 3;
    event->button.x_root = static_cast<gdouble>(x);
    event->button.y_root = static_cast<gdouble>(y);
    event->button.device = mouse;
    
    gdk_window_show_window_menu(window, event);
    gdk_event_free(event);
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

    wxFrame::SetWindowStyleFlag(style);

    if ((oldStyle & MASK) != (style & MASK)) {
        wxPostEvent(this, wxCommandEvent(wxEVT_UPDATE_SYSTEM_BUTTONS));
    }
}

void wxBorderlessFrameGTK::Init()
{
    //wxFrame::Init(); // we can't call it here because it's private O_O
    m_borderThickness = 1;
    m_borderColour = wxColour(168, 168, 168);
    m_gripper = wxWindowGripper::Create();

    m_shadowSize = 10;
    m_shadowOffset = wxPoint(0, 2);
    m_shadowAlpha = 168;
    m_disableShadowOnInactive = true;

    m_cssProvider = gtk_css_provider_new();
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), 
        GTK_STYLE_PROVIDER(m_cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    UpdateDecorationCss();
}

void wxBorderlessFrameGTK::UpdateDecorationCss()
{
    static const wxString FORMAT_STRING = 
        "decoration { "
            "border: %dpx solid %s; "
            "border-radius: 0; "
            "box-shadow: %dpx %dpx %fpx rgba(0, 0, 0, %f); "
        "}";

    static const wxString FORMAT_STRING_NO_INACTIVE = 
        "decoration { "
            "border: %dpx solid %s; "
            "border-radius: 0; "
            "box-shadow: %dpx %dpx %fpx rgba(0, 0, 0, %f); "
        "} "
        ""
        "decoration:backdrop { "
            "box-shadow: %3$dpx %4$dpx %5$fpx transparent; "
        "}";

    wxString formatted = wxString::Format(
        m_disableShadowOnInactive ? FORMAT_STRING_NO_INACTIVE : FORMAT_STRING,
        m_borderThickness,
        m_borderColour.GetAsString(wxC2S_HTML_SYNTAX),
        m_shadowOffset.x,
        m_shadowOffset.y,
        m_shadowSize / 2.f,
        m_shadowAlpha / 255.f
    );

    gtk_css_provider_load_from_data(m_cssProvider, formatted.utf8_str(), -1, nullptr);
}

void wxBorderlessFrameGTK::OnMouse(wxMouseEvent& evnt)
{
    if (evnt.GetEventType() == wxEVT_LEFT_DOWN) {
        wxWindowPart part = GetWindowPart(ClientToScreen(evnt.GetPosition()));
        if (part == wxWP_TITLEBAR) {
            m_gripper->StartDragMove(this);
        }

        if (part != wxWP_CLIENT_AREA) {
            evnt.SetEventType(wxEVT_NC_LEFT_DOWN);
            wxPostEvent(this, evnt);
            return;
        }
    }
    
    if (evnt.GetEventType() == wxEVT_LEFT_UP) {
        wxWindowPart part = GetWindowPart(ClientToScreen(evnt.GetPosition()));

        if (part != wxWP_CLIENT_AREA) {
            evnt.SetEventType(wxEVT_NC_LEFT_UP);
            wxPostEvent(this, evnt);
            return;
        }
    }

    if (evnt.GetEventType() == wxEVT_LEFT_DCLICK) {
        if (GetWindowPart(ClientToScreen(evnt.GetPosition())) == wxWP_TITLEBAR) {
            ExecSystemCommand(IsMaximized() ? wxSC_RESTORE : wxSC_MAXIMIZE);
            return;
        }
    }

    if (evnt.GetEventType() == wxEVT_RIGHT_DOWN) {
        if (GetWindowPart(ClientToScreen(evnt.GetPosition())) != wxWP_CLIENT_AREA) {
            evnt.SetEventType(wxEVT_NC_RIGHT_DOWN);
            wxPostEvent(this, evnt);
            return;
        }
    }

    if (evnt.GetEventType() == wxEVT_RIGHT_UP) {
        if (GetWindowPart(ClientToScreen(evnt.GetPosition())) != wxWP_CLIENT_AREA) {
            evnt.SetEventType(wxEVT_NC_RIGHT_UP);
            wxPostEvent(this, evnt);
            return;
        }
    }

    evnt.Skip();
}

#endif
