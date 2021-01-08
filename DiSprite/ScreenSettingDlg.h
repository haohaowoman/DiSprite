#pragma once
#include "afxwin.h"


// CScreenSettingDlg 对话框

class CScreenSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScreenSettingDlg)

public:
	CScreenSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScreenSettingDlg();

// 对话框数据
	enum { IDD = IDD_SCREEN_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_screenSizesCombo;
	SIZE m_selSize;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
