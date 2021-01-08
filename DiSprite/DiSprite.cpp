
// DiSprite.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "DiSprite.h"
#include "MainFrm.h"

#include "DiSpriteDoc.h"
#include "DiSpriteView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDiSpriteDoc* CDiSpriteApp:: GetDiDocument()
{
	auto pMainWnd = static_cast<CMainFrame*>(AfxGetMainWnd());
	if (NULL != pMainWnd)
	{
		return static_cast<CDiSpriteDoc*>(pMainWnd->GetActiveDocument());
	}
	return NULL;
}

CDiSpriteView* CDiSpriteApp:: GetDiView()
{
	auto pMainWnd = static_cast<CMainFrame*>(AfxGetMainWnd());
	if (NULL != pMainWnd)
	{
		return static_cast<CDiSpriteView*>(pMainWnd->GetActiveView());
	}
	return NULL;
}

// CDiSpriteApp

BEGIN_MESSAGE_MAP(CDiSpriteApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CDiSpriteApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
    ON_COMMAND(ID_APP_EXIT, &CDiSpriteApp::OnAppExit)
END_MESSAGE_MAP()


// CDiSpriteApp ����

CDiSpriteApp::CDiSpriteApp()
{
	m_bHiColorIcons = TRUE;

	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("DiSprite.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CDiSpriteApp ����

CDiSpriteApp theApp;


// CDiSpriteApp ��ʼ��

BOOL CDiSpriteApp::InitInstance()
{
	

	GdiplusStartupInput gdipluseStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdipluseStartupInput, NULL);

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("DI�ؼ��༭"));

	//VERIFY( 1 == InitSkinMagicLib( AfxGetInstanceHandle(), _T("ReadUI") ,NULL,NULL ) );
	//VERIFY( 1 == LoadSkinFile(".\\Skins\\xplus.smf"));
	//VERIFY( 1 == SetDialogSkin( _T("Dialog") ) );

	LoadStdProfileSettings(15);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDiSpriteDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CDiSpriteView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ���á�DDE ִ�С�
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	//VERIFY( 1 == SetWindowSkin(m_pMainWnd->m_hWnd ,  _T("MainFrame") ) );
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	// ������/��
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

int CDiSpriteApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	//ExitSkinMagicLib();

	AfxOleTerm(FALSE);
	GdiplusShutdown(m_gdiplusToken);
	return CWinAppEx::ExitInstance();
}

// CDiSpriteApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBegindragTree1(NMHDR *pNMHDR, LRESULT *pResult);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE1, &CAboutDlg::OnBegindragTree1)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CDiSpriteApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

CMainFrame* CDiSpriteApp::GetMainFarme()
{
	return static_cast<CMainFrame*>(AfxGetMainWnd());
}

// CDiSpriteApp �Զ������/���淽��

void CDiSpriteApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CDiSpriteApp::LoadCustomState()
{
}

void CDiSpriteApp::SaveCustomState()
{
}

// CDiSpriteApp ��Ϣ�������

void CAboutDlg::OnBegindragTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void CDiSpriteApp::OnAppExit()
{
    CWinAppEx::OnAppExit();
}
