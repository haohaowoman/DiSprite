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
    memBmp.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
    memDc.SelectObject(&memBmp);

    Gdiplus::Graphics gp(memDc.GetSafeHdc());
	gp.Clear(Gdiplus::Color::WhiteSmoke);

    m_imagePageVisual.Render(gp);

    if (m_bDragMode)
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

	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	// Update project views.
	theApp.GetMainFarme()->GetFileView()->RefreshProjectFiles();
	m_createCtrlType = DiCtrlType::Button;
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

void CDiSpriteView::SetEditImagePage(DiImagePage* image)
{
	if (m_editImagePage != image)
	{
		m_editImagePage = image;
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
    m_imagePageVisual.SelectedControl(ctrl);
    
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
		CRect rc;
		GetClientRect(rc);
		if (rc.PtInRect(m_lastDownPt))
		{
			nCtrl->Location(m_lastDownPt);
			
			/*rc.SetRect(m_lastDownPt.x, m_lastDownPt.y, m_lastDownPt.x, m_lastDownPt.y + 50);
			nCtrl->Left(rc.left);
			nCtrl->Top(rc.top);
			nCtrl->Right(rc.right);
			nCtrl->Bottom(rc.bottom);*/
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
    // 
    CRect rc;
    GetClientRect(rc);
    if (rc.PtInRect(point))
    {
        HitArea ha = HitArea::NotHit;

        if (nullptr != m_editControl)
        {
            DiControlVisual* dv = m_imagePageVisual.GetControlVisual(m_editControl);
        
            if (nullptr != dv)
            {
				ha = dv->HitTest(Gdiplus::Point(point.x, point.y));
                if (nFlags & MK_LBUTTON)
                {
					int des = (int)sqrt((double)(point.x - m_lastHitPt.X) * (point.x - m_lastHitPt.X) + (double)(point.y - m_lastHitPt.Y) * (point.y - m_lastHitPt.Y));
					if (des >= 5)
					{
						m_bDragMode = true;

						// Drag move or size.
						m_dragRect = dv->GetLayout();
						switch (m_lastHitArea)
						{
						case HitArea::Client:
							{
								// Move. 
								m_dragRect.Offset(point.x - m_lastHitPt.X, point.y - m_lastHitPt.Y);
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

		if (!m_bDragMode)
		{
			ChangeCursor(ha);
		}
    }

    CScrollView::OnMouseMove(nFlags, point);
}


void CDiSpriteView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    bool reSel = true;

    HitArea ha = HitArea::NotHit;

    if (nullptr != m_editControl)
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
                    // Select the control.
                    SetEditControl(dv->GetControl());
                    break;
                }
            } 
        }
    }

	if (ha == HitArea::NotHit || ha == HitArea::Tranprant)
	{
		SetEditControl(nullptr);
	}

    m_lastHitPt.X = point.x;
    m_lastHitPt.Y = point.y;
    m_lastHitArea = ha;
    this->SetCapture();
	SetFocus();
    ChangeCursor(ha);
	Invalidate(FALSE);
	m_lastDownPt = point;
    CScrollView::OnLButtonDown(nFlags, point);
}


void CDiSpriteView::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (m_bDragMode)
    {
        CRect rc;
        GetClientRect(rc);

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
    m_lastHitArea = HitArea::NotHit;
    ChangeCursor(m_lastHitArea);
    m_lastHitPt.X = 0;
    m_lastHitPt.Y = 0;
    m_dragRect = Rect();
    m_bDragMode = false;
    Invalidate();
    ReleaseCapture();
    CScrollView::OnLButtonUp(nFlags, point);
}


void CDiSpriteView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CDiSpriteView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
	CScrollView::OnChar(nChar, nRepCnt, nFlags);
}


void CDiSpriteView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// Move visual.
	if (nullptr != m_editControl)
	{
		DiControlVisual* dv = m_imagePageVisual.GetControlVisual(m_editControl);
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

			vl.X = max(0, vl.X);
			vl.Y = max(0, vl.Y);

			if (!vl.Equals(dv->GetLayout()))
			{
				dv->SetLayout(vl);
				theApp.GetMainFarme()->GetPropertysView()->RefreshProperty();
				Invalidate(FALSE);
			}
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
		
		if (m_editImagePage->RemoveControl(m_editControl) >= 0)
		{
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
	m_lastDownPt = point;
	CScrollView::OnRButtonDown(nFlags, point);
}
