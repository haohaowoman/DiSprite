
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "DiSprite.h"

#include "DiSpriteDoc.h"
#include "DiSpriteView.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
	m_selControlItem = m_selImagePageItem = NULL;
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_NEWIMAGEPAGE, OnNewImagePage)
	ON_COMMAND(ID_NEWDICONTROL, OnNewDiControl)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar ��Ϣ�������

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ������ͼ:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_TRACKSELECT | TVS_FULLROWSELECT;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("δ�ܴ����ļ���ͼ\n");
		return -1;      // δ�ܴ���
	}

	// ������ͼͼ��:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ������*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	RefreshProjectFiles();
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::FillFileView()
{
	HTREEITEM hRoot = m_wndFileView.InsertItem(_T("FakeApp �ļ�"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hSrc = m_wndFileView.InsertItem(_T("FakeApp Դ�ļ�"), 0, 0, hRoot);

	m_wndFileView.InsertItem(_T("FakeApp.cpp"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("FakeApp.rc"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("FakeAppDoc.cpp"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("FakeAppView.cpp"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("MainFrm.cpp"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("StdAfx.cpp"), 1, 1, hSrc);

	HTREEITEM hInc = m_wndFileView.InsertItem(_T("FakeApp ͷ�ļ�"), 0, 0, hRoot);

	m_wndFileView.InsertItem(_T("FakeApp.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("FakeAppDoc.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("FakeAppView.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("Resource.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("MainFrm.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("StdAfx.h"), 2, 2, hInc);

	HTREEITEM hRes = m_wndFileView.InsertItem(_T("FakeApp ��Դ�ļ�"), 0, 0, hRoot);

	m_wndFileView.InsertItem(_T("FakeApp.ico"), 2, 2, hRes);
	m_wndFileView.InsertItem(_T("FakeApp.rc2"), 2, 2, hRes);
	m_wndFileView.InsertItem(_T("FakeAppDoc.ico"), 2, 2, hRes);
	m_wndFileView.InsertItem(_T("FakeToolbar.bmp"), 2, 2, hRes);

	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	m_wndFileView.Expand(hSrc, TVE_EXPAND);
	m_wndFileView.Expand(hInc, TVE_EXPAND);
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// ѡ���ѵ�������:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
	theApp.GetMainFarme()->ShowPane(theApp.GetMainFarme()->GetPropertysView(), TRUE, FALSE, TRUE);

}

void CFileView::OnNewImagePage()
{
	const MSG* pMSG = GetCurrentMessage();
	AfxGetMainWnd()->SendMessage(pMSG->message, pMSG->wParam, pMSG->lParam);
}

void CFileView::OnNewDiControl()
{
	const MSG* pMSG = GetCurrentMessage();
	AfxGetMainWnd()->SendMessage(pMSG->message, pMSG->wParam, pMSG->lParam);
}

void CFileView::OnDummyCompile()
{
	// TODO: �ڴ˴���������������
}

void CFileView::OnEditCut()
{
	// TODO: �ڴ˴���������������
}

void CFileView::OnEditCopy()
{
	// TODO: �ڴ˴���������������
}

void CFileView::OnEditClear()
{
	const MSG* pMsg = GetCurrentMessage();
	AfxGetMainWnd()->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ����*/);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("�޷�����λͼ: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

void CFileView::RefreshProjectFiles()
{
	if (NULL == theApp.GetDiDocument())
	{
		return;
	}

	m_selControlItem = m_selImagePageItem = NULL;

	m_wndFileView.DeleteAllItems();

	auto pro = theApp.GetDiDocument()->CurrentProject();
	
	CString tempStr = ::PathFindFileName(pro->GetProjectFilePath());
	
	if (tempStr.IsEmpty())
	{
		tempStr = _T("***����Ŀ***");
	}
	else
	{
		tempStr = tempStr.Trim(::PathFindExtension(tempStr));
	}

	HTREEITEM hRoot = m_wndFileView.InsertItem(tempStr, 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	int iCount = pro->GetImagePagesCount();

	// Insert image pages.
	for (int i = 0; i < iCount; i++)
	{
		auto page = pro->GetImagePage(i);
		HTREEITEM hPage = m_wndFileView.InsertItem(::PathFindFileName(page->ImagePath()), 0, 0, hRoot);
		m_wndFileView.SetItemData(hPage, reinterpret_cast<DWORD_PTR>(page));

		int dCount = page->GetControlCount();
		for (int j = 0; j < dCount; j++)
		{
			auto ctrl = page->GetControl(j);
			tempStr.Format(_T("%03d-%s"), j + 1, static_cast<LPCTSTR>(ctrl->Name()));
			HTREEITEM hCtrl = m_wndFileView.InsertItem(tempStr, 1, 1, hPage);
			m_wndFileView.SetItemData(hCtrl, reinterpret_cast<DWORD_PTR>(ctrl));
		}
		m_wndFileView.Expand(hPage, TVE_EXPAND);
	}
	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	
}



BOOL CFileView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CDockablePane::OnNotify(wParam, lParam, pResult);
	// Tree Operators.
	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (lParam != NULL && pNMHDR->hwndFrom == m_wndFileView.GetSafeHwnd())
	{
		LPNMTREEVIEW pNMTreeView = (LPNMTREEVIEW)lParam;

		switch (pNMHDR->code)
		{
		case TVN_SELCHANGED:
			{
				CDiSpriteView* pView = theApp.GetDiView();
				if (NULL != pView && (pNMTreeView->itemNew.mask & (TVIF_HANDLE | TVIF_TEXT)))
				{
					HTREEITEM hRoot = m_wndFileView.GetRootItem();
					if (hRoot != pNMTreeView->itemNew.hItem)
					{
						// Select a image page.
						if (m_wndFileView.GetParentItem(pNMTreeView->itemNew.hItem) == hRoot)
						{
							DiImagePage* pPage = reinterpret_cast<DiImagePage*>(m_wndFileView.GetItemData(pNMTreeView->itemNew.hItem));
							ASSERT(pPage != NULL);
							pView->SetEditImagePage(pPage);
							pView->SetEditControl(nullptr);

							m_selImagePageItem = pNMTreeView->itemNew.hItem;
							m_selControlItem = NULL;
						}
						else
						{
							// Select a control.
							DiControlPri* pCtrl = reinterpret_cast<DiControlPri*>(m_wndFileView.GetItemData(pNMTreeView->itemNew.hItem));
							ASSERT(pCtrl != NULL);
							pView->SetEditControl(pCtrl);
							
							HTREEITEM parentItem = m_wndFileView.GetParentItem(pNMTreeView->itemNew.hItem);
							ASSERT(pNMTreeView != NULL);
							DiImagePage* pPage = reinterpret_cast<DiImagePage*>(m_wndFileView.GetItemData(parentItem));
							ASSERT(pPage != NULL);
							pView->SetEditImagePage(pPage);

							m_selControlItem = pNMTreeView->itemNew.hItem;
							m_selImagePageItem = parentItem;
						}
						

					}
					
				}
			}
			break;
		default:
			break;
		}

	}
	
	return bRes;
}
