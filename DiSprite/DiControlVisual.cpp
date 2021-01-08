#include "StdAfx.h"
#include "DiControlVisual.h"
#include <algorithm>
#include "DiSpriteDoc.h"
#include "DiSprite.h"
#include "DiAssist.h"
using namespace DiControl;

DiControlVisual::DiControlVisual(void): m_control(nullptr), m_selected(false)
{
    SetSize(Size(120, 50));
    SetMargin(Thickness(5));
	m_backgroundClr = Gdiplus::Color(180, 89, 89, 255);
}


DiControlVisual::~DiControlVisual(void)
{
}

void DiControlVisual::SetControl(DiControlPri* ctrl)
{
    if (ctrl != m_control)
    {
        m_control = ctrl;
    }
    UpdateControl();
}

DiControlPri* DiControlVisual::GetControl() const
{
    return m_control;
}

void DiControlVisual::Selected(bool selected)
{
    m_selected = selected;
}

bool DiControlVisual::Selected() const
{
    return m_selected;
}

void DiControlVisual::UpdateControl()
{
    if (m_control != nullptr)
    {
        SetLayout(Gdiplus::Rect(m_control->Left(), m_control->Top(), m_control->Right() - m_control->Left(), m_control->Bottom() - m_control->Top()));
    }
}

void DiControlVisual::OnRender(Graphics& dc)
{
    Gdiplus::RectF ctrlRc(static_cast<float>(GetLayout().X), static_cast<float>(GetLayout().Y), static_cast<float>(GetLayout().Width), static_cast<float>(GetLayout().Height));

    Gdiplus::Pen borderPen(Gdiplus::Color::BlueViolet);
    Gdiplus::SolidBrush background(m_backgroundClr);
    dc.FillRectangle(&background, ctrlRc);
    dc.DrawRectangle(&borderPen, ctrlRc);

    if (nullptr != m_control)
    {
        Gdiplus::StringFormat sf;
        sf.SetLineAlignment(StringAlignmentCenter);
        sf.SetAlignment(StringAlignmentCenter);
        sf.SetTrimming(StringTrimmingEllipsisCharacter);
		CString strFont = _T("Microsoft YaHei UI");
        Gdiplus::Font font(strFont.AllocSysString(), 12);
        Gdiplus::SolidBrush foreground(Gdiplus::Color::Black);
		CString strName =  m_control->Name();
        dc.DrawString(
			(LPCWSTR)strName,
            m_control->Name().GetLength(), 
            &font, 
            ctrlRc,
            &sf, 
            &foreground); 
    }

    if (m_selected)
    {
        Gdiplus::SolidBrush designCorBrush(Gdiplus::Color::Red);

        HitArea has[] = {
            HitArea::LeftTopCorner,
            HitArea::RightTopCorner,
            HitArea::RightBottomCorner,
            HitArea::LeftBottomCorner
        };

        int count = sizeof(has) / sizeof(HitArea);

        for (int i = 0; i < count; i++)
        {
			Rect rc = AdjustHitArea(has[i]);
            dc.FillRectangle(&designCorBrush, rc);

			switch(has[i])
			{
			case HitArea::LeftTopCorner:
				{
					rc.Offset(GetSize().Width / 2 - rc.Width / 2, 0);
					dc.FillRectangle(&designCorBrush, rc);
					
					rc.Offset(-GetSize().Width / 2 + rc.Width / 2, GetSize().Height / 2 - rc.Width / 2);
					dc.FillRectangle(&designCorBrush, rc);

				}
				break;
			case HitArea::RightBottomCorner:
				{
					rc.Offset(-GetSize().Width / 2 + rc.Width / 2, 0);
					dc.FillRectangle(&designCorBrush, rc);

					rc.Offset(+GetSize().Width / 2 - rc.Width / 2, -GetSize().Height / 2 + rc.Width / 2);
					dc.FillRectangle(&designCorBrush, rc);
				}
				break;
			default:
				break;
			}
			

        }


    }
}

void DiControlVisual::OnSizeChanged(const Size& nSize)
{
    if (nullptr != m_control)
    {
        m_control->Right(m_control->Left() + nSize.Width);
        m_control->Bottom(m_control->Top() + nSize.Height);
    }
}

void DiControlVisual::OnMoved(const Point& pt)
{
    if (nullptr != m_control)
    {
        m_control->Left(pt.X);
        m_control->Top(pt.Y);
        m_control->Right(m_control->Left() + GetLayout().Width);
        m_control->Bottom(m_control->Top() + GetLayout().Height);
    }
}

DiControlVisual* DiControl::DiControlVisual::CreateDiControlVisual(DiControlPri* ctrl)
{
    if (nullptr == ctrl)
    {
        return nullptr;
    }
    DiControlVisual* cv = new DiControlVisual();
    cv->SetControl(ctrl);
    return cv;
}

DiControl::DiImagePageVisual::DiImagePageVisual(): m_imagePage(nullptr), m_curSelDCtrlVisual(nullptr)
{
}

DiControl::DiImagePageVisual::~DiImagePageVisual()
{
    Dispose();
}

void DiControl::DiImagePageVisual::SetImagePage(DiImagePage* imagePage)
{
    if (imagePage != m_imagePage)
    {
        Dispose();
        if (nullptr != imagePage)
        {
            int count  = imagePage->GetControlCount();
            for	(int i = 0; i < count; i++)
            {
                DiControlPri* dICtrl = imagePage->GetControl(i);
                if (nullptr != dICtrl)
                {
                    shared_ptr<DiControlVisual> dv(DiControlVisual::CreateDiControlVisual(dICtrl));
					if (dICtrl->ControlType() <= static_cast<BYTE>(DiCtrlType::TextInput))
					{
						dv->BackgroundClr(Gdiplus::Color(180, 255, 255, 55));
					}
                    m_ctrlVisuals.insert(make_pair(dICtrl, dv));
                }
            }
            SetImageUri(imagePage->ImagePath()); 
        }
        m_imagePage = imagePage;
    }
    else
    {
        UpdateImagePage();
    }

	CDiSpriteDoc* pDoc = theApp.GetDiDocument();
	if (NULL != pDoc)
	{
		SIZE scSize = pDoc->CurrentProject()->ScreenSize();
		SetSize(Gdiplus::Size(scSize.cx, scSize.cy));
	}
}

DiImagePage* DiControl::DiImagePageVisual::GetImagePage() const
{
    return m_imagePage;
}

int DiControl::DiImagePageVisual::GetSelectedVisuals(std::list<DiControlVisual*>& visuals) const
{
	int count = 0;
	for_each(m_ctrlVisuals.begin(), m_ctrlVisuals.end(), [&](map<DiControlPri*, shared_ptr<DiControlVisual>>::value_type cv)
	{
		if (cv.second.get() != nullptr && cv.second.get()->Selected())
		{
			visuals.push_back(cv.second.get());
			count++;
		}
	});

	return count;
}

int DiControl::DiImagePageVisual::GetSelCount() const
{
	int count = 0;
	for_each(m_ctrlVisuals.begin(), m_ctrlVisuals.end(), [&](map<DiControlPri*, shared_ptr<DiControlVisual>>::value_type cv)
	{
		if (cv.second.get() != nullptr && cv.second.get()->Selected())
		{
			count++;
		}
	});

	return count;
}

DiControlVisual* DiImagePageVisual::GetControlVisual(DiControlPri* ctrl) const
{
    DiControlVisual* rv = nullptr;
    if (nullptr != ctrl)
    {
        auto it = m_ctrlVisuals.find(ctrl);
        if (it != m_ctrlVisuals.end())
        {
            rv = it->second.get();
        }
    }
    return rv;
}

int DiImagePageVisual::GetControlVisualCount() const
{
    return m_ctrlVisuals.size();
}

int DiImagePageVisual::GetControlVisuals(std::list<DiControlVisual*>& visuals) const
{
    int count = 0;
    for_each(m_ctrlVisuals.begin(), m_ctrlVisuals.end(), [&](map<DiControlPri*, shared_ptr<DiControlVisual>>::value_type cv)
    {
        if (cv.second.get() != nullptr)
        {
            visuals.push_back(cv.second.get());
            count++;
        }
    });

    return count;
}

void DiImagePageVisual::SelectedControl(DiControlPri* selCtrl, bool bMultiSel)
{
	if (!bMultiSel)
	{
		for_each(m_ctrlVisuals.begin(), m_ctrlVisuals.end(), [&](map<DiControlPri*, shared_ptr<DiControlVisual>>::value_type cv)
		{
			if (cv.second.get() != nullptr)
			{
				cv.second.get()->Selected(false);
			}
		});
	}

    DiControlVisual* cv = GetControlVisual(selCtrl);

    if (nullptr != cv)
    {
        cv->Selected(true);
    }
	m_curSelDCtrlVisual = cv;
}

DiControlVisual* DiImagePageVisual::GetSelectedVisual() const
{
    DiControlVisual* cv = nullptr;
    map<DiControlPri*, shared_ptr<DiControlVisual>>::const_iterator it = std::find_if(m_ctrlVisuals.begin(), m_ctrlVisuals.end(), [](map<DiControlPri*, shared_ptr<DiControlVisual>>::value_type cv){return cv.second->Selected();});

    if (it != m_ctrlVisuals.end())
    {
        cv = it->second.get();
    }
    return cv;
}

void DiImagePageVisual::UpdateCtrlVisual(DiControlPri* ctrl) const
{
    DiControlVisual* cv = GetControlVisual(ctrl);
    if (nullptr != cv)
    {
        cv->UpdateControl();
    }
}

void DiImagePageVisual::UpdateImagePage()
{
    if (nullptr != m_imagePage)
    {
        SetImageUri(m_imagePage->ImagePath());

        map<DiControlPri*, shared_ptr<DiControlVisual>> tempCtrlVisual;

        int count  = m_imagePage->GetControlCount();
        for	(int i = 0; i < count; i++)
        {
            DiControlPri* dICtrl = m_imagePage->GetControl(i);
            if (nullptr != dICtrl)
            {
                auto it = m_ctrlVisuals.find(dICtrl);

                if (it != m_ctrlVisuals.end())
                {
                    if (it->second != nullptr)
                    {
                        it->second.get()->UpdateControl();
                        tempCtrlVisual.insert(make_pair(dICtrl, it->second));
                    }
                }
                else
                {
                    shared_ptr<DiControlVisual> dv(DiControlVisual::CreateDiControlVisual(dICtrl));
					if (dICtrl->ControlType() <= static_cast<BYTE>(DiCtrlType::TextInput))
					{
						dv->BackgroundClr(Gdiplus::Color(180, 255, 255, 55));
					}
                    tempCtrlVisual.insert(make_pair(dICtrl, dv));
                }

            }
        }

        m_ctrlVisuals.clear();
        m_ctrlVisuals = tempCtrlVisual;
    }
}

void DiControl::DiImagePageVisual::Dispose()
{
    m_ctrlVisuals.clear();
	SetImageUri(_T(""));
	m_curSelDCtrlVisual = nullptr;
}

void DiImagePageVisual::OnRender(Graphics& dc)
{
    __super::OnRender(dc);

    for_each(m_ctrlVisuals.begin(), m_ctrlVisuals.end(), [&](map<DiControlPri*, shared_ptr<DiControlVisual>>::value_type cv)
    {
        if (cv.second.get() != nullptr)
        {
            cv.second.get()->Render(dc);
			if (cv.second.get() == m_curSelDCtrlVisual)
			{
				Rect erc = cv.second.get()->GetLayout();

				Gdiplus::Pen borderPen(Gdiplus::Color::Red, 2.f);
				
				dc.DrawRectangle(&borderPen, erc);
			}
        }
    });
}
