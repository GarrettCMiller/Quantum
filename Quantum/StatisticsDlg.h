#pragma once


// CStatisticsDlg dialog

class CStatisticsDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CStatisticsDlg)

public:
	CStatisticsDlg();
	virtual ~CStatisticsDlg();

// Dialog Data
	enum { IDD = IDD_STATS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:

	bool	m_bInitialized;

	CString m_strEmitterCount;
	CString m_strNumActive;
	CString m_strNumVisible;
	CString m_strNumRenderLists;
	CString m_strProcTime;
	CString m_strMemUsage;
	CString m_strNumParticles;

	virtual BOOL OnInitDialog();
	virtual void OnTimer(UINT_PTR nIDEvent);
public:
	//afx_msg void OnPaint();
};
