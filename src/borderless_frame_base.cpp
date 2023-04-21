/////////////////////////////////////////////////////////////////////////////
// Name:        borderless_frame_base.cpp
// Purpose:     wxBorderlessFrame event definitions
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2023-03-29
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include <wxbf/borderless_frame_base.h>

wxDEFINE_EVENT(wxEVT_UPDATE_SYSTEM_BUTTONS, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_NC_LEFT_DOWN, wxMouseEvent);
wxDEFINE_EVENT(wxEVT_NC_LEFT_UP, wxMouseEvent);
wxDEFINE_EVENT(wxEVT_NC_RIGHT_DOWN, wxMouseEvent);
wxDEFINE_EVENT(wxEVT_NC_RIGHT_UP, wxMouseEvent);
wxDEFINE_EVENT(wxEVT_NC_MOTION, wxMouseEvent);
wxDEFINE_EVENT(wxEVT_NC_LEAVE, wxMouseEvent);
