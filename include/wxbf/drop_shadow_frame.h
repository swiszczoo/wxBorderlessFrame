/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/drop_shadow_frame.h
// Purpose:     Includes and typedefs the correct wxDropShadowFrame implementation
// Author:      £ukasz Œwiszcz
// Modified by:
// Created:     2022-12-27
// Copyright:   (c) £ukasz Œwiszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXBF_DROP_SHADOW_FRAME_H_
#define _WXBF_DROP_SHADOW_FRAME_H_

#ifdef _WIN32

#include "drop_shadow_frame_msw.h"
typedef wxDropShadowFrameMSW wxDropShadowFrame;

#else

#error "This implementation is currently only available for MSW"

#endif

#endif
