#pragma once


// CNetTrafficMonitorDlg

class CNetTrafficMonitorDlg : public CWnd
{
	DECLARE_DYNAMIC(CNetTrafficMonitorDlg)

public:
	CNetTrafficMonitorDlg();
	virtual ~CNetTrafficMonitorDlg();

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};


