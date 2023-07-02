/////////////////////////////////////////////////////////////////////////////
// Name:        drop_shadow_frame_base.cpp
// Purpose:     wxDropShadowFrameBase
// Author:      Łukasz Świszcz
// Modified by:
// Created:     2022-12-27
// Copyright:   (c) Łukasz Świszcz
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include <wxbf/drop_shadow_frame_base.h>

#include <wx/log.h>
#include <wx/rawbmp.h>

#define _USE_MATH_DEFINES
#include <math.h>

const double wxDropShadowFrameBase::FADE_IN_TIME_MS = 500;
const double wxDropShadowFrameBase::FADE_IN_DELAY_MS = 200;
const int wxDropShadowFrameBase::TIMER_TICK_MS = 20;

bool wxDropShadowFrameBase::Create(wxWindow* parent,
    wxWindowID id, wxDropShadowWindowPart part, const wxPoint& pos,
    const wxSize& size, long style, const wxString& name)
{
    static const wxString FRAME_NAMES[] = {
        "wxDropShadowFrame_TOP",
        "wxDropShadowFrame_BOTTOM",
        "wxDropShadowFrame_LEFT",
        "wxDropShadowFrame_RIGHT",
    };

    m_part = part;

    wxASSERT_MSG(part >= 0 && part < 4, "part must be a valid value from enum");

    style &= ~wxDEFAULT_FRAME_STYLE;
    style |= wxFRAME_NO_TASKBAR | wxNO_BORDER;

    if (!wxFrame::Create(parent, id, FRAME_NAMES[part], pos, size, style, name)) {
        return false;
    }

    Bind(wxEVT_ACTIVATE, &wxDropShadowFrameBase::OnActivate, this);
    Bind(wxEVT_DPI_CHANGED, &wxDropShadowFrameBase::OnDpiChanged, this);
    m_fadeInTimer.Bind(wxEVT_TIMER, &wxDropShadowFrameBase::OnTimerTick, this);
    return true;
}

void wxDropShadowFrameBase::AttachToWindow(wxFrame* attachedFrame)
{
    m_attachedFrame = attachedFrame;
    m_attachedFrame->Bind(wxEVT_MOVE, &wxDropShadowFrameBase::OnAttachedMove, this);
    m_attachedFrame->Bind(wxEVT_MOVING, &wxDropShadowFrameBase::OnAttachedMove, this);
    m_attachedFrame->Bind(wxEVT_SIZE, &wxDropShadowFrameBase::OnAttachedSize, this);
    m_attachedFrame->Bind(wxEVT_SIZING, &wxDropShadowFrameBase::OnAttachedSize, this);
    m_attachedFrame->Bind(wxEVT_ICONIZE, &wxDropShadowFrameBase::OnAttachedMinimize, this);
    m_attachedFrame->Bind(wxEVT_MAXIMIZE, &wxDropShadowFrameBase::OnAttachedMaximize, this);
    m_attachedFrame->Bind(wxEVT_ACTIVATE, &wxDropShadowFrameBase::OnAttachedActivate, this);
    m_attachedFrame->Bind(wxEVT_DESTROY, &wxDropShadowFrameBase::OnAttachedDestroy, this);
    RepositionFrame();

    m_attachedFrameActive = attachedFrame->IsActive();
}

void wxDropShadowFrameBase::DetachFromWindow()
{
    if (m_attachedFrame) {
        m_attachedFrame->Unbind(wxEVT_MOVE, &wxDropShadowFrameBase::OnAttachedMove, this);
        m_attachedFrame->Unbind(wxEVT_MOVING, &wxDropShadowFrameBase::OnAttachedMove, this);
        m_attachedFrame->Unbind(wxEVT_SIZE, &wxDropShadowFrameBase::OnAttachedSize, this);
        m_attachedFrame->Unbind(wxEVT_SIZING, &wxDropShadowFrameBase::OnAttachedSize, this);
        m_attachedFrame->Unbind(wxEVT_ICONIZE, &wxDropShadowFrameBase::OnAttachedMinimize, this);
        m_attachedFrame->Unbind(wxEVT_MAXIMIZE, &wxDropShadowFrameBase::OnAttachedMaximize, this);
        m_attachedFrame->Unbind(wxEVT_ACTIVATE, &wxDropShadowFrameBase::OnAttachedActivate, this);
        m_attachedFrame->Unbind(wxEVT_DESTROY, &wxDropShadowFrameBase::OnAttachedDestroy, this);
        m_attachedFrame = NULL;
    }
}

bool wxDropShadowFrameBase::SetShadowSize(int size)
{
    if (size <= 0) {
        return false;
    }

    if (size != m_shadowSize) {
        m_shadowSize = size;
        InitGaussianKernel();
        RepositionFrame();
        RedrawShadow();
    }
    return true;
}

void wxDropShadowFrameBase::SetShadowOffset(wxPoint point)
{
    m_shadowOffset = point;
    RedrawShadow();
}

void wxDropShadowFrameBase::SetShadowAlpha(wxByte alpha)
{
    m_shadowAlpha = alpha;
    m_currentShadowAlpha = alpha;
    RedrawShadow();
}

void wxDropShadowFrameBase::SetDisableShadowOnInactiveWindow(bool disable)
{
    m_disableOnInactive = disable;
    RedrawShadow();
}

void wxDropShadowFrameBase::StartFadeIn()
{
    m_currentShadowAlpha = 1;
    m_fadeInTimerStart = std::chrono::steady_clock::now();
    m_fadeInTimer.Start(TIMER_TICK_MS);
    RedrawShadow();
}

bool wxDropShadowFrameBase::GetPlacementRect(wxRect& out)
{
    if (!m_attachedFrame) {
        return false;
    }

    wxRect windowRect = m_attachedFrame->GetRect();
    switch (GetPart()) {
    case wxSHADOW_TOP:
        out.SetPosition(windowRect.GetPosition() - wxPoint(0, m_shadowSize));
        out.SetSize(wxSize(windowRect.GetWidth(), m_shadowSize));
        return true;
    case wxSHADOW_BOTTOM:
        out.SetPosition(windowRect.GetBottomLeft() + wxPoint(0, 1));
        out.SetSize(wxSize(windowRect.GetWidth(), m_shadowSize));
        return true;
    case wxSHADOW_LEFT:
        out.SetPosition(windowRect.GetPosition() - wxPoint(m_shadowSize, m_shadowSize));
        out.SetSize(wxSize(m_shadowSize, windowRect.GetHeight() + 2 * m_shadowSize));
        return true;
    case wxSHADOW_RIGHT:
        out.SetPosition(windowRect.GetPosition() + wxPoint(windowRect.GetWidth(), -m_shadowSize));
        out.SetSize(wxSize(m_shadowSize, windowRect.GetHeight() + 2 * m_shadowSize));
        return true;
    }

    return false;
}

void wxDropShadowFrameBase::OnAttachedMove(wxMoveEvent& evnt)
{
    RepositionFrame();
    evnt.Skip();
}

void wxDropShadowFrameBase::OnAttachedSize(wxSizeEvent& evnt)
{
    if (!IsShown() && !m_attachedFrame->IsMaximized()) {
        Show();
        StartFadeIn();
    }

    RepositionFrame();
    evnt.Skip();
}

void wxDropShadowFrameBase::OnAttachedMinimize(wxIconizeEvent& evnt)
{
    if (evnt.IsIconized()) {
        Hide();
    }
    else if (m_attachedFrame && !m_attachedFrame->IsMaximized()) {
        Show();
        StartFadeIn();
        RepositionFrame();
    }
    
    evnt.Skip();
}

void wxDropShadowFrameBase::OnAttachedMaximize(wxMaximizeEvent& evnt)
{
    Hide();
    evnt.Skip();
}

void wxDropShadowFrameBase::OnAttachedActivate(wxActivateEvent& evnt)
{
    m_attachedFrameActive = evnt.GetActive();

    if (evnt.GetActive()) {
        if (m_disableOnInactive) {
            RedrawShadow();
        }
        RepositionFrame(true);
    }
    else if (m_disableOnInactive) {
        RedrawShadow();
    }

    evnt.Skip();
}

void wxDropShadowFrameBase::OnAttachedDestroy(wxWindowDestroyEvent& evnt)
{
    DetachFromWindow();
    Destroy();
    evnt.Skip();
}

void wxDropShadowFrameBase::OnActivate(wxActivateEvent& evnt)
{
    if (evnt.GetActive() && m_attachedFrame) {
        CallAfter([this, &evnt]() {
            m_attachedFrame->SetFocus();
        });
    }
}

void wxDropShadowFrameBase::OnDpiChanged(wxDPIChangedEvent& evnt)
{
    RepositionFrame(true);
}

void wxDropShadowFrameBase::RedrawShadow()
{
    wxScreenDC windc;
    wxSize size = GetSize();
    if (!m_bmp.IsOk() || m_bmp.GetSize() != size)
    {
#ifdef _WIN32
        m_bmp = wxBitmap(size.x, size.y, windc);
#else
        m_bmp = wxBitmap(size.x, size.y, wxAlphaPixelFormat::BitsPerPixel);
#endif
    }

    {
        wxPixelData<wxBitmap, wxAlphaPixelFormat> pixels(m_bmp);
        wxPixelData<wxBitmap, wxAlphaPixelFormat>::Iterator it(pixels);

        wxSize windowSize;
        if (m_attachedFrame) {
            windowSize = m_attachedFrame->GetSize();
        }
        else if (m_part == wxSHADOW_TOP || m_part == wxSHADOW_BOTTOM) {
            windowSize = wxSize(size.x, 1000);
        }
        else {
            windowSize = wxSize(1000, size.y);
        }

        wxPoint windowOffset = GetWindowOffset(windowSize);
        bool drawShadow = !m_disableOnInactive || !m_attachedFrame || m_attachedFrameActive;

        for (int y = 0; y < size.y; ++y) {
            auto row = it;

            for (int x = 0; x < size.x; ++x, ++it) {
                it.Red() = 0;
                it.Green() = 0;
                it.Blue() = 0;
                it.Alpha() = drawShadow ? 
                    GetShadowIntensity(x + windowOffset.x, y + windowOffset.y, windowSize) : 1;
            }

            it = row;
            it.OffsetY(pixels, 1);
        }
    }

    wxMemoryDC memdc(m_bmp);
    UpdateWindowContents(windc, memdc);
}

void wxDropShadowFrameBase::Init()
{
    m_shadowSize = 10;
    m_shadowOffset = wxPoint(0, 2);
    m_attachedFrame = NULL;
    m_shadowAlpha = 168;
    m_currentShadowAlpha = 168;
    m_disableOnInactive = true;

    InitGaussianKernel();
}

void wxDropShadowFrameBase::OnTimerTick(wxTimerEvent& evnt)
{
    auto now = std::chrono::steady_clock::now();

    int currentMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_fadeInTimerStart).count();
    if (currentMs < FADE_IN_DELAY_MS) {
        return;
    }
    currentMs -= FADE_IN_DELAY_MS;

    if (currentMs > FADE_IN_TIME_MS) {
        m_currentShadowAlpha = m_shadowAlpha;
        m_fadeInTimer.Stop();
    }
    else {
        m_currentShadowAlpha = static_cast<wxByte>(currentMs / FADE_IN_TIME_MS * m_shadowAlpha);
    }

    RedrawShadow();
}

void wxDropShadowFrameBase::InitGaussianKernel()
{
    InitGaussianKernel(static_cast<int>(m_shadowSize / 2.5));
}

void wxDropShadowFrameBase::InitGaussianKernel(size_t stdDev)
{
    double stdDevDbl = static_cast<double>(stdDev);
    size_t arrayDim = 8 * stdDev + 1;
    int arrayDimCenter = static_cast<int>(4 * stdDev);

    m_gaussianCoeffs.resize(arrayDim * arrayDim);
    m_gaussianCoeffPrefixSums.resize(arrayDim * arrayDim);
    m_gaussianCoeffsDim = arrayDim;

    for (int y = 0, idx = 0; y < arrayDim; ++y) {
        for (int x = 0; x < arrayDim; ++x, ++idx) {
            m_gaussianCoeffs[idx] = CalcGaussianCoeff(stdDevDbl, x - arrayDimCenter, y - arrayDimCenter);
        }
    }

    for (int y = 0, idx = 0; y < arrayDim; ++y) {
        for (int x = 0; x < arrayDim; ++x, ++idx) {
            double top = (y > 0) ? (m_gaussianCoeffPrefixSums[idx - arrayDim]) : 0.;
            double left = (x > 0) ? (m_gaussianCoeffPrefixSums[idx - 1]) : 0.;
            double topLeft = (y > 0 && x > 0) ? (m_gaussianCoeffPrefixSums[idx - arrayDim - 1]) : 0.;

            m_gaussianCoeffPrefixSums[idx] = m_gaussianCoeffs[idx] + top + left - topLeft;
        }
    }

    double normFactor = 1. / m_gaussianCoeffPrefixSums.back();

    for (double& element : m_gaussianCoeffPrefixSums) {
        element *= normFactor;
    }
}

double wxDropShadowFrameBase::CalcGaussianCoeff(double stdDev, double x, double y)
{
    const double FACTOR = 1 / std::sqrt(2. * M_PI * stdDev * stdDev);
    return FACTOR * std::exp(-(x * x + y * y) / (2. * stdDev * stdDev));
}

void wxDropShadowFrameBase::RepositionFrame(bool force)
{
    wxRect placement;
    if (GetPlacementRect(placement)) {
        if (force || placement.GetPosition() != GetPosition()) {
            SetPosition(placement.GetPosition());
        }

        if (force || placement.GetSize() != GetSize()) {
            SetSize(placement.GetSize());
            RedrawShadow();
        }
    }
}

wxByte wxDropShadowFrameBase::GetShadowIntensity(int x, int y, wxSize windowSize) const
{
    x -= m_shadowOffset.x;
    y -= m_shadowOffset.y;

    wxRect windowRect(-x, -y, windowSize.x, windowSize.y);
    wxRect normalized = NormalizeRect(windowRect);
    
    wxByte intensity = static_cast<wxByte>(GetRangeSum(normalized) * m_currentShadowAlpha);
    return (intensity > 1) ? intensity : 1;
}

wxRect wxDropShadowFrameBase::NormalizeRect(const wxRect& rect) const
{
    wxPoint pos = rect.GetPosition();
    wxSize size = rect.GetSize();

    int maxDisplacement = (m_gaussianCoeffsDim / 2);
    wxRect newRect = rect;

    if (pos.x < -maxDisplacement) {
        size.x -= (-maxDisplacement) - pos.x;
        pos.x = -maxDisplacement;
    }
    if (pos.x > maxDisplacement) {
        size.x -= pos.x - maxDisplacement;
        pos.x = maxDisplacement;
    }
    if (pos.y < -maxDisplacement) {
        size.y -= (-maxDisplacement) - pos.y;
        pos.y = -maxDisplacement;
    }
    if (pos.y > maxDisplacement) {
        size.y -= pos.y - maxDisplacement;
        pos.y = maxDisplacement;
    }

    newRect.SetPosition(pos);
    newRect.SetSize(size);
    wxPoint pos2 = newRect.GetBottomRight();

    if (pos2.x < -maxDisplacement) {
        pos2.x = -maxDisplacement;
    }
    if (pos2.x > maxDisplacement) {
        pos2.x = maxDisplacement;
    }
    if (pos2.y < -maxDisplacement) {
        pos2.y = -maxDisplacement;
    }
    if (pos2.y > maxDisplacement) {
        pos2.y = maxDisplacement;
    }

    newRect.SetBottomRight(pos2);
    return newRect;
}

double wxDropShadowFrameBase::GetRangeSum(const wxRect& rect) const
{
    int gaussOffset = m_gaussianCoeffsDim / 2;

    wxPoint startPt = rect.GetTopLeft() + wxPoint(gaussOffset - 1, gaussOffset - 1);
    wxPoint endPt = rect.GetBottomRight() + wxPoint(gaussOffset, gaussOffset);

    double top = (startPt.y > 0) ? (m_gaussianCoeffPrefixSums[startPt.y * m_gaussianCoeffsDim + endPt.x]) : 0.;
    double left = (startPt.x > 0) ? (m_gaussianCoeffPrefixSums[endPt.y * m_gaussianCoeffsDim + startPt.x]) : 0.;
    double topLeft = (startPt.y > 0 && startPt.x > 0) ? (m_gaussianCoeffPrefixSums[startPt.y * m_gaussianCoeffsDim + startPt.x]) : 0.;

    return m_gaussianCoeffPrefixSums[endPt.y * m_gaussianCoeffsDim + endPt.x] - top - left + topLeft;
}

wxPoint wxDropShadowFrameBase::GetWindowOffset(const wxSize& windowSize) const
{
    switch (m_part) {
    case wxSHADOW_TOP:
        return wxPoint(0, -m_shadowSize);
    case wxSHADOW_BOTTOM:
        return wxPoint(0, windowSize.y);
    case wxSHADOW_LEFT:
        return wxPoint(-m_shadowSize, -m_shadowSize);
    case wxSHADOW_RIGHT:
        return wxPoint(windowSize.x, -m_shadowSize);
    default:
        return wxPoint();
    }
}
