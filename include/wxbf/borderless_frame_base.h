/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/borderless_frame_base.h
// Purpose:     wxBorderlessFrameBase class
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2023-03-29
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
 * \file
 * \brief wxBorderlessFrameBase class.
 */

#ifndef _WXBF_BORDERLESS_FRAME_BASE_H_
#define _WXBF_BORDERLESS_FRAME_BASE_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "borderless_frame_common.h"

wxDECLARE_EXPORTED_EVENT(BFDLLEXPORT, wxEVT_UPDATE_SYSTEM_BUTTONS, wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(BFDLLEXPORT, wxEVT_NC_LEFT_DOWN, wxMouseEvent);
wxDECLARE_EXPORTED_EVENT(BFDLLEXPORT, wxEVT_NC_LEFT_UP, wxMouseEvent);
wxDECLARE_EXPORTED_EVENT(BFDLLEXPORT, wxEVT_NC_RIGHT_DOWN, wxMouseEvent);
wxDECLARE_EXPORTED_EVENT(BFDLLEXPORT, wxEVT_NC_RIGHT_UP, wxMouseEvent);
wxDECLARE_EXPORTED_EVENT(BFDLLEXPORT, wxEVT_NC_MOTION, wxMouseEvent);
wxDECLARE_EXPORTED_EVENT(BFDLLEXPORT, wxEVT_NC_LEAVE, wxMouseEvent);

/**
 * \brief An enum containing system commands that can be executed on the window.
 */
enum wxSystemCommand
{
    wxSC_MINIMIZE, //!< Minimize window
    wxSC_MAXIMIZE, //!< Maximize window (if it is not maximized)
    wxSC_RESTORE, //!< Restore window (if it is maximized)
    wxSC_CLOSE //!< Close window
};

/**
 * \brief An enum containing various window parts that mouse may be hovering over.
 */
enum wxWindowPart {
    wxWP_CLIENT_AREA, //!< The inside part of the window, where all child widgets are placed
    wxWP_TITLEBAR, //!< The window can be repositioned by pressing LMB over titlebar
    wxWP_MINIMIZE_BUTTON, //!< Minimize button of the window
    wxWP_MAXIMIZE_BUTTON, //!< Maximize or restore button of the window
    wxWP_CLOSE_BUTTON //!< Close button of the window
};

/**
 * \brief An abstract class that all platform-specific implementations of
 *        borderless frame derive from.
 * 
 * It currently provides only one method - for running a particular system
 * command.
 * 
 * \beginEventEmissionTable{wxCommandEvent,wxMouseEvent}
 * \event{EVT_UPDATE_SYSTEM_BUTTONS(id, func)}
 *      Process a wxEVT_UPDATE_SYSTEM_BUTTONS event, when system buttons need to be redrawn.
 * \event{EVT_NC_LEFT_DOWN(id, func)}
 *      Process a wxEVT_NC_LEFT_DOWN, when left mouse button is pressed in non-client area (see wxEVT_LEFT_DOWN)
 * \event{EVT_NC_LEFT_UP(id, func)}
 *      Process a wxEVT_NC_LEFT_UP, when left mouse button is released in non-client area (see wxEVT_LEFT_UP)
 * \event{EVT_NC_RIGHT_DOWN(id, func)}
 *      Process a wxEVT_NC_RIGHT_DOWN, when right mouse button is pressed in non-client area (see wxEVT_RIGHT_DOWN)
 * \event{EVT_NC_RIGHT_UP(id, func)}
 *      Process a wxEVT_NC_RIGHT_UP, when right mouse button is released in non-client area (see wxEVT_RIGHT_UP)
 * \event{EVT_NC_MOTION(id, func)}
 *      Process a wxEVT_NC_MOTION, when mouse is moved around in non-client area (see wxEVT_MOTION)
 * \event{EVT_NC_LEAVE(id, func)}
 *      Process a wxEVT_NC_LEAVE, when mouse is leaving the non-client area (see wxEVT_LEAVE)
 * \endEventTable
 */
class BFDLLEXPORT wxBorderlessFrameBase : public wxFrame
{
public:
    using wxFrame::wxFrame;

    /**
     * \brief Virtual destructor.
     */
    virtual ~wxBorderlessFrameBase() {}

    /**
     * \brief Executes the provided system command.
     * 
     * \param command the command to execute
     */
    virtual void ExecSystemCommand(wxSystemCommand command) = 0;
};

#endif
