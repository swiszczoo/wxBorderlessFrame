/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/system_buttons.h
// Purpose:     wxSystemButtonsFactory
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2023-03-27
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
 * \file
 * \brief wxSystemButtonsFactory class.
 */

#ifndef _WXBF_SYSTEM_BUTTONS_H_
#define _WXBF_SYSTEM_BUTTONS_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "borderless_frame_base.h"
#include "borderless_frame_common.h"
#include "system_buttons_base.h"

/**
 * \brief A utility class that provides a single method for constructing 
 *        platform-specific instances of wxSystemButtonsBase
 */
class BFDLLEXPORT wxSystemButtonsFactory final {
public:
    /**
     * \brief Constructs the best wxSystemButtonsBase available for
     *        current platform.
     * 
     * \param frame a borderless frame instance which will be passed to the
     *              constructor of implementation
     * \return a pointer to the newly created wxSystemButtonsBase instance. The
     *         caller takes ownership of the pointer and **must** explicitly free 
     *         it when it is no longer required. 
     */
    static wxSystemButtonsBase* CreateSystemButtons(wxBorderlessFrameBase* frame);
};

#endif
