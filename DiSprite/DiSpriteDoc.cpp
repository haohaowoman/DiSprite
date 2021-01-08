
// DiSpriteDoc.cpp : CDiSpriteDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
#include "NewProjectSettingDlg.h"
#include "ScreenSettingDlg.h"
// CDiSpriteDoc


IMPLEMENT_DYNCREATE(CDiSpriteDoc, CDocument)

BEGIN_MESSAGE_MAP(CDiSpriteDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, &CDiSpriteDoc::OnFileSave)
	ON_COMMAND(ID_NEWIMAGEPAGE, &CDiSpriteDoc::OnNewimagepage)
    ON_COMMAND(ID_FILE_CLOSE, &CDiSpriteDoc::OnFileClose)
	ON_COMMAND(ID_SCREEN_SETTING, &CDiSpriteDoc::OnScreenSetting)
	ON_COMMAND(ID_FILE_SAVE_AS, &CDiSpriteDoc::OnFileSaveAs)
END_MESSAGE_MAP()


// CDiSpriteDoc 构造/析构

CDiSpriteDoc::CDiSpriteDoc()
{
	// TODO: 在此添加一次性构造代码
	
}

CDiSpriteDoc::~CDiSpriteDoc()
{
}
//QTOE_SET
BOOL CDiSpriteDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

    CNewProjectSettingDlg npsdlg;
	INT_PTR nResult = npsdlg.DoModal();
    if (m_strPathName.IsEmpty() && IDOK == nResult)
    {
        
        m_strPathName = npsdlg.m_projectFilePath;

        CPath p(m_strPathName);
	    int index = p.FindFileName();
	    CString tempDic = m_strPathName.Mid(0, index);
	    SetCurrentDirectory(tempDic);

        m_project.NewProject(m_strPathName);
		m_project.ScreenSize(npsdlg.m_selSize);

		if (NULL != theApp.GetDiView())
		{
			theApp.GetDiView()->SetEditImagePage(nullptr);
		}
		CFile file;
		if (file.Open(m_strPathName, CFile::modeCreate|CFile::typeBinary))
		{
			SetPathName(m_strPathName);
			file.Close();
		}

		tempDic += _T("\\QTOE_SET");
		if (!::PathFileExists(tempDic))
		{
			// Create it;
			BOOL bc = ::CreateDirectory(tempDic, NULL);
			ASSERT(bc);
		}

		theApp.GetMainFarme()->GetFileView()->RefreshProjectFiles();

		theApp.GetMainFarme()->UpdateStatusProjectPath();
		CString statusStr;
		statusStr.LoadString(ID_PROJECTPATH_STATUS);
		statusStr += m_strPathName;
		theApp.GetMainFarme()->GetStatusBar()->SetPaneText(0, statusStr);
    }
	else if (IDYES == nResult)
	{
		m_strPathName = m_project.GetProjectFilePath();
		AfxGetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_FILE_OPEN, 0), 0);
	}
	
#ifdef _DEBUG
	/*DiImagePage * page = new DiImagePage();
	page->PageIndex(0);
	page->ImagePath(_T("C:\\Users\\ke.niu\\Pictures\\Colorfull ide\\1347979499119.jpg"));
	page->AddControl(new DiButton());
	page->AddControl(new DiButton());
	page->AddControl(new DiButton());
	m_project.AddImagePage(page);*/
#endif // _DEBUG
	
	return TRUE;
}




// CDiSpriteDoc 序列化

void CDiSpriteDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// Save
		
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CDiSpriteDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
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

// 搜索处理程序的支持
void CDiSpriteDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
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

// CDiSpriteDoc 诊断

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


// CDiSpriteDoc 命令


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

		theApp.GetMainFarme()->UpdateStatusProjectPath();

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
		theApp.GetDiView()->SetEditImagePage(nullptr);
		theApp.GetDiView()->SetEditControl(nullptr);
	}
	
	theApp.GetMainFarme()->UpdateStatusProjectPath();

	return TRUE;
}


void CDiSpriteDoc::OnNewimagepage()
{
	// Add a new Image page.
	CFileDialog fd(TRUE, 
		_T("bmp"),
		_T(""), 
		OFN_ALLOWMULTISELECT, 
		_T("BMP (*.bmp)|*.bmp|PNG (*.png)|*.png|JPEG (*.jpg)|*.jpg|GIF (*.gif)|*.gif|All Files (*.*)|*.*||"),
		AfxGetMainWnd()
		);

	if (IDOK == fd.DoModal())
	{
		CString pathName = fd.GetPathName();

		CPath p(m_project.GetProjectFilePath());
		int index = p.FindFileName();
		CString tempDic = m_strPathName.Mid(0, index);
		tempDic += _T("QTOE_SET\\");
		if (!::PathFileExists(tempDic))
		{
			// Create it;
			BOOL bc = ::CreateDirectory(tempDic, NULL);
			ASSERT(bc);
		}

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


void CDiSpriteDoc::OnFileClose()
{
    // Close Project file.
    if (!m_project.GetProjectFilePath().IsEmpty() || m_project.GetImagePagesCount() != 0)
    {
        UINT result = AfxMessageBox(_T("是否保存当前项目文件？"), MB_YESNOCANCEL);
        bool bdis = true;
        switch (result)
        {
        case IDYES:
            AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_FILE_SAVE, 0), 0);
            m_project.Dispose();
			
            break;
        case IDNO:
            m_project.Dispose();
            break;
        default:
            bdis = false;
            break;
        }
        if (bdis)
        {
            CDiSpriteView* pView = theApp.GetDiView();
            if (NULL != pView)
            {
				pView->SetEditImagePage(nullptr);
                pView->SetEditControl(nullptr);
            } 
			theApp.GetMainFarme()->GetFileView()->RefreshProjectFiles();
			m_strPathName.Empty();
			UpdateAllViews(NULL);
        }
    }
}


void CDiSpriteDoc::OnScreenSetting()
{
	//
	CScreenSettingDlg ssd;
	if (IDOK == ssd.DoModal())
	{
		m_project.ScreenSize(ssd.m_selSize);
		UpdateAllViews(NULL);
	}
}


void CDiSpriteDoc::OnFileSaveAs()
{
	// 
	__super::OnFileSaveAs();
	if (!GetPathName().IsEmpty() && GetPathName() != m_project.GetProjectFilePath())
	{
		CString tempStr = m_project.GetProjectFilePath();
		m_project.SetProjectFilePath(GetPathName());

		BOOL bw = m_project.SaveFile();
		ASSERT(bw);

		if (!tempStr.IsEmpty())
		{
			SetPathName(tempStr);
			m_project.SetProjectFilePath(tempStr);
		}
	}
}
