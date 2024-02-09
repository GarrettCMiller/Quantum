// PhysProps.cpp : implementation file
//

#include "stdafx.h"
#include "Quantum.h"
#include "PhysProps.h"


// CPhysPropsDlg dialog

IMPLEMENT_DYNAMIC(CPhysPropsDlg, CPropertyPage)

CPhysPropsDlg::CPhysPropsDlg()
	: CPropertyPage(CPhysPropsDlg::IDD)
	, m_bCollisions(FALSE)
	, m_bRandomizeMass(false)
	, m_bRandomizeCharge(false)
	, m_bApplyGravity(FALSE)

{

}

CPhysPropsDlg::~CPhysPropsDlg()
{
}

void CPhysPropsDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_COLLISIONS, m_bCollisions);

	DDX_Check(pDX, IDC_RANDOMIZE_MASS, m_bRandomizeMass);
	DDX_Check(pDX, IDC_RANDOM_CHARGE, m_bRandomizeCharge);

	DDX_Check(pDX, IDC_APPLY_GRAVITY, m_bApplyGravity);
	DDX_Check(pDX, IDC_APPLY_EMFORCE, m_bApplyEMForce);
}


BEGIN_MESSAGE_MAP(CPhysPropsDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_COLLISIONS, &CPhysPropsDlg::OnBnClickedCollisions)
	ON_BN_CLICKED(IDC_RANDOMIZE_MASS, &CPhysPropsDlg::OnBnClickedRandomizeMass)
	ON_BN_CLICKED(IDC_RANDOM_CHARGE, &CPhysPropsDlg::OnBnClickedRandomizeCharge)
	ON_BN_CLICKED(IDC_CLEAR_HISTORY, &CPhysPropsDlg::OnBnClickedClearHistory)
	ON_BN_CLICKED(IDC_APPLY_GRAVITY, &CPhysPropsDlg::OnBnClickedApplyGravity)
	ON_BN_CLICKED(IDC_APPLY_EMFORCE, &CPhysPropsDlg::OnBnClickedApplyEMForce)
END_MESSAGE_MAP()

BOOL CPhysPropsDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// CPhysPropsDlg message handlers
void CPhysPropsDlg::UpdateAllControls()
{
	m_bRandomizeMass = g_pCurrentEmitter->m_bRandomizeMass;
	m_bRandomizeCharge = g_pCurrentEmitter->m_bRandomizeCharge;

	m_bCollisions = g_pCurrentEmitter->m_bApplyCollisions;

	m_bApplyGravity = g_pCurrentEmitter->m_bApplyGravity;
	m_bApplyEMForce = g_pCurrentEmitter->m_bApplyEMForce;

	UpdateData(false);
}

void CPhysPropsDlg::OnBnClickedCollisions()
{
	UpdateData();
	g_Output << "Collisions Enabled: " << (m_bCollisions ? "true" : "false") << "\n";
	THREAD_SAFE(g_pCurrentEmitter->EnableCollisions(m_bCollisions));
}

void CPhysPropsDlg::OnBnClickedClearHistory()
{
	g_Output << "Clearing particle history...\n";

	THREAD_SAFE(g_pCurrentEmitter->ClearParticleHistory());
}

void CPhysPropsDlg::OnBnClickedRandomizeMass()
{
	UpdateData();
	g_Output << "Randomize Mass: " << (m_bRandomizeMass ? "true" : "false") << "\n";
	THREAD_SAFE(g_pCurrentEmitter->m_bRandomizeMass = m_bRandomizeMass);
}

void CPhysPropsDlg::OnBnClickedRandomizeCharge()
{
	UpdateData();
	g_Output << "Randomize Charge: " << (m_bRandomizeCharge ? "true" : "false") << "\n";
	THREAD_SAFE(g_pCurrentEmitter->m_bRandomizeCharge = m_bRandomizeCharge);
}

void CPhysPropsDlg::OnBnClickedApplyGravity()
{
	UpdateData();
	g_Output << "Gravity Enabled: " << (m_bApplyGravity ? "true" : "false") << "\n";
	THREAD_SAFE(g_pCurrentEmitter->m_bApplyGravity = m_bApplyGravity);
}

void CPhysPropsDlg::OnBnClickedApplyEMForce()
{
	UpdateData();
	g_Output << "Electromagnetic Force Enabled: " << (m_bApplyEMForce ? "true" : "false") << "\n";
	THREAD_SAFE(g_pCurrentEmitter->m_bApplyEMForce = m_bApplyEMForce);
}

