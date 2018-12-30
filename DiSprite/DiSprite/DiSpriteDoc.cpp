
// DiSpriteDoc.cpp : CDiSpriteDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "DiSprite.h"
#endif

#include "DiSpriteDoc.h"
#include <atlpath.h>
#include <propkey.h>

#include "DiControlData.h"
#include "DiButton.h"
using namespace DiControl;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "DiControlFactory.h"
#include "DiSpriteView.h"
#include "MainFrm.h"
// CDiSpriteDoc


IMPLEMENT_DYNCREATE(CDiSpriteDoc, CDocument)

BEGIN_MESSAGE_MAP(CDiSpriteDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, &CDiSpriteDoc::OnFileSave)
	ON_COMMAND(ID_NEWIMAGEPAGE, &CDiSpriteDoc::OnNewimagepage)
END_MESSAGE_MAP()


// CDiSpriteDoc ����/����

CDiSpriteDoc::CDiSpriteDoc()
{
	// TODO: �ڴ����һ���Թ������
	
}

CDiSpriteDoc::~CDiSpriteDoc()
{
}

BOOL CDiSpriteDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	CPath p(m_strPathName);
	int index = p.FindFileName();
	CString tempDic = m_strPathName.Mid(0, index);
	SetCurrentDirectory(tempDic);

	m_project.Dispose();

#ifdef _DEBUG
	DiImagePage * page = new DiImagePage();
	page->PageIndex(0);
	page->ImagePath(_T("C:\\Users\\ke.niu\\Pictures\\Colorfull ide\\1347979499119.jpg"));
	page->AddControl(new DiButton());
	page->AddControl(new DiButton());
	page->AddControl(new DiButton());
	m_project.AddImagePage(page);
#endif // _DEBUG
	
	if (NULL != theApp.GetDiView())
	{
		theApp.GetDiView()->SetEditControl(nullptr);
		theApp.GetDiView()->SetEditImagePage(nullptr);
	}

	theApp.GetMainFarme()->GetFileView()->RefreshProjectFiles();
	return TRUE;
}




// CDiSpriteDoc ���л�

void CDiSpriteDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// Save
		
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CDiSpriteDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CDiSpriteDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CDiSpriteDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CDiSpriteDoc ���

#ifdef _DEBUG
void CDiSpriteDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDiSpriteDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDiSpriteDoc ����


void CDiSpriteDoc::OnFileSave()
{
	CDocument::OnFileSave();
	CPath p(m_strPathName);
	int index = p.FindFileName();
	CString tempDic = m_strPathName.Mid(0, index);
	SetCurrentDirectory(tempDic);
	if (!(m_strPathName.IsEmpty()))
	{
		m_project.SetProjectFilePath(m_strPathName);
		BOOL bw = m_project.SaveFile();
		ASSERT(bw);
		theApp.GetMainFarme()->GetFileView()->RefreshProjectFiles();
	}
}


BOOL CDiSpriteDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	CPath p(lpszPathName);
	int index = p.FindFileName();
	CString tempDic = CString(lpszPathName).Mid(0, index);
	SetCurrentDirectory(tempDic);
	//Open project.
	m_project.Reset(lpszPathName);

	theApp.GetMainFarme()->GetFileView()->RefreshProjectFiles();
	if (NULL != theApp.GetDiView())
	{
		theApp.GetDiView()->SetEditControl(nullptr);
		theApp.GetDiView()->SetEditImagePage(nullptr);
	}
	return TRUE;
}


void CDiSpriteDoc::OnNewimagepage()
{
	// Add a new Image page.
	CFileDialog fd(TRUE, 
		_T("bmp"),
		_T(""), 
		OFN_ALLOWMULTISELECT, 
		_T("PNG (*.png)|*.png|JPEG (*.jpg)|*.jpg|GIF (*.gif)|*.gif|BMP (*.bmp)|*.bmp|All Files (*.*)|*.*||"),
		AfxGetMainWnd()
		);

	if (IDOK == fd.DoModal())
	{
		CString pathName = fd.GetPathName();
		DiImagePage* pImage = new DiImagePage();
		pImage->ImagePath(pathName);
		m_project.AddImagePage(pImage);
		theApp.GetMainFarme()->GetFileView()->RefreshProjectFiles();
		if (NULL != theApp.GetDiView())
		{
			theApp.GetDiView()->SetEditControl(nullptr);
			theApp.GetDiView()->SetEditImagePage(nullptr);
		}
        UpdateAllViews(theApp.GetDiView());
	}
}
