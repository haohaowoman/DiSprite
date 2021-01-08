
// DiSpriteView.h : CDiSpriteView 类的接口
//

#pragma once
#include "DiImagePage.h"
#include "DiControl.h"
#include "DiControlVisual.h"
#include "DiAssist.h"

class CDiSpriteView : public CScrollView
{
private:
    DiImagePage* m_editImagePage;
    DiControlPri* m_editControl;
    DiImagePageVisual m_imagePageVisual;
    HitArea m_lastHitArea;
    Point m_lastHitPt;
    Rect m_dragRect;
    bool m_bDragMode;
	CPoint m_lastDownPt;
	DiCtrlType m_createCtrlType;
	bool m_bMultiSel;
	bool m_bMultiSelDrag;
protected: // 仅从序列化创建
    CDiSpriteView();
    DECLARE_DYNCREATE(CDiSpriteView)

    // 特性
public:
    CDiSpriteDoc* GetDocument() const;

    void SetEditImagePage(DiImagePage* image);
    DiImagePage* GetEditImagePage() const;

    void SetEditControl(DiControlPri* ctrl);
    DiControlPri* GetEditControl() const;

    DiImagePageVisual* GetImageView() {return &m_imagePageVisual;}
    void UpdateEditControVisual();
    
	DiCtrlType GetCreateCtrlType() const{return m_createCtrlType;}
	void SetCreateCtrlType(DiCtrlType type){ m_createCtrlType = type;}
    // 操作
public:

    // 重写
public:
    virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
    virtual void OnInitialUpdate(); // 构造后第一次调用
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

    // 实现
public:
    virtual ~CDiSpriteView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;

#endif

	void CheckControlRect(Rect &vl);
protected:

    // 生成的消息映射函数
protected:
    afx_msg void OnFilePrintPreview();
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnNewdicontrol();
    afx_msg void OnUpdateNewdicontrol(CCmdUI *pCmdUI);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    void ChangeCursor(HitArea ha) const;
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditClear(CCmdUI *pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	afx_msg void OnAddControlRange(UINT uID);
	afx_msg void OnAlign(UINT uID);
	afx_msg void OnAlignUpdate(CCmdUI *pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // DiSpriteView.cpp 中的调试版本
inline CDiSpriteDoc* CDiSpriteView::GetDocument() const
{ return reinterpret_cast<CDiSpriteDoc*>(m_pDocument); }
#endif

