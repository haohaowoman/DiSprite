// DiSpriteView.cpp : CDiSpriteView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DiSprite.h"
#endif

#include "DiSpriteDoc.h"
#include "DiSpriteView.h"
#include "MainFrm.h"
#include "DiButton.h"
#include "DiControlFactory.h"
#include <algorithm>

using namespace DiControl;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDiSpriteView

LPCTSTR const CF_DICTRL_FORMAT = _T("CF_DICTRL");

IMPLEMENT_DYNCREATE(CDiSpriteView, CScrollView)

BEGIN_MESSAGE_MAP(CDiSpriteView, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDiSpriteView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_NEWDICONTROL, &CDiSpriteView::OnNewdicontrol)
	ON_UPDATE_COMMAND_UI(ID_NEWDICONTROL, &CDiSpriteView::OnUpdateNewdicontrol)
    ON_WM_ERASEBKGND()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
	ON_WM_KEYUP()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_EDIT_CLEAR, &CDiSpriteView::OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, &CDiSpriteView::OnUpdateEditClear)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND_RANGE(ID_TOOL_DATA_INPUT, ID_TOOL_BARCODES, &CDiSpriteView::OnAddControlRange)

	ON_COMMAND_RANGE(ID_ALIGN_LEFT, ID_ALIGN_BOTTOM, &CDiSpriteView::OnAlign)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ALIGN_LEFT, ID_ALIGN_BOTTOM, &CDiSpriteView::OnAlignUpdate)
	ON_COMMAND(ID_EDIT_COPY, &CDiSpriteView::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CDiSpriteView::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CDiSpriteView::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CDiSpriteView::OnUpdateEditPaste)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CDiSpriteView 构造/析构

CDiSpriteView::CDiSpriteView():
    m_editImagePage(nullptr),
    m_editControl(nullptr), m_lastHitArea(), m_bDragMode(false)
{
    // TODO: 在此处添加构造代码
}

CDiSpriteView::~CDiSpriteView()
{
}

BOOL CDiSpriteView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CDiSpriteView 绘制

void CDiSpriteView::UpdateEditControVisual()
{
    if (nullptr != m_editControl)
    {
        m_imagePageVisual.UpdateCtrlVisual(m_editControl);
        Invalidate(FALSE);
    }
}

void CDiSpriteView::OnDraw(CDC* pDC)
{
	CDiSpriteDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//
	CDC memDc;
    memDc.CreateCompatibleDC(pDC);
    CBitmap memBmp;
    CRect rc;
    GetClientRect(rc);
	rc.right = max(m_imagePageVisual.GetSize().Width + 5, rc.right);
	rc.bottom = max(m_imagePageVisual.GetSize().Height + 5, rc.bottom);
    memBmp.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
    memDc.SelectObject(&memBmp);

    Gdiplus::Graphics gp(memDc.GetSafeHdc());
	gp.Clear(Gdiplus::Color::WhiteSmoke);

    m_imagePageVisual.Render(gp);

	/*memDc.Rectangle(m_imagePageVisual.GetLayout().GetLeft(), m_imagePageVisual.GetLayout().GetTop(),
		m_imagePageVisual.GetLayout().GetRight(), m_imagePageVisual.GetLayout().GetBottom());*/

    if (m_bDragMode || m_bMultiSelDrag)
    {
        //Draw drag rect.
        Gdiplus::Pen dp(Gdiplus::Color::Black, 1);
        dp.SetDashStyle(DashStyle::DashStyleDashDotDot);
        gp.DrawRectangle(&dp, m_dragRect);

    }
    pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &memDc, 0, 0, SRCCOPY);

    memDc.DeleteDC();
    memBmp.DeleteObject();
	/*if (nullptr != m_editImagePage)
	{
        
		Gdiplus::Image image(m_editImagePage->ImagePath());

	    
	    {
	        
	    }

        CDC memDc;
        memDc.CreateCompatibleDC(pDC);
        CBitmap memBmp;
        memBmp.CreateCompatibleBitmap(pDC, image.GetWidth(), image.GetHeight());
        memDc.SelectObject(&memBmp);

        Gdiplus::Graphics gp(memDc.GetSafeHdc());
	    gp.Clear(Gdiplus::Color::WhiteSmoke);
        m_imagePageVisual.Render(gp);
		gp.DrawImage(&image, 0, 0, 0, 0, image.GetWidth(), image.GetHeight(), Gdiplus::UnitPixel);

		int count = m_editImagePage->GetControlCount();
		for (int i = 0; i < count; i++)
		{
			DiControlPri* pCtrl = m_editImagePage->GetControl(i);
			if (nullptr != pCtrl)
			{
				Gdiplus::RectF ctrlRc((float)pCtrl->Left(), (float)pCtrl->Top(), (float)(pCtrl->Right() - pCtrl->Left()), (float)(pCtrl->Bottom() - pCtrl->Top()));

				Gdiplus::Pen borderPen(Gdiplus::Color::BlueViolet);
				Gdiplus::SolidBrush background(Gdiplus::Color(200, 255, 255, 255));
				gp.FillRectangle(&background, ctrlRc);
				gp.DrawRectangle(&borderPen, ctrlRc);
                Gdiplus::StringFormat sf;
                sf.SetLineAlignment(StringAlignmentCenter);
                sf.SetAlignment(StringAlignmentCenter);
                sf.SetTrimming(StringTrimmingEllipsisCharacter);
				Gdiplus::Font font(_T("Microsoft YaHei UI"), 16);
				Gdiplus::SolidBrush foreground(Gdiplus::Color::Black);
				gp.DrawString(
					pCtrl->Name(),
					pCtrl->Name().GetLength(), 
					&font, 
					ctrlRc,
					&sf, 
					&foreground);
			}
			
			
		}

        pDC->BitBlt(0, 0, image.GetWidth(), image.GetWidth(), &memDc, 0, 0, SRCCOPY);

        memDc.DeleteDC();
        memBmp.DeleteObject();
	}*/
}

void CDiSpriteView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	ModifyStyle(NULL, WS_BORDER);
	ModifyStyleEx(NULL, WS_EX_STATICEDGE);
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED | SWP_DRAWFRAME);
	CSize sizeTotal;
	sizeTotal.cx = GetDocument()->CurrentProject()->ScreenSize().cx + 5;
	sizeTotal.cy = GetDocument()->CurrentProject()->ScreenSize().cy + 5;
	SetScrollSizes(MM_TEXT, sizeTotal);

	// Update project views.
	theApp.GetMainFarme()->GetFileView()->RefreshProjectFiles();
    theApp.GetMainFarme()->GetPropertysView()->RefreshProperty();
	m_createCtrlType = DiCtrlType::Button;
	m_bMultiSel = false;
	m_bMultiSelDrag = false;

	AddClipboardFormatListener(m_hWnd);
}


// CDiSpriteView 打印


void CDiSpriteView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDiSpriteView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDiSpriteView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDiSpriteView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CDiSpriteView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDiSpriteView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDiSpriteView 诊断

#ifdef _DEBUG
void CDiSpriteView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDiSpriteView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}


CDiSpriteDoc* CDiSpriteView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDiSpriteDoc)));
	return (CDiSpriteDoc*)m_pDocument;
}
#endif //_DEBUG

void CDiSpriteView::CheckControlRect(Rect &vl)
{
	Rect irc = m_imagePageVisual.GetLayout();

	if (vl.X < 0)
	{
		vl.Offset(-vl.X, 0);
	}

	if (vl.Y < 0)
	{
		vl.Offset(0, -vl.Y);
	}

	if (vl.GetRight() > irc.GetRight())
	{
		vl.Offset(irc.GetRight() - vl.GetRight(), 0);
	}

	if (vl.GetBottom() > irc.GetBottom())
	{
		vl.Offset(0, irc.GetBottom() - vl.GetBottom());
	}

	vl.X = max(0, vl.X);
	vl.Y = max(0, vl.Y);

	vl.Width = max(5, vl.Width);
	vl.Height = max(5, vl.Height);
}

void CDiSpriteView::SetEditImagePage(DiImagePage* image)
{
	if (m_editImagePage != image)
	{
		m_editImagePage = image;
		SetEditControl(nullptr);
	}
    m_imagePageVisual.SetImagePage(image);

	theApp.GetMainFarme()->GetPropertysView()->RefreshProperty();
	Invalidate(TRUE);
}

DiImagePage* CDiSpriteView::GetEditImagePage() const
{
	return m_editImagePage;
}

void CDiSpriteView::SetEditControl(DiControlPri* ctrl)
{
	if (m_editControl != ctrl)
	{
		m_editControl = ctrl;
		
	}
    m_imagePageVisual.SelectedControl(ctrl, m_bMultiSel);
    
    theApp.GetMainFarme()->GetPropertysView()->RefreshProperty();
    Invalidate(FALSE);
}

DiControlPri* CDiSpriteView::GetEditControl() const
{
	return m_editControl;
}




// CDiSpriteView 消息处理程序

void CDiSpriteView::OnNewdicontrol()
{
	// 
	DiButton data;
	DiControlPri* nCtrl = DiControFactory::CreateControl(m_createCtrlType);

	if (nullptr != nCtrl && nullptr != m_editImagePage)
	{
		if (m_imagePageVisual.GetLayout().Contains(Gdiplus::Point(m_lastDownPt.x, m_lastDownPt.y)))
		{
			SIZE cs = nCtrl->Size();
			
			if (m_lastDownPt.x + cs.cx > m_imagePageVisual.GetLayout().GetRight())
			{
				m_lastDownPt.Offset(m_imagePageVisual.GetLayout().GetRight() - (m_lastDownPt.x + cs.cx), 0);
			}

			if (m_lastDownPt.y + cs.cy > m_imagePageVisual.GetLayout().GetBottom())
			{
				m_lastDownPt.Offset(0, m_imagePageVisual.GetLayout().GetBottom() - (m_lastDownPt.y + cs.cy));
			}

			nCtrl->Location(m_lastDownPt);
			
			/*rc.SetRect(m_lastDownPt.x, m_lastDownPt.y, m_lastDownPt.x, m_lastDownPt.y + 50);
			nCtrl->Left(rc.left);
			nCtrl->Top(rc.top);
			nCtrl->Right(rc.right);
			nCtrl->Bottom(rc.bottom);*/

			if (nCtrl->Right() > m_imagePageVisual.GetLayout().GetRight())
			{
				
			}

		}

		m_editImagePage->AddControl(nCtrl);
        nCtrl->ScreenIndex(m_editImagePage->PageIndex());

		m_imagePageVisual.SetImagePage(m_editImagePage);
		SetEditControl(nCtrl);
		theApp.GetMainFarme()->GetFileView()->RefreshProjectFiles();
        Invalidate(FALSE);
	}
}


void CDiSpriteView::OnUpdateNewdicontrol(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetEditImagePage() != nullptr);
}


BOOL CDiSpriteView::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
    return CScrollView::OnEraseBkgnd(pDC);
}


void CDiSpriteView::ChangeCursor(HitArea ha) const
{
    LPCTSTR IDhCursor = IDC_ARROW;
    switch (ha)
    {
    case HitArea::Client:
        IDhCursor = IDC_SIZEALL;
        break;
    case HitArea::RightBottomCorner:
    case HitArea::LeftTopCorner: 
        IDhCursor = IDC_SIZENWSE;
        break;
    case HitArea::LeftBottomCorner:
    case HitArea::RightTopCorner: 
        IDhCursor = IDC_SIZENESW;
        break;
    case HitArea::RightMargin: 
    case HitArea::LeftMargin: 
        IDhCursor = IDC_SIZEWE;
        break;
    case HitArea::TopMargin:
    case HitArea::BottomMargin:
        IDhCursor = IDC_SIZENS;
        break;
    default: 
        break;
    }

    ::SetClassLong(m_hWnd, GCL_HCURSOR, reinterpret_cast<LONG>(::LoadCursor(NULL, IDhCursor)));
}

void CDiSpriteView::OnMouseMove(UINT nFlags, CPoint point)
{
	static CPoint sLastPt = point;
    // 
	CPoint spt = GetScrollPosition();

	point.Offset(spt);
    if (m_imagePageVisual.GetLayout().Contains(Point(point.x, point.y)))
    {
        HitArea ha = HitArea::NotHit;

        if (!m_bMultiSelDrag && nullptr != m_editControl)
        {
			DiControlVisual* dv = m_imagePageVisual.GetControlVisual(m_editControl);

            if (nullptr != dv)
            {
				/*list<DiControlVisual*> sels;

				const int selCount = m_imagePageVisual.GetSelectedVisuals(sels);

				for (auto sv = sels.rbegin(); sv != sels.rend(); ++sv)
				{
				if (nullptr != *sv)
				{
				ha = dv->HitTest(Gdiplus::Point(point.x, point.y));
				if (HitArea::NotHit != ha && HitArea::Tranprant != ha)
				{
				dv = *sv;
				}
				}
				}
				*/
				ha = dv->HitTest(Gdiplus::Point(point.x, point.y));
                if (nFlags & MK_LBUTTON)
                {
					int des = (int)sqrt((double)(point.x - m_lastHitPt.X) * (point.x - m_lastHitPt.X) + (double)(point.y - m_lastHitPt.Y) * (point.y - m_lastHitPt.Y));
					if (des >= 5)
					{
						// Drag move or size.
						m_dragRect = dv->GetLayout();

						m_bDragMode = true;

						switch (m_lastHitArea)
						{
						case HitArea::Client:
							{
								// Move. 
								list<DiControlVisual*> sels;
								const int selCount = m_imagePageVisual.GetSelectedVisuals(sels);

								if (selCount > 1)
								{
									for (auto sv = sels.rbegin(); sv != sels.rend(); ++sv)
									{
										if (nullptr != *sv)
										{
											Rect src = (*sv)->GetLayout();
											src.Offset(point.x - sLastPt.x, point.y - sLastPt.y);
											
											CheckControlRect(src);

											(*sv)->SetLayout(src);
										}
									}

									m_bDragMode = false;
								}
								else
								{
									Rect irc = m_imagePageVisual.GetLayout();
									m_dragRect.Offset(point.x - m_lastHitPt.X, point.y - m_lastHitPt.Y);
									int mr = irc.GetRight() - m_dragRect.GetRight();
									if (mr < 0)
									{
										m_dragRect.Offset(mr, 0);
									}
									int mb = irc.GetBottom() - m_dragRect.GetBottom();
									if (mb < 0)
									{
										m_dragRect.Offset(0, mb);
									}
								}
							}
							break;
						case HitArea::LeftTopCorner:
							{
								CPoint rb(m_dragRect.GetRight(), m_dragRect.GetBottom());
								if (point.x < rb.x)
								{
									m_dragRect.X = point.x;
									m_dragRect.Width = abs(rb.x - point.x);
								}
								
								if (point.y < rb.y)
								{
									m_dragRect.Y = point.y;
									m_dragRect.Height = abs(rb.y - point.y);
								}
							}
							break;
						case HitArea::LeftBottomCorner:
							{
								CPoint rt(m_dragRect.GetRight(), m_dragRect.GetTop());
								if (point.x < rt.x)
								{
									m_dragRect.X = point.x;
									m_dragRect.Width = abs(point.x - rt.x);
								}
								
								if (point.y > rt.y)
								{
									m_dragRect.Height = (point.y - rt.y);
								}
								
							}
							break;
						case HitArea::RightTopCorner:
							{
								CPoint lb(m_dragRect.X, m_dragRect.GetBottom());
								if (point.x > lb.x)
								{
									m_dragRect.Width = (point.x - lb.x);
								}

								if (point.y < lb.y)
								{
									m_dragRect.Y = point.y;
									m_dragRect.Height = abs(point.y - lb.y);
								}
							}
							break;
						case HitArea::RightBottomCorner:
							{
								CPoint lt(m_dragRect.X, m_dragRect.Y);
								m_dragRect.Width = abs(point.x - lt.x);
								m_dragRect.Height = abs(point.y - lt.y);
							}
							break;
						case HitArea::LeftMargin:
							{
								long r = m_dragRect.GetRight();
								if (point.x < r)
								{
									m_dragRect.X = point.x;
									m_dragRect.Width = (r - point.x);
								}
							}
							break;
						case HitArea::TopMargin:
							{
								long b = m_dragRect.GetBottom();
								if (point.y < b)
								{
									m_dragRect.Y = point.y;
									m_dragRect.Height = (b - point.y);
								}
								
							}
							break;
						case HitArea::RightMargin:
							{
								int r = m_dragRect.GetRight();
								m_dragRect.Width += point.x - r;
							}
							break;
						case HitArea::BottomMargin:
							{
								int b = m_dragRect.GetBottom();
								m_dragRect.Height += point.y - b;
							}
							break;
						default: 
							break;
						}

						m_dragRect.X = max(0, m_dragRect.X);
						m_dragRect.Y = max(0, m_dragRect.Y);

						m_dragRect.Width = max(5, m_dragRect.Width);
						m_dragRect.Height = max(5, m_dragRect.Height);
					}
					else
					{
						m_bDragMode = false;
					}

					Invalidate(FALSE);
                }
            }
        }
		else
		{
			if ((nFlags & MK_LBUTTON) && m_bMultiSelDrag)
			{
				m_dragRect.X = min(m_lastHitPt.X, point.x);
				m_dragRect.Y = min(m_lastHitPt.Y, point.y);

				m_dragRect.Width = abs(m_lastHitPt.X - point.x);
				m_dragRect.Height = abs(m_lastHitPt.Y - point.y);

				list<DiControlVisual*> ctrls;

				if (0 < m_imagePageVisual.GetControlVisuals(ctrls))
				{
					for_each(ctrls.begin(), ctrls.end(), [&](DiControlVisual* dv)
					{
						Rect c;
						if (Rect::Intersect(c, m_dragRect, dv->GetLayout()))
						{
							dv->Selected(true);
						}
						else
						{
							dv->Selected(false);
						}
					});
				}

				Invalidate(FALSE);
			}
			else
			{
				m_bMultiSelDrag = false;
			}
		}

		if (!m_bDragMode)
		{
			ChangeCursor(ha);
		}
    }
	sLastPt = point;
    CScrollView::OnMouseMove(nFlags, point);
}


void CDiSpriteView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    bool reSel = true;
	CPoint spt = GetScrollPosition();

	m_bMultiSel = (nFlags & MK_CONTROL) == MK_CONTROL;

	point.Offset(spt);

	if (m_imagePageVisual.GetLayout().Contains(Point(point.x, point.y)))
	{
		m_lastDownPt = point;
	}

    HitArea ha = HitArea::NotHit;
	DiControlPri* editCtrl = m_editControl;

    if (!m_bMultiSel && nullptr != m_editControl)
    {
        DiControlVisual* dv = m_imagePageVisual.GetControlVisual(m_editControl);
        if (nullptr != dv)
        {
            ha = dv->HitTest(Gdiplus::Point(point.x, point.y));
            reSel = HitArea::NotHit == ha || HitArea::Tranprant == ha;
        }
    }

    if (reSel)
    {
        list<DiControlVisual*> visuals;
        int count =  m_imagePageVisual.GetControlVisuals(visuals);

        for (list<DiControlVisual*>::reverse_iterator i = visuals.rbegin(); i != visuals.rend(); ++i)
        {
            DiControlVisual* dv = *i;
            if (nullptr != dv)
            {
                ha = dv->HitTest(Gdiplus::Point(point.x, point.y));

                if (HitArea::NotHit != ha && HitArea::Tranprant != ha)
                {
					editCtrl = dv->GetControl();
					/*if (m_bMultiSel && m_imagePageVisual.GetSelCount() > 1 && dv->Selected())
					{
						dv->Selected(false);
						if (dv->GetControl() == m_editControl)
						{
							DiControlVisual* tsDv = m_imagePageVisual.GetSelectedVisual();
							if (nullptr != tsDv)
							{
								editCtrl = tsDv->GetControl();
							}
							else
							{
								editCtrl = nullptr;
							}
						}
					}
					else
					{
						editCtrl = dv->GetControl();
					}
                    */
                    break;
                }
            } 
        }
    }

	if (ha == HitArea::NotHit || ha == HitArea::Tranprant)
	{
		editCtrl = nullptr;
		m_bMultiSelDrag = true;
	}
	else
	{
		m_bMultiSelDrag = false;
	}
	// Select the control.
	SetEditControl(editCtrl);

    m_lastHitPt.X = point.x;
    m_lastHitPt.Y = point.y;
    m_lastHitArea = ha;
    this->SetCapture();
	SetFocus();
    ChangeCursor(ha);
	Invalidate(FALSE);
    CScrollView::OnLButtonDown(nFlags, point);
}


void CDiSpriteView::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
	CPoint spt = GetScrollPosition();

	point.Offset(spt);

    if (m_bDragMode)
    {
        DiControlVisual* dv = m_imagePageVisual.GetControlVisual(m_editControl);

        if (nullptr != dv)
        {
            // Drag move or size.
            Rect oldLayout = dv->GetLayout();
            
            if (!oldLayout.Equals(m_dragRect))
            {
                dv->SetLayout(m_dragRect);
                theApp.GetMainFarme()->GetPropertysView()->RefreshProperty();
            }

        }

    }
	else if (m_bMultiSelDrag)
	{

		list<DiControlVisual*> ctrls;
		DiControlVisual* teDv = nullptr;

		if (0 < m_imagePageVisual.GetSelectedVisuals(ctrls))
		{
			for_each(ctrls.begin(), ctrls.end(), [&](DiControlVisual* dv)
			{
				if (nullptr == teDv)
				{
					teDv = dv;
				}
				else
				{
					Point cpt;

					cpt.X = teDv->GetLayout().GetLeft() + teDv->GetLayout().Width / 2;
					cpt.Y = teDv->GetLayout().GetTop() + teDv->GetLayout().Height / 2;

					double d1 = sqrt((double)(cpt.X - point.x) * (cpt.X - point.x) + (cpt.Y - point.y) * (cpt.Y - point.y));

					cpt.X = dv->GetLayout().GetLeft() + dv->GetLayout().Width / 2;
					cpt.Y = dv->GetLayout().GetTop() + dv->GetLayout().Height / 2;

					double d2 = sqrt((double)(cpt.X - point.x) * (cpt.X - point.x) + (cpt.Y - point.y) * (cpt.Y - point.y));

					if (d1 > d2)
					{
						teDv = dv;
					}
				}
			});
		}

		if (teDv != nullptr && teDv->GetControl() != m_editControl)
		{
			m_bMultiSel = true;

			SetEditControl(teDv->GetControl());
		}
	}

    m_lastHitArea = HitArea::NotHit;
    ChangeCursor(m_lastHitArea);
    m_lastHitPt.X = 0;
    m_lastHitPt.Y = 0;
    m_dragRect = Rect();
    m_bDragMode = false;
	m_bMultiSelDrag = false;
	m_bMultiSel = false;
    Invalidate();
    ReleaseCapture();
    CScrollView::OnLButtonUp(nFlags, point);
}


void CDiSpriteView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (VK_ESCAPE == nChar && ::IsClipboardFormatAvailable(::RegisterClipboardFormat(CF_DICTRL_FORMAT)))
	{
		::OpenClipboard(m_hWnd) && ::EmptyClipboard() && CloseClipboard();
	}
	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CDiSpriteView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
	CScrollView::OnChar(nChar, nRepCnt, nFlags);
}


void CDiSpriteView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// Move visual.
	list<DiControlVisual*> sels;

	if (m_imagePageVisual.GetSelectedVisuals(sels) > 0)
	{
		bool bUpdate = false;
		for_each(sels.begin(), sels.end(), [&](DiControlVisual* dv)
		{
			if (nullptr != dv)
			{
				Rect vl = dv->GetLayout();
				switch(nChar)
				{
				case VK_UP:
					vl.Offset(0, -1);
					break;
				case VK_DOWN:
					vl.Offset(0, 1);
					break;
				case VK_LEFT:
					vl.Offset(-1, 0);
					break;
				case VK_RIGHT:
					vl.Offset(1, 0);
					break;
				default:
					break;
				}

				CheckControlRect(vl);

				if (!vl.Equals(dv->GetLayout()))
				{
					bUpdate = true;
					dv->SetLayout(vl);

				}
			}
		});

		if (bUpdate)
		{
			theApp.GetMainFarme()->GetPropertysView()->RefreshProperty();
			Invalidate(FALSE);
		}
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CDiSpriteView::OnEditClear()
{
	// Delete control.
	if (nullptr != m_editControl && nullptr != m_editImagePage)
	{
		if (IDYES != AfxMessageBox(IDS_DELETECONTROLTIP, MB_YESNO))
		{
			return;
		}
		
		if (m_editImagePage->GetControlCount() > 0)
		{
			list<DiControlVisual*> sels;
			if (m_imagePageVisual.GetSelectedVisuals(sels) > 0)
			{
				for_each(sels.begin(), sels.end(), [&](DiControlVisual* cv)
				{
					if (cv != nullptr && cv->GetControl() != nullptr)
					{
						m_editImagePage->RemoveControl(cv->GetControl());
					}
				});
			}

			SetEditControl(nullptr);
			SetEditImagePage(m_editImagePage);
			m_imagePageVisual.SetImagePage(m_editImagePage);
			theApp.GetMainFarme()->GetFileView()->RefreshProjectFiles();
			Invalidate(FALSE);
		}

	}
	else if (nullptr != m_editImagePage)
	{
		if (IDYES != AfxMessageBox(IDS_DELETEIMAGEPAGETIP, MB_YESNO))
		{
			return;
		}
		
		DiDocument::DiProject* pro = GetDocument()->CurrentProject();

		if (pro->RemoveImagePage(m_editImagePage) >= 0)
		{
			SetEditImagePage(nullptr);
			SetEditControl(nullptr);
			m_imagePageVisual.SetImagePage(nullptr);
			theApp.GetMainFarme()->GetFileView()->RefreshProjectFiles();
			Invalidate();
		}
	}
}


void CDiSpriteView::OnUpdateEditClear(CCmdUI *pCmdUI)
{
	if (GetFocus() == this)
	{
		pCmdUI->Enable(m_editControl != nullptr);
	}
}


void CDiSpriteView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CPoint spt = GetScrollPosition();

	point.Offset(spt);
	if (m_imagePageVisual.GetLayout().Contains(Point(point.x, point.y)))
	{
		m_lastDownPt = point;
	}
	
	CScrollView::OnRButtonDown(nFlags, point);
}


void CDiSpriteView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	CSize sizeTotal;
	sizeTotal.cx = GetDocument()->CurrentProject()->ScreenSize().cx + 5;
	sizeTotal.cy = GetDocument()->CurrentProject()->ScreenSize().cy + 5;
	SetScrollSizes(MM_TEXT, sizeTotal);
	SetEditImagePage(m_editImagePage);
    theApp.GetMainFarme()->GetFileView()->RefreshProjectFiles();
    theApp.GetMainFarme()->GetPropertysView()->RefreshProperty();
}

void CDiSpriteView::OnAddControlRange(UINT uID)
{
	DiCtrlType types[] = 
	{
		DiCtrlType::DataInput, 
		DiCtrlType::Button, 
		DiCtrlType::TextInput,
		DiCtrlType::Edit,
		DiCtrlType::Text, 
		DiCtrlType::RTC_Text, DiCtrlType::QR_Code
	};
	SetCreateCtrlType(types[uID - ID_TOOL_DATA_INPUT]);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_NEWDICONTROL, 0), 0);
}

void CDiSpriteView::OnAlign(UINT uID)
{
	list<DiControlVisual*> sels;

	if (nullptr != m_editControl && m_imagePageVisual.GetSelectedVisuals(sels) >= 2)
	{
		const DiControlVisual* editCV = m_imagePageVisual.GetControlVisual(m_editControl);
		
		if (nullptr != editCV)
		{
			Rect irc = m_imagePageVisual.GetLayout();
			Rect crc = editCV->GetLayout();

			for_each(sels.begin(), sels.end(), [&](DiControlVisual* cv)
			{
				if (cv != editCV)
				{

					Rect trc = cv->GetLayout();

					switch(uID)
					{
					case ID_ALIGN_LEFT:
						trc.Offset(crc.X - trc.X, 0);
						break;
					case ID_ALIGN_RIGHT:
						trc.Offset(crc.GetRight() - trc.GetRight(), 0);
						break;
					case ID_ALIGN_TOP:
						trc.Offset(0, crc.Y - trc.Y);
						break;
					case ID_ALIGN_BOTTOM:
						trc.Offset(0, crc.GetBottom() - trc.GetBottom());
						break;
					default:
						break;
					}

					CheckControlRect(trc);

					cv->SetLayout(trc);
				}

			});
		}

		Invalidate(FALSE);
	}
}

void CDiSpriteView::OnAlignUpdate(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(nullptr != m_editControl && m_imagePageVisual.GetSelCount() >= 2);
}

void CDiSpriteView::OnEditCopy()
{
	UINT format = ::RegisterClipboardFormat(CF_DICTRL_FORMAT);
	if (::OpenClipboard(m_hWnd))
	{
		::EmptyClipboard();

		list<DiControlVisual*> sels;

		if (m_imagePageVisual.GetSelectedVisuals(sels) > 0)
		{
			int selCount = sels.size();
			HGLOBAL hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, selCount * sizeof(DiControlData));

			if (NULL != hglbCopy)
			{
				DiControlData* pDatas = (DiControlData*)::GlobalLock(hglbCopy);

				int i = 0;
				for (auto cv = sels.begin(); cv != sels.end(); ++cv, i++)
				{
					if (*cv != nullptr && (*cv)->GetControl() != nullptr)
					{
						pDatas[i] = *(DiControlData*)((*cv)->GetControl()->Data);
					}
				}

				::GlobalUnlock(hglbCopy);

				::SetClipboardData(format, hglbCopy);
			}

		}
		
		::CloseClipboard();
	}
}


void CDiSpriteView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_editControl != nullptr);
}


void CDiSpriteView::OnEditPaste()
{
	if (nullptr == m_editImagePage)
	{
		return;
	}

	UINT format = ::RegisterClipboardFormat(CF_DICTRL_FORMAT);
	if (::IsClipboardFormatAvailable(format) && ::OpenClipboard(m_hWnd))
	{
		HGLOBAL hglb = GetClipboardData(format); 
		if (hglb != NULL) 
		{ 
			DiControlData* pData = (DiControlData*)::GlobalLock(hglb);
			SIZE_T sz = ::GlobalSize(hglb);
			if (pData != NULL)
			{ 
				int ctrlCount = sz / sizeof(DiControlData);
				if (ctrlCount > 0)
				{
					for (int i = 0; i < ctrlCount; i++)
					{
						DiCtrlType type = DiCtrlType::Button;
						if (pData[i].Header.CF == 0x5A)
						{
							type = (DiCtrlType)pData[i].Header.Page[1];
						}
						else
						{
							type = (DiCtrlType)pData[i].Header.Page[15];
						}

						DiControlPri* dCtrl = DiControFactory::CreateControl(type, &pData[i]);

						switch(type)
						{
						case DiControl::Edit:
						case DiControl::RTC_Text:
						case DiControl::QR_Code:
							{
								SIZE sz = {120, 60};
								dCtrl->Size(sz);
							}
							break;
						default:
							break;

						}

						if (NULL != dCtrl)
						{
							if (1 == ctrlCount)
							{
								SIZE sz = dCtrl->Size();
								Rect rc(Point((int)m_lastDownPt.x, (int)m_lastDownPt.y), Size(sz.cx, sz.cy));

								CheckControlRect(rc);
								POINT pt = {(LONG)rc.X, (LONG)rc.Y};
								dCtrl->Location(pt);
							}
							else
							{
								SIZE sz = dCtrl->Size();
								POINT pt = dCtrl->Location();
								Rect rc(Point((int)pt.x, (int)pt.y), Size(sz.cx, sz.cy));

								rc.Offset(10, 10);

								CheckControlRect(rc);
								
								pt.x = rc.X;
								pt.y = rc.Y;

								dCtrl->Location(pt);
							}

							m_editImagePage->AddControl(dCtrl);
							dCtrl->ScreenIndex(m_editImagePage->PageIndex());
						}
					}

					m_imagePageVisual.SetImagePage(m_editImagePage);
					theApp.GetMainFarme()->GetFileView()->RefreshProjectFiles();
					Invalidate(FALSE);
				}
				GlobalUnlock(hglb); 
			} 
		} 
		CloseClipboard(); 
	}
}


void CDiSpriteView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(nullptr != m_editImagePage && (::IsClipboardFormatAvailable(::RegisterClipboardFormat(CF_DICTRL_FORMAT))));
}


void CDiSpriteView::OnDestroy()
{
	::RemoveClipboardFormatListener(m_hWnd);
	CScrollView::OnDestroy();
}
