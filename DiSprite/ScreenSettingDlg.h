#pragma once
#include "afxwin.h"


// CScreenSettingDlg �Ի���

class CScreenSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScreenSettingDlg)

public:
	CScreenSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CScreenSettingDlg();

// �Ի�������
	enum { IDD = IDD_SCREEN_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_screenSizesCombo;
	SIZE m_selSize;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
