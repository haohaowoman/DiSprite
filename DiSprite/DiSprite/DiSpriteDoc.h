
// DiSpriteDoc.h : CDiSpriteDoc ��Ľӿ�
//


#pragma once

#include "DiProject.h"

using namespace DiDocument;

class CDiSpriteDoc : public CDocument
{
protected: // �������л�����
	CDiSpriteDoc();
	DECLARE_DYNCREATE(CDiSpriteDoc)

private:
	DiProject m_project;

// ����
public:

	DiProject* CurrentProject() { return &m_project;}

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CDiSpriteDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileSave();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnNewimagepage();
};
