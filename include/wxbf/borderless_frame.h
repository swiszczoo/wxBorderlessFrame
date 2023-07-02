/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/borderless_frame.h
// Purpose:     Includes and typedefs the correct wxBorderlessframe implementation
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2022-12-26
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
 * \file
 * \brief Typedefs correct implementation of wxBorderlessFrame for the current
 *        platform.
 */

#ifndef _WXBF_BORDERLESS_FRAME_H_
#define _WXBF_BORDERLESS_FRAME_H_

#if defined(_WIN32)

#include "borderless_frame_msw.h"
typedef wxBorderlessFrameMSW wxBorderlessFrame;

#elif defined(__WXGTK__)

#include "borderless_frame_gtk.h"
typedef wxBorderlessFrameGTK wxBorderlessFrame;

#else

#error "This implementation is currently only available for MSW and GTK"

#endif

#endif
