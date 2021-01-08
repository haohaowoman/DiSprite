// ScreenSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DiSprite.h"
#include "ScreenSettingDlg.h"
#include "afxdialogex.h"
#include "NewProjectSettingDlg.h"
#include "DiSpriteDoc.h"
// CScreenSettingDlg 对话框

IMPLEMENT_DYNAMIC(CScreenSettingDlg, CDialogEx)

CScreenSettingDlg::CScreenSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScreenSettingDlg::IDD, pParent)
{

}

CScreenSettingDlg::~CScreenSettingDlg()
{
}

void CScreenSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_screenSizesCombo);
}


BEGIN_MESSAGE_MAP(CScreenSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CScreenSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CScreenSettingDlg 消息处理程序


BOOL CScreenSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CDiSpriteDoc* pDoc = theApp.GetDiDocument();
	if (NULL != pDoc)
	{
		m_selSize = pDoc->CurrentProject()->ScreenSize();
	}
	int sel = 2;
	int count  = sizeof(SelectionSizes) / sizeof(SIZE);
	for (int i = 0; i < count; i++)
	{
		CString sStr;
		sStr.Format(_T("%d×%d"), SelectionSizes[i].cx, SelectionSizes[i].cy);
		m_screenSizesCombo.AddString(sStr);
		if (SelectionSizes[i].cx == m_selSize.cx || SelectionSizes[i].cy == m_selSize.cy)
		{
			sel = i;
		}
	}

	m_screenSizesCombo.SetCurSel(sel);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CScreenSettingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_selSize = SelectionSizes[m_screenSizesCombo.GetCurSel()];
	CDialogEx::OnOK();
}
