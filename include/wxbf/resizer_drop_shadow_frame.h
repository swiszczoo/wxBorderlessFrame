/////////////////////////////////////////////////////////////////////////////
// Name:        wxbf/resizer_drop_shadow_frame.h
// Purpose:     Connects wxDropShadowFrame with wxWindowGripper
// Author:      £ukasz Œwiszcz
// Modified by:
// Created:     2022-12-29
// Copyright:   (c) £ukasz Œwiszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXBF_RESIZER_DROP_SHADOW_FRAME_H_
#define _WXBF_RESIZER_DROP_SHADOW_FRAME_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "window_gripper.h"

template <class DropShadowImpl>
class wxResizerDropShadowFrame : public DropShadowImpl
{
public:
    wxResizerDropShadowFrame(wxWindowGripper* gripper) 
    { 
        Init(); 
        m_gripper = gripper; 
    }

    ~wxResizerDropShadowFrame()
    {
        delete m_gripper;
    }

    wxResizerDropShadowFrame(wxWindowGripper* gripper, wxWindow* parent,
        wxWindowID id,
        wxDropShadowWindowPart part,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr))
    {
        Init();
        m_gripper = gripper;
        Create(parent, id, part, pos, size, style, name);
    }

    bool Create(wxWindow* parent,
        wxWindowID id,
        wxDropShadowWindowPart part,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr))
    {
        if (!DropShadowImpl::Create(parent, id, part, pos, size, style, name)) {
            return false;
        }

        DropShadowImpl::Bind(wxEVT_LEFT_DOWN, &wxResizerDropShadowFrame::OnLeftDown, this);
        DropShadowImpl::Bind(wxEVT_MOTION, &wxResizerDropShadowFrame::OnMouseMotion, this);
        return true;
    }

    bool SetCornerRadius(int radius)
    {
        if (radius <= 0) {
            return false;
        }

        m_cornerRadius = radius;
        return true;
    }

    int GetCornerRadius() const { return m_cornerRadius; }

private:
    wxWindowGripper* m_gripper;
    wxStockCursor m_lastCursor;
    int m_cornerRadius;

    void Init()
    {
        DropShadowImpl::Init();
        m_lastCursor = wxCURSOR_NONE;
    }

    wxStockCursor GetCursor(wxPoint mousePos) const
    {
        wxSize size = DropShadowImpl::GetSize();
        int shadowSize = DropShadowImpl::GetShadowSize();

        switch (DropShadowImpl::GetPart()) {
        case wxSHADOW_TOP:
            if (mousePos.x <= m_cornerRadius) {
                return wxCURSOR_SIZENWSE;
            }
            else if (mousePos.x + m_cornerRadius >= size.x) {
                return wxCURSOR_SIZENESW;
            }
            return wxCURSOR_SIZENS;
        case wxSHADOW_BOTTOM:
            if (mousePos.x <= m_cornerRadius) {
                return wxCURSOR_SIZENESW;
            }
            else if (mousePos.x + m_cornerRadius >= size.x) {
                return wxCURSOR_SIZENWSE;
            }
            return wxCURSOR_SIZENS;
        case wxSHADOW_LEFT:
            if (mousePos.y <= m_cornerRadius + shadowSize) {
                return wxCURSOR_SIZENWSE;
            }
            else if (mousePos.y + m_cornerRadius + shadowSize >= size.y) {
                return wxCURSOR_SIZENESW;
            }
            return wxCURSOR_SIZEWE;
        case wxSHADOW_RIGHT:
            if (mousePos.y <= m_cornerRadius + shadowSize) {
                return wxCURSOR_SIZENESW;
            }
            else if (mousePos.y + m_cornerRadius + shadowSize >= size.y) {
                return wxCURSOR_SIZENWSE;
            }
            return wxCURSOR_SIZEWE;
        }

        return wxCURSOR_SIZING;
    }

    wxDirection GetResizeDirection(wxStockCursor cursor) const
    {
        switch (DropShadowImpl::GetPart())
        {
        case wxSHADOW_TOP:
            if (cursor == wxCURSOR_SIZENWSE) return static_cast<wxDirection>(wxTOP | wxLEFT);
            if (cursor == wxCURSOR_SIZENESW) return static_cast<wxDirection>(wxTOP | wxRIGHT);
            return wxTOP;
        case wxSHADOW_BOTTOM:
            if (cursor == wxCURSOR_SIZENESW) return static_cast<wxDirection>(wxBOTTOM | wxLEFT);
            if (cursor == wxCURSOR_SIZENWSE) return static_cast<wxDirection>(wxBOTTOM | wxRIGHT);
            return wxBOTTOM;
        case wxSHADOW_LEFT:
            if (cursor == wxCURSOR_SIZENWSE) return static_cast<wxDirection>(wxTOP | wxLEFT);
            if (cursor == wxCURSOR_SIZENESW) return static_cast<wxDirection>(wxBOTTOM | wxRIGHT);
            return wxLEFT;
        case wxSHADOW_RIGHT:
            if (cursor == wxCURSOR_SIZENESW) return static_cast<wxDirection>(wxTOP | wxRIGHT);
            if (cursor == wxCURSOR_SIZENWSE) return static_cast<wxDirection>(wxBOTTOM | wxRIGHT);
            return wxRIGHT;
        }

        return wxTOP;
    }

    void OnLeftDown(wxMouseEvent& evnt)
    {
        wxWindow* win = DropShadowImpl::GetAttachedWindow();
        if (win) {
            wxStockCursor current = GetCursor(evnt.GetPosition());
            m_gripper->StartDragResize(win, GetResizeDirection(current));
        }
    }

    void OnMouseMotion(wxMouseEvent& evnt)
    {
        wxWindow* attached = DropShadowImpl::GetAttachedWindow();
        wxStockCursor current = wxCURSOR_ARROW;

        if (attached && (attached->GetWindowStyle() & wxRESIZE_BORDER)) {
            current = GetCursor(evnt.GetPosition());
        }

        if (current != m_lastCursor) {
            m_lastCursor = current;
            DropShadowImpl::SetCursor(current);
        }
    }
};

#endif
