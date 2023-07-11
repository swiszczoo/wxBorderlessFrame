/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/borderless_frame_common.h
// Purpose:     Macros for exporting DLL API for MSVC
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2022-12-26
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
 * \file
 * \brief Macros for exporting DLL API for MSVC.
 */

#ifndef _WXBF_BORDERLESS_FRAME_COMMON_H_
#define _WXBF_BORDERLESS_FRAME_COMMON_H_

#if defined(_MSC_VER) && defined(BFRAME_SHARED_LIBRARY)
#ifdef BFRAME_DLL_EXPORT
#define BFDLLEXPORT __declspec(dllexport)
#else
#define BFDLLEXPORT __declspec(dllimport)
#endif
#else
#define BFDLLEXPORT
#endif

#endif
