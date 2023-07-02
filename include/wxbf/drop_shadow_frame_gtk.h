/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/drop_shadow_frame_gtk.h
// Purpose:     wxDropShadowFrame implementation for GTK
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2023-07-02
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
 * \file
 * \brief wxDropShadowFrameBase implementation for GTK.
 */

#ifndef _WXBF_DROP_SHADOW_FRAME_GTK_H_
#define _WXBF_DROP_SHADOW_FRAME_GTK_H_

#include "borderless_frame_common.h"
#include "drop_shadow_frame_base.h"

#include <gdk/gdk.h>
#include <gtk/gtk.h>

/**
 * \brief A platform specific implementation of wxDropShadowFrameBase for GTK.
 */
class BFDLLEXPORT wxDropShadowFrameGTK : public wxDropShadowFrameBase
{
public:
    /**
     * \brief Default constructor.
     */
    wxDropShadowFrameGTK() { Init(); }

    /**
     * \brief Constructor, creating a platform-specific wxDropShadowFrameGTK 
     *        instance.
     *
     * \param parent Pointer to a parent window.
     * \param id Window identifier. If wxID_ANY, will automatically create an identifier.
     * \param part Edge of the window this shadow frame corresponds to.
     * \param pos Window position. wxDefaultPosition indicates that wxWidgets
     *            should generate a default position for the window. If using
     *            the wxWindow class directly, supply an actual position.
     * \param size Window size. wxDefaultSize indicates that wxWidgets should
     *             generate a default size for the window. If no suitable size
     *             can be found, the window will be sized to 20x20 pixels so
     *             that the window is visible but obviously not correctly sized.
     * \param style Window style. For generic window styles, please see wxWindow.
     * \param name Window name.
     */
    wxDropShadowFrameGTK(wxWindow* parent,
        wxWindowID id,
        wxDropShadowWindowPart part,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr))
    {
        Init();
        Create(parent, id, part, pos, size, style, name);
    }

    /**
     * \brief Creates a wxDropShadowFrameGTK instance.
     *
     * Call this method only if constructed with default constructor.
     *
     * \param parent Pointer to a parent window.
     * \param id Window identifier. If wxID_ANY, will automatically create an identifier.
     * \param part Edge of the window this shadow frame corresponds to.
     * \param pos Window position. wxDefaultPosition indicates that wxWidgets
     *            should generate a default position for the window. If using
     *            the wxWindow class directly, supply an actual position.
     * \param size Window size. wxDefaultSize indicates that wxWidgets should
     *             generate a default size for the window. If no suitable size
     *             can be found, the window will be sized to 20x20 pixels so
     *             that the window is visible but obviously not correctly sized.
     * \param style Window style. For generic window styles, please see wxWindow.
     * \param name Window name.
     * \return `true`, if creation was successful, `false` otherwise
     */
    bool Create(wxWindow* parent,
        wxWindowID id,
        wxDropShadowWindowPart part,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr));


    virtual void AttachToWindow(wxFrame* attachedFrame) wxOVERRIDE;
    virtual void DetachFromWindow() wxOVERRIDE;

protected:
    virtual void UpdateWindowContents(wxDC& winDc, wxMemoryDC& dc) wxOVERRIDE;

    /**
     * \brief Initializes class instance.
     */
    void Init();

private:
    bool m_supportsAlpha;

    void OnPaint(wxPaintEvent& evnt);
    static void OnGtkScreenChanged(GtkWidget *widget, GdkScreen *old_screen, gpointer userdata);
};

#endif
