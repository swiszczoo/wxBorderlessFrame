/////////////////////////////////////////////////////////////////////////////
// Name:        window_gripper_gtk.cpp
// Purpose:     wxWindowGripper implementation for GTK
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2023-07-01
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __WXGTK__

#include <wxbf/window_gripper_gtk.h>
#include <gtk/gtk.h>

bool wxWindowGripperGTK::StartDragMove(wxWindow* window)
{
    GtkWidget* widget = window->GetHandle();
    wxPoint mousePos = wxGetMousePosition();

    gtk_window_begin_move_drag(reinterpret_cast<GtkWindow*>(widget), 1, 
        mousePos.x, mousePos.y, GDK_CURRENT_TIME);
    return true;
}

static GdkWindowEdge GetWindowEdge(wxDirection which) 
{
    switch (static_cast<int>(which)) {
    case wxTOP: return GDK_WINDOW_EDGE_NORTH;
    case wxLEFT: return GDK_WINDOW_EDGE_WEST;
    case wxRIGHT: return GDK_WINDOW_EDGE_EAST;
    case wxBOTTOM: return GDK_WINDOW_EDGE_SOUTH;
    case wxTOP | wxLEFT: return GDK_WINDOW_EDGE_NORTH_WEST;
    case wxTOP | wxRIGHT: return GDK_WINDOW_EDGE_NORTH_EAST;
    case wxBOTTOM | wxLEFT: return GDK_WINDOW_EDGE_SOUTH_WEST;
    case wxBOTTOM | wxRIGHT: return GDK_WINDOW_EDGE_SOUTH_EAST;
    default: return GDK_WINDOW_EDGE_NORTH_WEST;
    }
}

bool wxWindowGripperGTK::StartDragResize(wxWindow* window, wxDirection which)
{
    if (window->GetWindowStyle() & wxRESIZE_BORDER) {
        GtkWidget* widget = window->GetHandle();
        wxPoint mousePos = wxGetMousePosition();

        gtk_window_begin_resize_drag(reinterpret_cast<GtkWindow*>(widget),
            GetWindowEdge(which), 1, mousePos.x, mousePos.y, GDK_CURRENT_TIME);
        return true;
    }

    return false;
}

#endif
