#pragma once

#ifdef _WIN32

#include "drop_shadow_frame_msw.h"
typedef wxDropShadowFrameMSW wxDropShadowFrame;

#else

#error "This implementation is currently only available for MSW"

#endif
