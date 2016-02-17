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
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSelfStarting();
	afx_msg void OnTopMost();
	afx_msg void OnTransparency();
	afx_msg void OnExit();

private:
	int m_iWidth;
	int m_iHeight;
	int m_iTransparency;

	DWORD m_dwUploadTraffic;
	DWORD m_dwDownloadTraffic;

	BOOL m_bSelfStarting;
	BOOL m_bTopMost;

	CSliderCtrl* m_pSlider;
	CToolTipCtrl m_cToolTip;

	CBitmap m_cCheckNo;
	CBitmap m_cCheckYes;
	CBitmap m_cExit;
};


