#pragma once

#ifdef _WIN32

#include "borderless_frame_msw.h"
typedef wxBorderlessFrameMSW wxBorderlessFrame;

#else

#error "This implementation is currently only available for MSW"

#endif
