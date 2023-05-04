/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/system_buttons_base.h
// Purpose:     wxSystemButtonsBase class
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2023-03-27
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
 * \file
 * \brief wxSystemButtonsBase class.
 */

#ifndef _WXBF_SYSTEM_BUTTONS_BASE_H_
#define _WXBF_SYSTEM_BUTTONS_BASE_H_

#include "borderless_frame_base.h"
#include "borderless_frame_common.h"

/**
 * \brief An enum containing all supported system menu buttons.
 */
enum wxSystemButton {
    wxSB_MINIMIZE, //!< Minimize button
    wxSB_MAXIMIZE, //!< Maximize button (if window is not maximized)
    wxSB_RESTORE, //!< Restore button (if window is maximized)
    wxSB_CLOSE, //!< Close button
};

/**
 * \brief An enum containing colour kinds.
 */
enum wxSystemButtonColourKind {
    wxSB_COLOUR_BACKGROUND, //!< Background colour
    wxSB_COLOUR_FOREGROUND, //!< Foreground colour (symbol colour)
};

/**
 * \brief An enum containing possible system menu button states.
 */
enum wxSystemButtonState {
    wxSB_STATE_INACTIVE, //!< If the window is currently not focused
    wxSB_STATE_DISABLED, //!< If the button is not interactive
    wxSB_STATE_NORMAL, //!< Normal state
    wxSB_STATE_HOVER, //!< If the button is hovered over by mouse
    wxSB_STATE_PRESSED, //!< If the button is currently hovered over and pressed
};

/**
 * \brief An abstract class for implementing system menu button renderers.
 * 
 * This class is responsible for painting system buttons on a borderless 
 * frame. They must be painted directly because decorations must be a part of
 * the frame, not its child widgets. The class has methods responsible for 
 * customizing the appearance of the buttons abstractly, as well as some pure
 * virtual methods that perform drawing routines.
 * 
 * This base class encapsulates all of the system button logic in
 * a platform-independent way. If you want to do more customization than simply
 * changing colours, the only thing you'd have to do is implementing two pure
 * virtual methods.
 */
class BFDLLEXPORT wxSystemButtonsBase {
public:
    /**
     * \brief Creates a wxSystemButtonsBase instance bound to the provided frame.
     * 
     * \param frame the frame to draw buttons on
     */
    explicit wxSystemButtonsBase(wxBorderlessFrameBase* frame);
    /** \brief Virtual destructor. */
    virtual ~wxSystemButtonsBase();

    /**
     * \brief Returns `true` if the buttons are aligned to the right.
     * 
     * \return `true`, if the buttons are aligned to the right (like on MSW), 
     *         `false`, if they're not (like on OSX)
     */
    virtual bool AreButtonsRightAligned() const = 0;
    /**
     * \brief Gets the best button size of this implementation.
     * 
     * \return preferred button size of this implementation
     */
    virtual wxSize GetPreferredButtonSize() const = 0;

    /**
     * \brief Modifies a single colour to customize button appearance.
     * 
     * \param which which button to modify
     * \param state which state of the button to modify
     * \param kind what kind of color to set
     * \param colour new colour to set
     */
    void SetColourTableEntry(wxSystemButton which, wxSystemButtonState state,
        wxSystemButtonColourKind kind, const wxColour& colour)
    {
        m_colourTable[which][state][kind] = colour;
    }

    /**
     * \brief Gets a single chosen colour.
     * 
     * \param which which button to get colour of
     * \param state which state of the button to get colour of
     * \param kind what kind of color to get
     * \return the chosen colour
     */
    wxColour& GetColourTableEntry(wxSystemButton which, wxSystemButtonState state,
        wxSystemButtonColourKind kind)
    {
        return m_colourTable[which][state][kind];
    }

    /**
     * \brief Sets the actual button size.
     * 
     * If any of the dimensions are negative, they will be ignored and
     * preferred dimensions will be used instead.
     * 
     * \param size new size of a single button
     */
    void SetButtonSize(wxSize size);
    /**
     * \brief Gets the actual button size.
     *
     * If any of the dimensions are negative, they will be ignored and
     * preferred dimensions will be used instead.
     * 
     * \return current size of a single button, can contain negative dimensions
     * 
     */
    wxSize GetButtonSize() const;

    /**
     * \brief Calculates the bounding rectangle of all system buttons 
     *        and returns its size.
     * 
     * \return size of the bouding rectangle
     */
    wxSize GetTotalSize() const
    {
        return m_totalRect.GetSize();
    }

    /**
     * \brief Calculates the bounding rectangle of all system buttons.
     *
     * \return the bouding rectangle
     */
    wxRect GetBoundingRect() const
    {
        return m_totalRect;
    }

    /**
     * \brief Retrieves over what part of the window is the mouse currently
     *        hovering over.
     * 
     * This method is provided here to simplify integration of the borderless
     * frame. It can be directly called from wxBorderlessFrame::GetWindowPart().
     * 
     * \param mousePos current mouse position, 
     *                 relative to the screen top-left corner
     * \return window part the mouse is currently hovering over
     */
    wxWindowPart GetWindowPart(wxPoint mousePos) const;

    /**
     * \brief Updates internal state of the class and requests redraw.
     * 
     * This method **must** be called after changing frame style in some
     * unpredictable way (simply calling `SetWindowStyle()` should work out-of
     * the box) or changing colors by calling \ref SetColourTableEntry(). 
     * The class will update which buttons are enabled or disabled and their 
     * positions respectively.
     */
    virtual void UpdateState();

protected:
    /**
     * \brief Calculates and retrieves the actual size of a particular button.
     * 
     * \param[in] which which button to calculate size
     * \param[out] margin calculated spacing between this and next button, in DIP
     * \return calculated size of the button, in DIP
     */
    virtual wxSize MeasureButton(wxSystemButton which, wxCoord& margin) const = 0;
    /**
     * \brief Draws a specified button having a provided state.
     * 
     * \param dc the device context
     * \param which which button to draw
     * \param state what state the button currently has
     * \param rect bounding rect to draw the button to
     */
    virtual void DrawButton(wxDC& dc, wxSystemButton which,
        wxSystemButtonState state, const wxRect& rect) = 0;

    virtual void OnDestroy(wxWindowDestroyEvent& evnt); //!< `wxEVT_DESTROY` handler
    virtual void OnUpdateSystemButtons(wxCommandEvent& evnt); //!< `wxEVT_UPDATE_SYSTEM_BUTTONS` handler
    virtual void OnSize(wxSizeEvent& evnt); //!< `wxEVT_SIZE` handler
    virtual void OnMouseCaptureLost(wxMouseCaptureLostEvent& evnt); //!< `wxEVT_MOUSE_CAPTURE_LOST` handler
    virtual void OnMouse(wxMouseEvent& evnt); //!< multiple mouse events handler
    virtual void OnActivate(wxActivateEvent& evnt); //!< `wxEVT_ACTIVATE` handler
    virtual void OnMaximize(wxMaximizeEvent& evnt); //!< `wxEVT_MAXIMIZE` handler
    virtual void OnPaint(wxPaintEvent& evnt); //!< `wxEVT_PAINT` handler

private:
    wxBorderlessFrameBase* m_owner;
    bool m_ownerActive;

    wxColour m_colourTable[4][5][2];
    wxSize m_buttonSize;
    wxRect m_totalRect;
    wxRect m_buttonRects[4];
    wxSystemButtonState m_lastButtonState[4];
    wxSystemButtonState m_buttonState[4];
    bool m_buttonVisible[4];
    int m_pressedButton;

    void InitColourTable();
    void UpdateSystemButtons();
    void UpdateDisabledActiveState();
    void Layout();
    void Redraw(bool withLayout = false);

    static void AppendToRect(wxRect& rect, wxRect toAppend) 
    {
        if (rect.IsEmpty()) {
            rect = toAppend;
        }
        else {
            rect += toAppend;
        }
    }
};

#endif
