#pragma once
#include "afxwin.h"


// CNewProjectSettingDlg dialog

class CNewProjectSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewProjectSettingDlg)

public:
	CNewProjectSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewProjectSettingDlg();

// Dialog Data
	enum { IDD = IDD_NEW_PROJECT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    CEdit m_name;
    CEdit m_path;
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnEnChangeEditProjectName();
    
    CString m_projectFilePath;
	SIZE m_selSize;

    afx_msg void OnBnClickedButtonBrows();
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
	CComboBox m_screenSizesCombo;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedYes();
};

extern const SIZE SelectionSizes[11];