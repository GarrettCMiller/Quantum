// WaveProperties.cpp : implementation file
//

#include "stdafx.h"
#include "Quantum.h"
#include "WaveProperties.h"


// CWaveProperties dialog

IMPLEMENT_DYNAMIC(CWaveProperties, CDialog)

CWaveProperties::CWaveProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CWaveProperties::IDD, pParent)
	, m_fFrequency(0)
	, m_fBase(0)
	, m_fAmplitude(0)
{

}

CWaveProperties::~CWaveProperties()
{
}

void CWaveProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FREQ_EDIT, m_fFrequency);
	DDX_Text(pDX, IDC_BASE_EDIT, m_fBase);
	DDX_Text(pDX, IDC_BASE_EDIT2, m_fAmplitude);
}


BEGIN_MESSAGE_MAP(CWaveProperties, CDialog)
	ON_BN_CLICKED(IDC_APPLY, &CWaveProperties::OnBnClickedApply)
END_MESSAGE_MAP()


// CWaveProperties message handlers

void CWaveProperties::OnBnClickedApply()
{
	
}

BOOL CWaveProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
