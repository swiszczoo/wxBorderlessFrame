/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/borderless_frame_common.h
// Purpose:     Macros for exporting DLL API for MSVC
// Author:      £ukasz Œwiszcz
// Modified by:
// Created:     2022-12-26
// Copyright:   (c) £ukasz Œwiszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXBF_BORDERLESS_FRAME_COMMON_H_
#define _WXBF_BORDERLESS_FRAME_COMMON_H_

#ifdef _MSC_VER
#ifdef BFRAME_DLL_EXPORT
#define BFDLLEXPORT __declspec(dllexport)
#else
#define BFDLLEXPORT __declspec(dllimport)
#endif
#else
#define BFDLLEXPORT
#endif

#endif
