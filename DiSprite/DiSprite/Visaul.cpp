#include "StdAfx.h"
#include "Visaul.h"
#include "DiAssist.h"
using namespace DiControl;


Visaul::Visaul(void): m_layout(0, 0, 100, 100)
{
}


Visaul::~Visaul(void)
{
}

void DiControl::Visaul::Render(Graphics& dc)
{
    OnRender(dc);
}

Size DiControl::Visaul::GetSize() const
{
    return Gdiplus::Size(m_layout.Width, m_layout.Height);
}

void DiControl::Visaul::SetSize(const Size& size)
{
    if (size.Width != m_layout.Width || size.Height != m_layout.Height)
    {
        m_layout.Width = size.Width;
        m_layout.Height = size.Height;

        OnSizeChanged(size);
    }
}

Point DiControl::Visaul::GetPosition() const
{
    return Gdiplus::Point(m_layout.GetLeft(), m_layout.GetTop());
}

void DiControl::Visaul::SetPostion(const Point& pos)
{
    if (pos.X != m_layout.X || pos.Y != m_layout.Y)
    {
        m_layout.X = pos.X;
        m_layout.Y = pos.Y;
        OnMoved(pos);
    }
}

const Rect& DiControl::Visaul::GetLayout() const
{
    return m_layout;
}

void DiControl::Visaul::SetLayout(const Rect& layout)
{
    SetPostion(Point(layout.X, layout.Y));
    SetSize(Size(layout.Width, layout.Height));
}

void DiControl::Visaul::SetMargin(const Thickness& margin)
{
    if (margin.Left != m_margin.Left ||
        m_margin.Top != m_margin.Top ||
        margin.Right != m_margin.Right ||
        margin.Bottom != m_margin.Bottom)
    {
        m_margin = margin;
        OnMarginChanged(margin);
    }
}

const Thickness& DiControl::Visaul::GetMargin() const
{
    return m_margin;
}

Rect DiControl::Visaul::GetClientArea() const
{
    Rect ca = m_layout;
    ca.Offset(-m_layout.X, - m_layout.Y);
    ca.Width -= m_margin.Left + m_margin.Right;
    ca.Height -= m_margin.Top + m_margin.Bottom;
    return ca;
}

DiControl::HitArea DiControl::Visaul::HitTest(const Point& pt) const
{
    if (m_layout.Contains(pt))
    {
        return OnHitTest(pt);
    }
    return HitArea::NotHit;
}

void DiControl::Visaul::OnRender(Graphics& dc)
{

}

void DiControl::Visaul::OnSizeChanged(const Size& nSize)
{

}

void DiControl::Visaul::OnMoved(const Point& pt)
{

}

void DiControl::Visaul::OnMarginChanged(const Thickness& margin)
{

}

DiControl::HitArea DiControl::Visaul::OnHitTest(const Point& pt) const
{
    HitArea hta = HitArea::Tranprant;

    Rect crc(
        m_layout.X + m_margin.Left, 
        m_layout.Y + m_margin.Top,
        m_layout.Width - m_margin.Left - m_margin.Right,
        m_layout.Height - m_margin.Top - m_margin.Bottom
        );

    Rect ltrc(m_layout.X, m_layout.Y, m_margin.Left, m_margin.Top);

    Rect lrc(
        m_layout.X, 
        m_layout.Y + m_margin.Top, 
        m_margin.Left,
        m_layout.Height - m_margin.Top - m_margin.Bottom
        );

    Rect lbrc(m_layout.X, 
        m_layout.Y + m_layout.Height - m_margin.Bottom,
        m_margin.Left, m_margin.Bottom);

    Rect trc(m_layout.X + m_margin.Left, m_layout.Y, 
        m_layout.Width - m_margin.Left - m_margin.Right,
        m_margin.Top);

    Rect rtrc(m_layout.X + m_layout.Width - m_margin.Right,
        m_layout.Y,
        m_margin.Right, m_margin.Top);

    Rect rrc(m_layout.X + m_layout.Width - m_margin.Right, 
        m_layout.Y + m_margin.Top,
        m_margin.Right,
        m_layout.Height - m_margin.Top - m_margin.Bottom
        );

    Rect rbrc(m_layout.X + m_layout.Width - m_margin.Right, 
        m_layout.Y + m_layout.Height - m_margin.Bottom,
        m_margin.Right, m_margin.Bottom);

    Rect brc(m_layout.X + m_margin.Left,
        m_layout.Y + m_layout.Height - m_margin.Bottom, 
        m_layout.Width - m_margin.Left - m_margin.Right,
        m_margin.Bottom);

    struct RectHit
    {
        Rect* rc;
        HitArea hitResult;
    } rhs[] = {
        {&crc, HitArea::Client},
        {&lrc, HitArea::LeftMargin},
        {&trc, HitArea::TopMargin},
        {&rrc, HitArea::RightMargin},
        {&brc, HitArea::BottomMargin},
        {&ltrc, HitArea::LeftTopCorner},
        {&rtrc, HitArea::RightTopCorner},
        {&rbrc, HitArea::RightBottomCorner},
        {&lbrc, HitArea::LeftBottomCorner}
    };

    int count = sizeof(rhs) / sizeof(RectHit);

    for (int i = 0; i < count; i++)
    {
        if (rhs[i].rc->Contains(pt))
        {
            hta = rhs[i].hitResult;
            break;
        }
    }

    return hta;
}

Rect Visaul::AdjustHitArea(HitArea area) const
{
    Rect rr;
    switch (area)
    {
    case HitArea::Client:
        {
            rr = Rect(
                m_layout.X + m_margin.Left, 
                m_layout.Y + m_margin.Top,
                m_layout.Width - m_margin.Left - m_margin.Right,
                m_layout.Height - m_margin.Top - m_margin.Bottom
                );
        }
        break;
    case HitArea::LeftTopCorner:
        {
            rr = Rect (m_layout.X, m_layout.Y, m_margin.Left, m_margin.Top);
        }
        break;
    case HitArea::LeftBottomCorner:
        {
            rr = Rect (m_layout.X, 
                m_layout.Y + m_layout.Height - m_margin.Bottom,
                m_margin.Left, m_margin.Bottom);
        }
        break;
    case HitArea::RightTopCorner:
        {
            rr  =Rect (m_layout.X + m_layout.Width - m_margin.Right,
                m_layout.Y,
                m_margin.Right, m_margin.Top);
        }
        break;
    case HitArea::RightBottomCorner:
        {
            rr = Rect (m_layout.X + m_layout.Width - m_margin.Right, 
                m_layout.Y + m_layout.Height - m_margin.Bottom,
                m_margin.Right, m_margin.Bottom);
        }
        break;
    case HitArea::LeftMargin:
        {
            rr=  Rect (
                m_layout.X, 
                m_layout.Y + m_margin.Top, 
                m_margin.Left,
                m_layout.Height - m_margin.Top - m_margin.Bottom
                );
        }
        break;
    case HitArea::TopMargin:
        {
            rr = Rect (m_layout.X + m_margin.Left, m_layout.Y, 
                m_layout.Width - m_margin.Left - m_margin.Right,
                m_margin.Top);
        }
        break;
    case HitArea::RightMargin:
        {
            rr = Rect (m_layout.X + m_layout.Width - m_margin.Right, 
                m_layout.Y + m_margin.Top,
                m_margin.Right,
                m_layout.Height - m_margin.Top - m_margin.Bottom
                );
        }
        break;
    case HitArea::BottomMargin:
        {
            rr = Rect (m_layout.X + m_margin.Left,
                m_layout.Y + m_layout.Height - m_margin.Bottom, 
                m_layout.Width - m_margin.Left - m_margin.Right,
                m_margin.Bottom);
        }
        break;
    default: 
        break;
    }
    return rr;
}

DiControl::ImageVisaul::ImageVisaul():m_image(nullptr), m_imagePath(_T(""))
{

}

DiControl::ImageVisaul::~ImageVisaul()
{
    SafeDelete(&m_image);
}

void DiControl::ImageVisaul::SetImageUri(const CString& uri)
{
    if (uri != m_imagePath)
    {
        SafeDelete(&m_image);
        if (::PathFileExists(uri))
        {
            m_image = Image::FromFile(uri);
            SetSize(GetImageSize());
        }
        m_imagePath = uri;
    }
}

const CString& DiControl::ImageVisaul::GetImageUri() const
{
    return m_imagePath;
}

Size DiControl::ImageVisaul::GetImageSize() const
{
    if (nullptr != m_image)
    {
        return Size(m_image->GetWidth(), m_image->GetHeight());
    }
    return Size();
}

void DiControl::ImageVisaul::OnRender(Graphics& dc)
{
    if (nullptr != m_image)
    {
        dc.DrawImage(m_image, 0, 0, 0, 0, m_image->GetWidth(), m_image->GetHeight(), Gdiplus::UnitPixel);
    }
}
