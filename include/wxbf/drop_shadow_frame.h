/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/drop_shadow_frame.h
// Purpose:     Includes and typedefs the correct wxDropShadowFrame implementation
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2022-12-27
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
 * \file
 * \brief Typedefs correct implementation of wxDropShadowFrame for the current
 *        platform.
 */

#ifndef _WXBF_DROP_SHADOW_FRAME_H_
#define _WXBF_DROP_SHADOW_FRAME_H_

#ifdef _WIN32

#include "drop_shadow_frame_msw.h"
typedef wxDropShadowFrameMSW wxDropShadowFrame;

#else

#error "This implementation is currently only available for MSW"

#endif

#endif
