#pragma once
#ifdef _MSC_VER
#ifdef BFRAME_DLL_EXPORT
#define BFDLLEXPORT __declspec(dllexport)
#else
#define BFDLLEXPORT __declspec(dllimport)
#endif
#else
#define BFDLLEXPORT
#endif
