#pragma once


// CNetTrafficMonitorDlg

class CNetTrafficMonitorDlg : public CWnd
{
	DECLARE_DYNAMIC(CNetTrafficMonitorDlg)

public:
	CNetTrafficMonitorDlg();
	CNetTrafficMonitorDlg(int nWidth);
	virtual ~CNetTrafficMonitorDlg();

public:
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg void OnMove(int x, int y);
//	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSelfStarting();
//	afx_msg void OnTopMost();
//	afx_msg void OnTransparency();
	afx_msg void OnExit();

private:
	int m_nWidth;

	BOOL m_bSelfStarting;

//	CSliderCtrl* m_pSlider;
//	CToolTipCtrl m_cToolTip;
	CStatic* m_pStatic1;
	CStatic* m_pStatic2;
	CStatic* m_pUploadStatic;
	CStatic* m_pDownloadStatic;
	CFont* m_pFont;

	CBitmap m_cCheckNo;
	CBitmap m_cCheckYes;
	CBitmap m_cExit;
};


