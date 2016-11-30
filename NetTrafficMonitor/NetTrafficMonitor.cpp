
// NetTrafficMonitor.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "NetTrafficMonitor.h"
#include "NetTrafficMonitorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNetTrafficMonitorApp

BEGIN_MESSAGE_MAP(CNetTrafficMonitorApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CNetTrafficMonitorApp 构造

CNetTrafficMonitorApp::CNetTrafficMonitorApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CNetTrafficMonitorApp 对象

CNetTrafficMonitorApp theApp;


// CNetTrafficMonitorApp 初始化

BOOL CNetTrafficMonitorApp::InitInstance()
{
	CWinApp::InitInstance();

	HWND hShellTrayWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
	HWND hReBarWnd = ::FindWindowEx(hShellTrayWnd, NULL, _T("ReBarWindow32"), NULL);
	HWND hTaskWnd = ::FindWindowEx(hReBarWnd, NULL, _T("MSTaskSwWClass"), NULL);
	HWND hCiceroUIWnd = ::FindWindowEx(hReBarWnd, NULL, _T("CiceroUIWndFrame"), NULL);

	int nWidth = 80;
	RECT rcReBar;
	::GetClientRect(hReBarWnd, &rcReBar);
	RECT rcTask;
	::GetClientRect(hTaskWnd, &rcTask);
	RECT rcCiceroUI;
	::GetClientRect(hCiceroUIWnd, &rcCiceroUI);
	if (rcTask.right + rcCiceroUI.right >= rcReBar.right - 8 &&
		rcTask.right + rcCiceroUI.right <= rcReBar.right + 8)
	{
		::MoveWindow(hTaskWnd, 0, 0, rcTask.right - nWidth, rcTask.bottom, TRUE);
	}

	m_pMainWnd = new CNetTrafficMonitorDlg(nWidth);
	m_pMainWnd->CreateEx(0, AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS),
		_T("NetTrafficMonitor"), WS_CHILDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		rcTask.right - nWidth, 0, nWidth, rcReBar.bottom, hReBarWnd, NULL);

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	//return FALSE;
	return TRUE;
}

