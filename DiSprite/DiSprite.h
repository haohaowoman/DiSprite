
// DiSprite.h : DiSprite Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

// CDiSpriteApp:
// �йش����ʵ�֣������ DiSprite.cpp
//
class CDiSpriteDoc;
class CDiSpriteView;
class CMainFrame;
class CDiSpriteApp : public CWinAppEx
{
public:
	CDiSpriteApp();


private:
	ULONG_PTR m_gdiplusToken;

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;


	CDiSpriteDoc* GetDiDocument();

	CDiSpriteView* GetDiView();

	CMainFrame* GetMainFarme();

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

protected:
	afx_msg void OnAppAbout();

	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnAppExit();
};

extern CDiSpriteApp theApp;
