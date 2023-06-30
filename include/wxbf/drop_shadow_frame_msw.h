/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/drop_shadow_frame_msw.h
// Purpose:     wxDropShadowFrame implementation for MSW
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2022-12-27
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
 * \file
 * \brief wxDropShadowFrameBase implementation for MSW.
 */

#ifndef _WXBF_DROP_SHADOW_FRAME_MSW_H_
#define _WXBF_DROP_SHADOW_FRAME_MSW_H_

#include "borderless_frame_common.h"
#include "drop_shadow_frame_base.h"

/**
 * \brief A platform specific implementation of wxDropShadowFrameBase for MSW.
 */
class BFDLLEXPORT wxDropShadowFrameMSW : public wxDropShadowFrameBase
{
public:
    /**
     * \brief Default constructor.
     */
    wxDropShadowFrameMSW() { Init(); }

    /**
     * \brief Constructor, creating a platform-specific wxDropShadowFrameMSW 
     *        instance.
     *
     * \param parent Pointer to a parent window.
     * \param id Window identifier. If wxID_ANY, will automatically create an identifier.
     * \param part Edge of the window this shadow frame corresponds to.
     * \param pos Window position. wxDefaultPosition indicates that wxWidgets
     *            should generate a default position for the window. If using
     *            the wxWindow class directly, supply an actual position.
     * \param size Window size. wxDefaultSize indicates that wxWidgets should
     *             generate a default size for the window. If no suitable size
     *             can be found, the window will be sized to 20x20 pixels so
     *             that the window is visible but obviously not correctly sized.
     * \param style Window style. For generic window styles, please see wxWindow.
     * \param name Window name.
     */
    wxDropShadowFrameMSW(wxWindow* parent,
        wxWindowID id,
        wxDropShadowWindowPart part,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr))
    {
        Init();
        Create(parent, id, part, pos, size, style, name);
    }

    /**
     * \brief Creates a wxDropShadowFrameMSW instance.
     *
     * Call this method only if constructed with default constructor.
     *
     * \param parent Pointer to a parent window.
     * \param id Window identifier. If wxID_ANY, will automatically create an identifier.
     * \param part Edge of the window this shadow frame corresponds to.
     * \param pos Window position. wxDefaultPosition indicates that wxWidgets
     *            should generate a default position for the window. If using
     *            the wxWindow class directly, supply an actual position.
     * \param size Window size. wxDefaultSize indicates that wxWidgets should
     *             generate a default size for the window. If no suitable size
     *             can be found, the window will be sized to 20x20 pixels so
     *             that the window is visible but obviously not correctly sized.
     * \param style Window style. For generic window styles, please see wxWindow.
     * \param name Window name.
     * \return `true`, if creation was successful, `false` otherwise
     */
    bool Create(wxWindow* parent,
        wxWindowID id,
        wxDropShadowWindowPart part,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr));

    /**
     * \brief Does not work and always returns `false`.
     * 
     * MSW does not allow to both have transparent window content and set its
     * alpha. If you want to set shadow opacity, see 
     * wxDropShadowFrameBase::SetShadowAlpha()
     * 
     * \see wxDropShadowFrameBase::SetShadowAlpha()
     * \param alpha ignored
     * \return always `false`
     */
    virtual bool SetTransparent(wxByte alpha) wxOVERRIDE;

    virtual void AttachToWindow(wxFrame* attachedFrame) wxOVERRIDE;
    virtual void DetachFromWindow() wxOVERRIDE;

protected:
    virtual void UpdateWindowContents(wxDC& winDc, wxMemoryDC& dc) wxOVERRIDE;
};

#endif
