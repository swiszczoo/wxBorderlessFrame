/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/window_gripper.h
// Purpose:     wxWindowGripper abstract class
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2022-12-29
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
 * \file
 * \brief wxWindowGripper abstract class.
 */

#ifndef _WXBF_WINDOW_GRIPPER_H_
#define _WXBF_WINDOW_GRIPPER_H_

#include "borderless_frame_common.h"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

/**
 * \brief This abstract class defines a platform-independent interface for 
 *        performing move and resize operations on the window.
 * 
 * It contains a static factory method to enable creating its platform-specific
 * instances in a simple way.
 */
class BFDLLEXPORT wxWindowGripper
{
public:
    /**
     * \brief Virtual destructor.
     */
    virtual ~wxWindowGripper() {};

    /**
     * \brief Starts to move the given window, as when the user clicks 
     *        and drags on its titlebar.
     * 
     * \param window the window to start moving
     * \return `true`, if the command was successfully processed
     */
    virtual bool StartDragMove(wxWindow* window) = 0;

    /**
     * \brief Starts to resize the given window, as when the user clicks 
     *        and drags on its frame.
     *
     * \param window the window to start resizing
     * \param which the direction of resizing, may be a combination of
     *              multiple values, e.g. (`wxTOP | wxLEFT`)
     * \return `true`, if the command was successfully processed
     */
    virtual bool StartDragResize(wxWindow* window, wxDirection which) = 0;

    /**
     * \brief Creates a new platform-specific instance of wxWindowGripper.
     * 
     * \return a pointer to the newly created wxWindowGripper instance. The
     *         caller takes ownership of the pointer and **must** explicitly free 
     *         it when it is no longer required.
     */
    static wxWindowGripper* Create();
};

#endif
