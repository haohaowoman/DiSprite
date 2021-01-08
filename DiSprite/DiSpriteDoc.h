
// DiSpriteDoc.h : CDiSpriteDoc 类的接口
//


#pragma once

#include "DiProject.h"

using namespace DiDocument;

class CDiSpriteDoc : public CDocument
{
protected: // 仅从序列化创建
	CDiSpriteDoc();
	DECLARE_DYNCREATE(CDiSpriteDoc)

private:
	DiProject m_project;

// 特性
public:

	DiProject* CurrentProject() { return &m_project;}

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CDiSpriteDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileSave();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnNewimagepage();
    afx_msg void OnFileClose();
	afx_msg void OnScreenSetting();
	afx_msg void OnFileSaveAs();
};
