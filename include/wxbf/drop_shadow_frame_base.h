/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/drop_shadow_frame_base.h
// Purpose:     wxDropShadowFrameBase class
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2022-12-27
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
 * \file
 * \brief wxDropShadowFrameBase class.
 */

#ifndef _WXBF_DROP_SHADOW_FRAME_BASE_H_
#define _WXBF_DROP_SHADOW_FRAME_BASE_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/popupwin.h>

#include "borderless_frame_common.h"

#include <chrono>

/**
 * \brief An enum containing sides of the window.
 */
enum wxDropShadowWindowPart
{
    wxSHADOW_TOP,
    wxSHADOW_BOTTOM,
    wxSHADOW_LEFT,
    wxSHADOW_RIGHT
};

/**
 * \brief An abstract class that contains platform-independent logic for drawing
 *        shadow around the frame.
 * 
 * The shadow is actually composed of 4 separate `wxFrame`s which handle events
 * such as dragging window edge with mouse to resize it.
 * 
 * Platform specific implementations must implement a pure virtual method
 * UpdateWindowContents() which sets window contents to a transparent bitmap.
 */
class BFDLLEXPORT wxDropShadowFrameBase : public wxFrame
{
public:
    /**
     * \brief Default constructor.
     */
    wxDropShadowFrameBase() { Init(); }

    /**
     * \brief Consturctor, creating a wxDropShadowFrameBase.
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
    wxDropShadowFrameBase(wxWindow* parent,
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
     * \brief Creates a wxDropShadowFrameBase.
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
     * \brief Attaches this shadow frame to the provided frame.
     * 
     * Attached shadow frame responds to all important events sent by its new
     * owner. It also maintains its position and size.
     * 
     * \param attachedFrame Window to attach shadow.
     */
    virtual void AttachToWindow(wxFrame* attachedFrame);

    /**
     * \brief Detaches this shadow from its current owner.
     * 
     * This function unregisters all event handlers so both windows can be safely
     * destroyed.
     */
    virtual void DetachFromWindow();

    /**
     * \brief Retrieves a pointer to the current owner.
     * 
     * \return Pointer to the frame that owns this shadow instance.
     */
    wxFrame* GetAttachedWindow() const { return m_attachedFrame; }

    /**
     * \brief Sets size of the shadow and draggable area.
     * 
     * Because a user can resize the main window by clicking and dragging its 
     * shadow edges, changing shadow size will effectively resize the draggable
     * area a user can use to change frame size.
     * 
     * \param size New shadow size, in pixels
     * \return `true`, if size was successfully changed, `false` otherwise
     */
    bool SetShadowSize(int size);

    /**
     * \brief Retrieves current shadow size.
     * 
     * \return current shadow size, in pixels
     */
    int GetShadowSize() const { return m_shadowSize; }

    /**
     * \brief Sets shadow offset.
     * 
     * *Example:* setting offset to (0, 5) would move the shadow 5px lower than
     * where it would normally be. Effectively, the bottom edge of the frame would
     * be significantly darker that the top edge.
     * 
     * \param point new shadow offset, in pixels
     */
    void SetShadowOffset(wxPoint point);

    /**
     * \brief Sets shadow offset.
     *
     * *Example:* setting offset to (0, 5) would move the shadow 5px lower than
     * where it would normally be. Effectively, the bottom edge of the frame would
     * be significantly darker that the top edge.
     *
     * \param x x coordinate of the new shadow offset, in pixels
     * \param y y coordinate of the new shadow offset, in pixels
     */
    void SetShadowOffset(int x, int y) { SetShadowOffset(wxPoint(x, y)); }

    /**
     * \brief Retrieves current shadow offset.
     * 
     * \return current shadow offset, in pixels
     */
    wxPoint GetShadowOffset() const { return m_shadowOffset; }

    /**
     * \brief Sets shadow alpha.
     * 
     * Alpha is a single value in range 0-255, where 255 means the shadow is 
     * completely opaque (its darkest point) and 0 means it's completely
     * transparent.
     * 
     * \param alpha new value of alpha
     */
    void SetShadowAlpha(wxByte alpha);

    /**
     * \brief Retrieves shadow alpha.
     * 
     * \return current value of alpha
     */
    wxByte GetShadowAlpha() const { return m_shadowAlpha; }

    /**
     * \brief Sets if the shadow is hidden while the owner frame is inactive.
     * 
     * The default setting is `true`.
     * 
     * \param disable `true`, if the shadow will be hidden while owner frame 
     *                is inactive, `false` if the shadow should be visible all
     *                time.
     */
    void SetDisableShadowOnInactiveWindow(bool disable);

    /**
     * \brief Retrieves if the shadow is hidden while the owner frame is inactive.
     * 
     * \return `true` if shadow will be hidden, `false` if shadow is visible 
     *         all time
     */
    bool IsDisableShadowOnInactiveWindow() const { return m_disableOnInactive; }

    /**
     * \brief Retrieves which window edge this shadow frame corresponds to.
     * 
     * \return Window part.
     */
    wxDropShadowWindowPart GetPart() const { return m_part; }

    /**
     * \brief Starts a fade in animation, as when window is restored 
     *        after iconizing.
     */
    virtual void StartFadeIn();

protected:
    /**
     * \brief Sets window contents to the provided transparent bitmap.
     * 
     * \param winDc target dc
     * \param dc source dc, containing transparent bitmap
     */
    virtual void UpdateWindowContents(wxDC& winDc, wxMemoryDC& dc) = 0;

    /**
     * \brief Calculates new size and position for this shadow part.
     * 
     * This function takes shadow part into account, as well as owner rect.
     * 
     * \param[out] out calculated shadow part rect
     * \return `true`, if the calculation was successful
     */
    virtual bool GetPlacementRect(wxRect& out);

    /**
     * \brief Handler for the `wxEVT_MOVE` event of the owner frame.
     */
    virtual void OnAttachedMove(wxMoveEvent& evnt);

    /**
     * \brief Handler for the `wxEVT_SIZE` and `wxEVT_SIZING` events of the owner frame.
     */
    virtual void OnAttachedSize(wxSizeEvent& evnt);

    /**
     * \brief Handler for the `wxEVT_ICONIZE` event of the owner frame.
     */
    virtual void OnAttachedMinimize(wxIconizeEvent& evnt);

    /**
     * \brief Handler for the `wxEVT_MAXIMIZE` event of the owner frame.
     */
    virtual void OnAttachedMaximize(wxMaximizeEvent& evnt);

    /**
     * \brief Handler for the `wxEVT_ACTIVATE` event of the owner frame.
     */
    virtual void OnAttachedActivate(wxActivateEvent& evnt);

    /**
     * \brief Handler for the `wxEVT_DESTROY` event of the owner frame.
     */
    virtual void OnAttachedDestroy(wxWindowDestroyEvent& evnt);

    /**
     * \brief Handler for the `wxEVT_ACTIVATE` event of this frame.
     */
    virtual void OnActivate(wxActivateEvent& evnt);

    /**
     * \brief Handler for the `wxEVT_DPI_CHANGED` event of this frame.
     */
    virtual void OnDpiChanged(wxDPIChangedEvent& evnt);

    /**
     * \brief Redraws shadow from scratch.
     */
    virtual void RedrawShadow();

    /**
     * \brief Initializes class instance.
     */
    void Init();

    /**
     * \brief Retrieves current shadow bitmap
     * 
     * \return Current shadow bitmap.
     */
    wxBitmap& GetShadowBitmap() 
    {
        return m_bmp;
    }

private:
    static const double FADE_IN_TIME_MS;
    static const double FADE_IN_DELAY_MS;
    static const int TIMER_TICK_MS;

    wxDropShadowWindowPart m_part;
    wxVector<double> m_gaussianCoeffs;
    wxVector<double> m_gaussianCoeffPrefixSums;
    int m_gaussianCoeffsDim;

    wxByte m_shadowAlpha;
    wxByte m_currentShadowAlpha;
    bool m_disableOnInactive;
    bool m_attachedFrameActive;
    wxBitmap m_bmp;
    wxFrame* m_attachedFrame;
    int m_shadowSize;
    wxPoint m_shadowOffset;

    std::chrono::steady_clock::time_point m_fadeInTimerStart;
    wxTimer m_fadeInTimer;

    void OnTimerTick(wxTimerEvent& evnt);

    void InitGaussianKernel();
    void InitGaussianKernel(size_t stdDev);
    double CalcGaussianCoeff(double stdDev, double x, double y);
    void RepositionFrame(bool force = false);
    wxByte GetShadowIntensity(int x, int y, wxSize windowSize) const;
    wxRect NormalizeRect(const wxRect& rect) const;
    double GetRangeSum(const wxRect& rect) const;
    wxPoint GetWindowOffset(const wxSize& windowSize) const;
};

#endif
