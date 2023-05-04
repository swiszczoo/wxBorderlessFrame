/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/window_gripper_msw.h
// Purpose:     wxWindowGripper implementation for MSW
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2022-12-29
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
 * \file
 * \brief wxWindowGripper implementation for MSW.
 */

#ifndef _WXBF_WINDOW_GRIPPER_MSW_H_
#define _WXBF_WINDOW_GRIPPER_MSW_H_

#include "borderless_frame_common.h"
#include "window_gripper.h"

/**
 * \brief A class that encapsulates wxWindowGripper implementation for wxMSW.
 */
class BFDLLEXPORT wxWindowGripperMSW : public wxWindowGripper
{
public:
    /**
     * \brief Starts to move the given window, as when the user clicks
     *        and drags on its titlebar.
     *
     * \param window the window to start moving
     * \return true, if the command was successfully processed
     */
    virtual bool StartDragMove(wxWindow* window) wxOVERRIDE;

    /**
     * \brief Starts to resize the given window, as when the user clicks
     *        and drags on its frame.
     *
     * \param window the window to start resizing
     * \param which the direction of resizing, may be a combination of
     *              multiple values, e.g. (`wxTOP | wxLEFT`)
     * \return `true`, if the command was successfully processed
     */
    virtual bool StartDragResize(wxWindow* window, wxDirection which) wxOVERRIDE;

private:
    /**
     * \brief Retrieves the appropriate `LPARAM` value for resize `WM_SYSCOMMAND`
     *        message.
     * 
     * \param dir which edge is being dragged
     * \return `LPARAM` value for `WM_SYSCOMMAND` message
     */
    int GetResizeMsgId(wxDirection dir) const;
};

#endif
