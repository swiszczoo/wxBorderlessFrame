#pragma once
#include "borderless_frame_common.h"
#include <wx/wx.h>

#include <chrono>

enum wxDropShadowWindowPart
{
    wxSHADOW_TOP,
    wxSHADOW_BOTTOM,
    wxSHADOW_LEFT,
    wxSHADOW_RIGHT
};

class BFDLLEXPORT wxDropShadowFrameBase : public wxFrame
{
public:
    wxDropShadowFrameBase() { Init(); }
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

    bool Create(wxWindow* parent,
        wxWindowID id,
        wxDropShadowWindowPart part,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr));

    virtual void AttachToWindow(wxFrame* attachedFrame);
    virtual void DetachFromWindow();
    wxFrame* GetAttachedWindow() const { return m_attachedFrame; }

    bool SetShadowSize(int size);
    int GetShadowSize() const { return m_shadowSize; }

    void SetShadowOffset(wxPoint point);
    void SetShadowOffset(int x, int y) { SetShadowOffset(wxPoint(x, y)); }
    wxPoint GetShadowOffset() const { return m_shadowOffset; }

    void SetShadowAlpha(wxByte alpha);
    wxByte GetShadowAlpha() const { return m_shadowAlpha; }

    void SetDisableShadowOnInactiveWindow(bool disable);
    bool IsDisableShadowOnInactiveWindow() const { return m_disableOnInactive; }

    wxDropShadowWindowPart GetPart() const { return m_part; }

    virtual void StartFadeIn();

protected:
    virtual void UpdateWindowContents(wxDC& winDc, wxMemoryDC& dc) = 0;
    virtual bool GetPlacementRect(wxRect& out);

    virtual void OnAttachedMove(wxMoveEvent& evnt);
    virtual void OnAttachedSize(wxSizeEvent& evnt);
    virtual void OnAttachedMinimize(wxIconizeEvent& evnt);
    virtual void OnAttachedMaximize(wxMaximizeEvent& evnt);
    virtual void OnAttachedActivate(wxActivateEvent& evnt);
    virtual void OnAttachedDestroy(wxWindowDestroyEvent& evnt);

    virtual void OnActivate(wxActivateEvent& evnt);
    virtual void OnDpiChanged(wxDPIChangedEvent& evnt);

    virtual void RedrawShadow();

    void Init();

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
