
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "PropertiesWnd.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
protected:
	CSplitterWnd m_wndSplitter;
public:
	CFileView* GetFileView() {return &m_wndFileView;}
	CPropertiesWnd* GetPropertysView() {return &m_wndProperties;}
	CMFCStatusBar* GetStatusBar() {return &m_wndStatusBar;}

	void UpdateStatusProjectPath();
// ����
public:

// ��д
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CFileView         m_wndFileView;
	CClassView        m_wndClassView;
	CPropertiesWnd    m_wndProperties;

	CMFCToolBar m_toolBar;
	CMFCToolBar m_alignToolBar;
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnAddDataInput();
	afx_msg void OnAddButton();
	afx_msg void OnAddWordsInput();
	afx_msg void OnAddEdit();
	afx_msg void OnAddText();
	afx_msg void OnAddRTCEdit();
	afx_msg void OnAddRTCCode();
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	afx_msg void OnClose();
};


