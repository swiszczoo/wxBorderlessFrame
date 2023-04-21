/////////////////////////////////////////////////////////////////////////////
// Name:        window_gripper.cpp
// Purpose:     wxWindowGripper factory
// Author:      �ukasz �wiszcz
// Modified by:
// Created:     2022-04-17
// Copyright:   (c) �ukasz �wiszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include <wxbf/window_gripper.h>
#ifdef _WIN32
#include <wxbf/window_gripper_msw.h>
#endif

wxWindowGripper* wxWindowGripper::Create()
{
#ifdef _WIN32
    return new wxWindowGripperMSW();
#endif

    wxLogError("Could not find implementation of wxWindowGripper for this platform");
    return nullptr;
}
