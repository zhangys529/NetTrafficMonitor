// NetTrafficMonitorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NetTrafficMonitor.h"
#include "NetTrafficMonitorDlg.h"
#include "NetTraffic.h"


// CNetTrafficMonitorDlg

IMPLEMENT_DYNAMIC(CNetTrafficMonitorDlg, CWnd)

CNetTrafficMonitorDlg::CNetTrafficMonitorDlg()
{
	m_llWidth = 70;
	m_llHeight = 30;
	m_dwUploadTraffic = 0;
	m_dwDownloadTraffic = 0;
	m_bSelfStarting = FALSE;
}

CNetTrafficMonitorDlg::~CNetTrafficMonitorDlg()
{
}


BEGIN_MESSAGE_MAP(CNetTrafficMonitorDlg, CWnd)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SELFSTARTING, OnSelfStarting)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
END_MESSAGE_MAP()



// CNetTrafficMonitorDlg 消息处理程序




BOOL CNetTrafficMonitorDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此添加专用代码和/或调用基类
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	if (GetClassInfoEx(cs.hInstance, cs.lpszClass, &wcex))
	{
		return TRUE;
	}
	wcex.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = AfxWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = cs.hInstance;
	wcex.hIcon = wcex.hIconSm = LoadIcon(cs.hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = cs.lpszClass;
	return RegisterClassEx(&wcex);

	//return CWnd::PreCreateWindow(cs);
}


int CNetTrafficMonitorDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ModifyStyle(WS_THICKFRAME, 0);									// 不可调大小
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);				// 隐藏任务栏图标
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);	// 置顶
	SetTimer(1, 1000, NULL);										// 间隔1秒刷新
	ShowWindow(SW_SHOW);

	TCHAR szFileFullPath[MAX_PATH] = { 0 };
	TCHAR szFileName[_MAX_FNAME] = { 0 };
	GetModuleFileName(NULL, szFileFullPath, MAX_PATH);							// 得到程序自身的全路径
	_wsplitpath(szFileFullPath, NULL, NULL, szFileName, NULL);					// 获得程序名
	HKEY hKey;
	LPCTSTR lpRun = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");	// 找到系统的启动项
	if (ERROR_SUCCESS == RegOpenKey(HKEY_CURRENT_USER, lpRun, &hKey))			// 打开启动项Key
	{
		DWORD dwType = REG_SZ, dwSize = MAX_PATH;
		TCHAR szValue[MAX_PATH] = { 0 };
		if (ERROR_SUCCESS == RegQueryValueEx(hKey, szFileName, NULL, &dwType, (LPBYTE)szValue, &dwSize))
		{
			m_bSelfStarting = TRUE;
		}
		else
		{
			m_bSelfStarting = FALSE;
		}
	}
	RegCloseKey(hKey);											// 关闭注册表

	return 0;
}


HBRUSH CNetTrafficMonitorDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	CFont font;
	font.CreateFont(17, 0, 0, 0, 550,
		false, false, false,
		GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_SCRIPT,
		_T("微软雅黑"));
	pDC->SelectObject(&font);

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CNetTrafficMonitorDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	CFont font;
	font.CreateFont(15, 0, 0, 0, 550,
		false, false, false,
		GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_SCRIPT,
		_T("微软雅黑"));
	dc.SelectObject(&font);
	dc.SetBkMode(TRANSPARENT);

	CString strText;
	strText.Format(_T(" ↑ %.2f KB/s"), m_dwUploadTraffic / 1024.00);
	dc.DrawText(strText, CRect(0, 0, m_llWidth, m_llHeight / 2), DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
	strText.Format(_T(" ↓ %.2f KB/s"), m_dwDownloadTraffic / 1024.00);
	dc.DrawText(strText, CRect(0, m_llHeight / 2, m_llWidth, m_llHeight), DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);

	m_cCheckNo.LoadBitmap(IDB_CHECKNO);
	m_cCheckYes.LoadBitmap(IDB_CHECKYES);
	m_cExit.LoadBitmap(IDB_EXIT);
}


void CNetTrafficMonitorDlg::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
}


void CNetTrafficMonitorDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMinTrackSize.x = lpMMI->ptMaxTrackSize.x = lpMMI->ptMaxSize.x = m_llWidth;
	lpMMI->ptMinTrackSize.y = lpMMI->ptMaxTrackSize.y = lpMMI->ptMaxSize.y = m_llHeight;

	CWnd::OnGetMinMaxInfo(lpMMI);
}


void CNetTrafficMonitorDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);

	CWnd::OnLButtonDown(nFlags, point);
}


void CNetTrafficMonitorDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	SendMessage(WM_NCLBUTTONDBLCLK, HTCAPTION, 0);

	CWnd::OnLButtonDblClk(nFlags, point);
}


void CNetTrafficMonitorDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	LPPOINT lpPoint = new tagPOINT();
	GetCursorPos(lpPoint);

	CMenu menu;
	menu.CreatePopupMenu();	// 声明一个弹出式菜单
	menu.AppendMenu(MF_STRING, IDC_SELFSTARTING, _T("开机启动"));
	menu.AppendMenu(MF_STRING, IDC_EXIT, _T("退出"));	// 增加菜单项“退出”，点击则发送消息给主窗口将程序结束
	if (m_bSelfStarting)
	{
		menu.SetMenuItemBitmaps(0, MF_BYPOSITION | MF_STRING | MF_ENABLED, &m_cCheckYes, &m_cCheckYes);
	}
	else
	{
		menu.SetMenuItemBitmaps(0, MF_BYPOSITION | MF_STRING | MF_ENABLED, &m_cCheckNo, &m_cCheckNo);
	}
	menu.SetMenuItemBitmaps(1, MF_BYPOSITION | MF_STRING | MF_ENABLED, &m_cExit, &m_cExit);
	//SetForegroundWindow();
	menu.TrackPopupMenu(TPM_LEFTALIGN, lpPoint->x, lpPoint->y, this);	// 确定弹出式菜单的位置
	menu.Detach();	// 资源回收
	menu.DestroyMenu();
	delete lpPoint;

	CWnd::OnRButtonUp(nFlags, point);
}


void CNetTrafficMonitorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CNetTraffic* pNetTraffic = CNetTraffic::create_instance();
	pNetTraffic->RefreshInterfacesTraffic();
	
	m_dwUploadTraffic = 0;
	m_dwDownloadTraffic = 0;
	int iNetworkInterfacesCount = pNetTraffic->GetNetworkInterfacesCount();
	for (int i = 0; i < iNetworkInterfacesCount; ++i)
	{
		m_dwUploadTraffic += pNetTraffic->GetIncrementalOutgoingTraffic(i);
	}
	for (int i = 0; i < iNetworkInterfacesCount; ++i)
	{
		m_dwDownloadTraffic += pNetTraffic->GetIncrementalIncomingTraffic(i);
	}

	Invalidate();

	CWnd::OnTimer(nIDEvent);
}


void CNetTrafficMonitorDlg::OnSelfStarting()
{
	TCHAR szFileFullPath[MAX_PATH] = { 0 };
	TCHAR szFileName[_MAX_FNAME] = { 0 };
	GetModuleFileName(NULL, szFileFullPath, MAX_PATH);							// 得到程序自身的全路径
	_wsplitpath(szFileFullPath, NULL, NULL, szFileName, NULL);					// 获得程序名
	HKEY hKey;
	LPCTSTR lpRun = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");	// 找到系统的启动项
	if (ERROR_SUCCESS == RegOpenKey(HKEY_CURRENT_USER, lpRun, &hKey))			// 打开启动项Key
	{
		if (m_bSelfStarting)
		{
			if (ERROR_SUCCESS == RegDeleteValue(hKey, szFileName))				// 删除一个子Key
			{
				m_bSelfStarting = FALSE;
			}
		}
		else
		{
			if (ERROR_SUCCESS == RegSetValueEx(hKey,
				szFileName,
				0,
				REG_SZ,
				(LPBYTE)szFileFullPath,
				(lstrlen(szFileFullPath) + 1) * sizeof(TCHAR)))					// 添加一个子Key,并设置值
			{
				m_bSelfStarting = TRUE;
			}
		}
	}
	RegCloseKey(hKey);															// 关闭注册表
}


void CNetTrafficMonitorDlg::OnExit()
{
	DestroyWindow();
}
