/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/borderless_frame_msw.h
// Purpose:     wxBorderlessFrame implementation for MSW
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2022-12-26
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
 * \file
 * \brief wxBorderlessFrame implementation for MSW.
 */

#ifndef _WXBF_BORDERLESS_FRAME_MSW_H_
#define _WXBF_BORDERLESS_FRAME_MSW_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "borderless_frame_base.h"
#include "borderless_frame_common.h"
#include "drop_shadow_frame_msw.h"
#include "resizer_drop_shadow_frame.h"

/**
 * \brief A platform specific implementation of wxBorderlessFrameBase for MSW.
 * 
 * You would probably want to put this as the base class for your frames,
 * replacing `wxFrame` occurrences.
 * 
 * To actually use this class, you probably need to override the default trivial
 * implementation of GetWindowPart().
 */
class BFDLLEXPORT wxBorderlessFrameMSW : public wxBorderlessFrameBase
{
public:
    /**
     * \brief Default constructor.
     */
    wxBorderlessFrameMSW() { Init(); }

    /**
     * \brief Constructor, creating the borderless window.
     * 
     * \param parent The window parent. This may be, and often is, `NULL`. 
     *               If it is non-`NULL`, the frame will be minimized when its 
     *               parent is minimized and restored when it is restored 
     *               (although it will still be possible to minimize and restore 
     *               just this frame itself)
     * \param id The window identifier. It may take a value of -1 
     *           to indicate a default value.
     * \param title The caption to be displayed on the frame's title bar.
     * \param pos The window position. The value wxDefaultPosition indicates 
     *            a default position, chosen by either the windowing system or 
     *            wxWidgets, depending on platform.
     * \param size The window size. The value wxDefaultSize indicates a default 
     *             size, chosen by either the windowing system or wxWidgets, 
     *             depending on platform.
     * \param style The window style. See wxFrame class description.
     * \param name The name of the window. This parameter is used to associate 
     *             a name with the item, allowing the application user to set 
     *             Motif resource values for individual windows.
     */
    wxBorderlessFrameMSW(wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr));

    /**
     * \brief Virtual destructor.
     */
    virtual ~wxBorderlessFrameMSW();

    /**
     * \brief Creates the borderless window, only use after calling the default
     *        constructor.
     * 
     * \see wxBorderlessFrameMSW() parametrized constructor
     * \return `true` if creation was successful, `false` otherwise
     */
    bool Create(wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr));

    /**
     * \brief Sets the colour of the border around the window.
     * 
     * \param borderColour New border colour.
     */
    void SetBorderColour(wxColour borderColour)
    {
        m_borderColour = borderColour;
        UpdateNcArea();
    }

    /**
     * \brief Retrieves current colour of the border around the window.
     * 
     * \return Current colour of the border.
     */
    wxColour GetBorderColour() const { return m_borderColour; }

    /**
     * \brief Sets the thickness of the border around the window.
     * 
     * \param thicknessPx New border thickness, in pixels.
     */
    void SetBorderThickness(int thicknessPx)
    {
        wxASSERT_MSG(thicknessPx >= 0, "thicknessPx must be non-negative");

        m_borderThickness = thicknessPx;
        UpdateNcArea();
    }

    /**
     * \brief Retrieves the thickness of the border around the window.
     * 
     * \return Current border thickness, in pixels.
     */
    int GetBorderThickness() const
    {
        return m_borderThickness;
    }

    /**
     * \brief Retrieves current window shadow alpha.
     * 
     * \return Current alpha, in range [0-255].
     */
    wxByte GetShadowAlpha() const
    {
        return m_shadow[0]->GetShadowAlpha();
    }

    /**
     * \brief Sets window shadow alpha.
     *
     * Alpha is a single value in range [0-255], where 255 means the shadow is
     * completely opaque (its darkest point) and 0 means it's completely
     * transparent.
     * 
     * \param newAlpha New shadow alpha, in range [0-255].
     */
    void SetShadowAlpha(wxByte newAlpha)
    {
        for (size_t i = 0; i < 4; ++i) m_shadow[i]->SetShadowAlpha(newAlpha);
    }

    /**
     * \brief Retrieves current window shadow size.
     *
     * \return current shadow size, in pixels
     */
    int GetShadowSize() const
    {
        return m_shadow[0]->GetShadowSize();
    }

    /**
     * \brief Sets size of the shadow around the window and its draggable area.
     *
     * Because a user can resize the main window by clicking and dragging its
     * shadow edges, changing shadow size will effectively resize the draggable
     * area a user can use to change frame size.
     *
     * \param shadowSize New shadow size, in pixels
     */
    void SetShadowSize(int shadowSize)
    {
        for (size_t i = 0; i < 4; ++i) m_shadow[i]->SetShadowSize(shadowSize);
    }

    /**
     * \brief Retrieves current window shadow offset.
     *
     * \return current shadow offset, in pixels
     */
    wxPoint GetShadowOffset() const
    {
        return m_shadow[0]->GetShadowOffset();
    }

    /**
     * \brief Sets window shadow offset.
     *
     * *Example:* setting offset to (0, 5) would move the shadow 5px lower than
     * where it would normally be. Effectively, the bottom edge of the frame would
     * be significantly darker that the top edge.
     *
     * \param newOffset new shadow offset, in pixels
     */
    void SetShadowOffset(wxPoint newOffset)
    {
        for (size_t i = 0; i < 4; ++i) m_shadow[i]->SetShadowOffset(newOffset);
    }

    /**
     * \brief Retrieves if the shadow is hidden while the frame is inactive.
     *
     * \return `true` if shadow will be hidden, `false` if shadow is visible
     *         all time
     */
    bool IsShadowDisabledOnInactiveWindow() const
    {
        return m_shadow[0]->IsDisableShadowOnInactiveWindow();
    }

    /**
     * \brief Sets if the shadow is hidden while the frame is inactive.
     *
     * The default setting is `true`.
     *
     * \param disable `true`, if the shadow will be hidden while owner frame
     *                is inactive, `false` if the shadow should be visible all
     *                time.
     */
    void SetShadowDisabledOnInactiveWindow(bool disable)
    {
        for (size_t i = 0; i < 4; ++i) m_shadow[i]->SetDisableShadowOnInactiveWindow(disable);
    }

    /**
     * \brief Opens a system menu.
     * 
     * This menu contains numerous operations which can be performed on the
     * window, such as *Move*, *Resize*, *Minimize*, *Maximize*, *Close* and so
     * on. On MSW it is usually available under the `Alt+Space` keyboard shortcut.
     * 
     * The system menu usually pops up when the user right-clicks window titlebar.
     */
    void PopupSystemMenu();

    /**
     * \brief MSW WndProc implementation.
     */
    virtual WXLRESULT MSWWindowProc(WXUINT message,
        WXWPARAM wParam,
        WXLPARAM lParam) wxOVERRIDE;

    /**
     * \brief Executes a system command on the frame.
     * 
     * You would usually call this function in response to user clicking one of
     * the system menu buttons (*Minimize*, *Maximize* or *Close*)
     * 
     * \param command The command to execute.
     */
    virtual void ExecSystemCommand(wxSystemCommand command) wxOVERRIDE;

    /**
     * Updates window style and emits an event, if system menu should be redrawn.
     * 
     * You probably would not want to call this function. Better call
     * `SetWindowStyle()` directly.
     * 
     * \brief style New window style.
     */
    void SetWindowStyleFlag(long style) wxOVERRIDE;

protected:
    /**
     * \brief Returns, which window parent is the mouse currently hovering over.
     * 
     * It is required to provide that information to the OS, because that enables
     * it to provide implementations for several system-wide gestures, like 
     * Aero Shake or Windows 11 snap layouts.
     * 
     * \param mousePosition Current mouse position, relative to the screen
     *                      top-left corner.
     * \return Which part the mouse is hovering over.
     */
    virtual wxWindowPart GetWindowPart(wxPoint mousePosition) const;

private:
    wxColour m_borderColour;
    int m_borderThickness;
    bool m_maximizedTheme;
    bool m_leaveTrackArmed;

    wxResizerDropShadowFrame<wxDropShadowFrameMSW>* m_shadow[4];

    void Init();
    void UpdateNcArea();
    void UpdateSystemMenu(::HMENU sysMenu);
    void AdjustMaximizedClientRect(::HWND window, RECT& rect);
    void UpdateTheme();

    void OnMaximize(wxMaximizeEvent& evnt);
    void OnIconize(wxIconizeEvent& evnt);
    void OnSize(wxSizeEvent& evnt);

    WXLRESULT HandleNcMessage(WXUINT message, WXWPARAM wParam, WXLPARAM lParam); 
    void TrackNcLeave();
};

#endif
