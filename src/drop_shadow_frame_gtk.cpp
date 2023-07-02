/////////////////////////////////////////////////////////////////////////////
// Name:        drop_shadow_frame_gtk.cpp
// Purpose:     wxDropShadowFrame implementation for GTK
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2022-12-28
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __WXGTK__

#include <wxbf/drop_shadow_frame_gtk.h>


void wxDropShadowFrameGTK::OnGtkScreenChanged(
    GtkWidget *widget, GdkScreen *old_screen, gpointer userdata)
{
    /* To check if the display supports alpha channels, get the colormap */
    GdkScreen *screen = gtk_widget_get_screen(widget);
    GdkVisual *visual = gdk_screen_get_rgba_visual(screen);

    if (!visual)
    {
        static_cast<wxDropShadowFrameGTK*>(userdata)->m_supportsAlpha = false;
    }
    else
    {
        static_cast<wxDropShadowFrameGTK*>(userdata)->m_supportsAlpha = true;
    }

    /* Now we have a colormap appropriate for the screen, use it */
    gtk_widget_set_visual(widget, visual);
}

bool wxDropShadowFrameGTK::Create(wxWindow* parent,
    wxWindowID id, wxDropShadowWindowPart part, const wxPoint& pos,
    const wxSize& size, long style, const wxString& name)
{
    if (!wxDropShadowFrameBase::Create(parent, id, part, pos, size, style, name)) {
        return false;
    }

    GtkWidget* window = GetHandle();
    gtk_widget_set_app_paintable(GetHandle(), true);
    g_signal_connect(G_OBJECT(window), "screen-changed", G_CALLBACK(OnGtkScreenChanged), this);

    OnGtkScreenChanged(window, NULL, this);

    Bind(wxEVT_PAINT, &wxDropShadowFrameGTK::OnPaint, this);
    return true;
}

void wxDropShadowFrameGTK::AttachToWindow(wxFrame* attachedFrame)
{
    wxDropShadowFrameBase::AttachToWindow(attachedFrame);

    GtkWindow* window = reinterpret_cast<GtkWindow*>(GetHandle());
    GtkWindow* parent = reinterpret_cast<GtkWindow*>(attachedFrame->GetHandle());
    gtk_window_set_transient_for(window, parent);
}

void wxDropShadowFrameGTK::DetachFromWindow()
{
    wxDropShadowFrameBase::DetachFromWindow();

    GtkWindow* window = reinterpret_cast<GtkWindow*>(GetHandle());
    gtk_window_set_transient_for(window, NULL);
}

void wxDropShadowFrameGTK::UpdateWindowContents(wxDC& winDc, wxMemoryDC& dc)
{
    Refresh();
}

void wxDropShadowFrameGTK::Init()
{
    m_supportsAlpha = false;
}

void wxDropShadowFrameGTK::OnPaint(wxPaintEvent& evnt)
{
    wxPaintDC dc(this);
    //dc.SetBrush(*wxWHITE_BRUSH);
    //dc.SetPen(*wxTRANSPARENT_PEN);
    //dc.DrawRectangle(dc.GetSize());

    dc.DrawBitmap(GetShadowBitmap(), 0, 0);
}

#endif
