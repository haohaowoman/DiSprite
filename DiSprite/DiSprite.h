
// DiSprite.h : DiSprite 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

// CDiSpriteApp:
// 有关此类的实现，请参阅 DiSprite.cpp
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

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
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
