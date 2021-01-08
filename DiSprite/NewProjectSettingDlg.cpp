// NewProjectSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DiSprite.h"
#include "NewProjectSettingDlg.h"
#include "afxdialogex.h"


const SIZE SelectionSizes[] = 
{
	{320, 240},
	{480, 272},
	{272, 480},
	{640, 480},
	{800, 480},
	{800, 600},
	{1024, 600},
	{1024, 768},
	{480, 640},
	{480, 800},
	{600, 1024},
};

// CNewProjectSettingDlg dialog

IMPLEMENT_DYNAMIC(CNewProjectSettingDlg, CDialogEx)

    CNewProjectSettingDlg::CNewProjectSettingDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CNewProjectSettingDlg::IDD, pParent)
{
	m_selSize = SelectionSizes[2];
}

CNewProjectSettingDlg::~CNewProjectSettingDlg()
{
}

void CNewProjectSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PROJECT_NAME, m_name);
	DDX_Control(pDX, IDC_EDIT_PROJECT_PATH, m_path);
	DDX_Control(pDX, IDC_COMBO1, m_screenSizesCombo);
}


BEGIN_MESSAGE_MAP(CNewProjectSettingDlg, CDialogEx)
    ON_EN_CHANGE(IDC_EDIT_PROJECT_NAME, &CNewProjectSettingDlg::OnEnChangeEditProjectName)
    ON_EN_CHANGE(IDC_EDIT_PROJECT_PATH, &CNewProjectSettingDlg::OnEnChangeEditProjectName)
    ON_BN_CLICKED(IDC_BUTTON_BROWS, &CNewProjectSettingDlg::OnBnClickedButtonBrows)
    ON_BN_CLICKED(IDOK, &CNewProjectSettingDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CNewProjectSettingDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDYES, &CNewProjectSettingDlg::OnBnClickedYes)
END_MESSAGE_MAP()


// CNewProjectSettingDlg message handlers

void CNewProjectSettingDlg::OnEnChangeEditProjectName()
{
    CString strName;
    m_name.GetWindowText(strName);
    if (strName.IsEmpty())
    {
        GetDlgItem(IDOK)->EnableWindow(FALSE);
        return;
    }

    //strName += _T(".disln");
    CString strPath;
    m_path.GetWindowText(strPath);
    strPath += _T("\\");
    strPath += strName;

    if (!::PathFileExists(strPath))
    {
        GetDlgItem(IDOK)->EnableWindow(TRUE);
        m_projectFilePath = strPath;
    }
    else
    {
        GetDlgItem(IDOK)->EnableWindow(FALSE);
    }
}

int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData) 
{ 
	switch(uMsg) 
	{ 
	case BFFM_INITIALIZED: 
		{
			TCHAR curFloder[MAX_PATH] = {0};

			if (0 < GetCurrentDirectory(MAX_PATH, curFloder))
			{
				::SendMessage(hwnd,BFFM_SETSELECTION,TRUE, (LPARAM)curFloder); 
			}
		}
		break; 
	default:
		break;
	} 

	return  0; 
} 

void CNewProjectSettingDlg::OnBnClickedButtonBrows()
{
    BROWSEINFO      sInfo;  
    ::ZeroMemory(&sInfo, sizeof(BROWSEINFO)); 
	sInfo.hwndOwner = m_hWnd;
	TCHAR curFloder[MAX_PATH] = {0};

	if (0 < GetCurrentDirectory(MAX_PATH, curFloder))
	{
		sInfo.pszDisplayName = curFloder;
	}
	sInfo.lpfn = BrowseCallbackProc;
    sInfo.pidlRoot   = 0;  
    sInfo.lpszTitle   = _T("请选择项目存储路径");  
    sInfo.ulFlags   = BIF_RETURNONLYFSDIRS|BIF_EDITBOX|BIF_DONTGOBELOWDOMAIN;  
    TCHAR szFolderPath[MAX_PATH] = {0}; 
    CString strFolderPath = TEXT("");

    // 显示文件夹选择对话框  

    LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
    
    if (lpidlBrowse != NULL)  
    {  
        // 取得文件夹名  
        if (::SHGetPathFromIDList(lpidlBrowse,szFolderPath))    
        {  
            strFolderPath = szFolderPath;
            m_path.SetWindowText(strFolderPath);
        }
        ::CoTaskMemFree(lpidlBrowse);
    }

}


BOOL CNewProjectSettingDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    TCHAR tempStr[MAX_PATH] = {0};
    GetCurrentDirectory(MAX_PATH, tempStr);
    CString str = tempStr;
    if (str.IsEmpty())
    {
        ZeroMemory(tempStr, sizeof(TCHAR) * MAX_PATH);
        GetModuleFileName(NULL, tempStr, MAX_PATH);
        str = tempStr;
        int l = str.Find(theApp.m_pszAppName, 0);
        str = str.Left(l);
    }
    m_path.SetWindowText(str);
    m_projectFilePath = _T("");
    GetDlgItem(IDOK)->EnableWindow(FALSE);

	int count  = sizeof(SelectionSizes) / sizeof(SIZE);
	for (int i = 0; i < count; i++)
	{
		CString sStr;
		sStr.Format(_T("%d×%d"), SelectionSizes[i].cx, SelectionSizes[i].cy);
		m_screenSizesCombo.AddString(sStr);
	}

	m_screenSizesCombo.SetCurSel(2);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void CNewProjectSettingDlg::OnBnClickedOk()
{
    //
	m_projectFilePath += _T("\\");
	BOOL bc = ::CreateDirectory(m_projectFilePath, NULL);
	if (!bc)
	{
		AfxMessageBox(_T("输入项目名称无效，请重新输入！"));
		return;
	}
	
	CString strName;
	m_name.GetWindowText(strName);
	strName += _T(".disln");
	m_projectFilePath += strName;
	m_selSize = SelectionSizes[m_screenSizesCombo.GetCurSel()];
    EndDialog(IDOK);
    //CDialogEx::OnOK();
}


void CNewProjectSettingDlg::OnCbnSelchangeCombo1()
{
	//
}


void CNewProjectSettingDlg::OnBnClickedYes()
{
	EndDialog(IDYES);
}
