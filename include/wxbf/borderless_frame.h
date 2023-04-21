/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/borderless_frame.h
// Purpose:     Includes and typedefs the correct wxBorderlessframe implementation
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2022-12-26
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXBF_BORDERLESS_FRAME_H_
#define _WXBF_BORDERLESS_FRAME_H_

#ifdef _WIN32

#include "borderless_frame_msw.h"
typedef wxBorderlessFrameMSW wxBorderlessFrame;

#else

#error "This implementation is currently only available for MSW"

#endif

#endif
