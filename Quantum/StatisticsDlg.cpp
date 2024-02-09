// StatisticsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Quantum.h"
#include "StatisticsDlg.h"


// CStatisticsDlg dialog
Timer temp;

IMPLEMENT_DYNAMIC(CStatisticsDlg, CPropertyPage)

CStatisticsDlg::CStatisticsDlg()
	: CPropertyPage(CStatisticsDlg::IDD)
	, m_strEmitterCount(_T("1"))
	, m_strNumActive(_T("1"))
	, m_strNumVisible(_T("1"))
	, m_strNumRenderLists(_T("1"))
	, m_strProcTime(_T("00:00:00"))
	, m_strMemUsage(_T("0 KB"))
	, m_strNumParticles(_T("1000"))
	, m_bInitialized(false)
{

}

CStatisticsDlg::~CStatisticsDlg()
{
}

void CStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EMITTER_COUNT,	m_strEmitterCount);
	DDX_Text(pDX, IDC_NUM_ACTIVE,		m_strNumActive);
	DDX_Text(pDX, IDC_NUM_VISIBLE,		m_strNumVisible);
	DDX_Text(pDX, IDC_NUM_RENDERLISTS,	m_strNumRenderLists);
	DDX_Text(pDX, IDC_PROC_TIME,		m_strProcTime);
	DDX_Text(pDX, IDC_MEM_USAGE,		m_strMemUsage);
	DDX_Text(pDX, IDC_NUM_PARTICLES,	m_strNumParticles);
}


BEGIN_MESSAGE_MAP(CStatisticsDlg, CPropertyPage)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CStatisticsDlg message handlers

 void CStatisticsDlg::OnTimer(UINT_PTR nIDEvent)
 {
	 //static float time;

	 //time = temp.Get();

	 //if (time > 0.0f && m_bInitialized)
	 //{
		// m_strProcTime.Format("%.2d:%.2d:%.2d", static_cast<int>(time) / 60, static_cast<int>(time) % 60, static_cast<int>(time) * 60 % 100);
		// //temp.Reset();
		// //UpdateData(false);
	 //}
 
 	CPropertyPage::OnTimer(nIDEvent);
 }

LRESULT CStatisticsDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
// 	LRESULT ret = CPropertyPage::WindowProc(message, wParam, lParam);
// 
// 	static float time = 0.0f;
// 	static UINT frameCount = 0;
// 
// 	time = temp.Get();
// 
// 	if (time > 0.0f && m_bInitialized)
// 	{
// 		frameCount++;
// 
// 		if (frameCount > 10)
// 		{
// 			m_strProcTime.Format("%.2d:%.2d:%.2d", static_cast<int>(time) / 60, static_cast<int>(time) % 60, static_cast<int>(time) * 60 % 100);
// 			GetDlgItem(IDC_PROC_TIME)->SetWindowText(m_strProcTime);
// 			Invalidate();
// 			frameCount = 0;
// 		}
// 	}
// 
// 	return ret;

	return CPropertyPage::WindowProc(message, wParam, lParam);
}

BOOL CStatisticsDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	temp.Reset();
	
	//SetTimer(1, 1000, NULL);

	m_bInitialized = true;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
