// MovementDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Quantum.h"
#include "MovementDlg.h"

#include "MainFrm.h"

// CMovementDlg dialog

IMPLEMENT_DYNAMIC(CMovementDlg, DialogParentClass)

CMovementDlg::CMovementDlg(/*CWnd* pParent*/ /*=NULL*/)
	: DialogParentClass(CMovementDlg::IDD/*, pParent*/)
	, m_fVelocityX(0)
	, m_fVelocityY(0)
	, m_fVelocityZ(0)
	, m_fPolarPickupRange(0)
	, m_fVelocityR(0)
	, m_fVelocityA(0)
	, m_fVelocityPhi(0)
	, m_fAccelX(0)
	, m_fAccelY(0)
	, m_fAccelR(0)
	, m_fAccelA(0)
	, m_bRadialVelocityWave(FALSE)
	, m_bThetaVelocityWave(FALSE)
	, m_bApplyWind(FALSE)
	, m_bPhiVelocityWave(FALSE)
	, m_fAccelZ(0)
	, m_fAccelPhi(0)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CMovementDlg::~CMovementDlg()
{
}

// CMovementDlg message handlers

BOOL CMovementDlg::OnInitDialog()
{
	DialogParentClass::OnInitDialog();

	static const float Mag = 50.0f;
	
	m_sVelocityX.SetFloatRange(-Mag, Mag);
	m_sVelocityY.SetFloatRange(-Mag, Mag);
	m_sVelocityZ.SetFloatRange(-Mag, Mag);
	
	m_sVelocityR.SetFloatRange(-Mag, Mag);
	m_sVelocityA.SetFloatRange(-Mag, Mag);
	m_sVelocityPhi.SetFloatRange(-Mag, Mag);

	m_sVelocityX.SetPageSize(g_fFloatPrecision * 0.25f);
	m_sVelocityY.SetPageSize(g_fFloatPrecision * 0.25f);
	m_sVelocityZ.SetPageSize(g_fFloatPrecision * 0.25f);
	
	m_sVelocityR.SetPageSize(g_fFloatPrecision * 0.25f);
	m_sVelocityA.SetPageSize(g_fFloatPrecision * 0.25f);
	m_sVelocityPhi.SetPageSize(g_fFloatPrecision * 0.25f);

	m_sPolarPickupRange.SetFloatRange(0.0f, 1000.0f);
	//m_sPolarPickupRange.SetPageSize(g_fFloatPrecision * 10);

	m_sAccelX.SetFloatRange(-Mag, Mag);
	m_sAccelY.SetFloatRange(-Mag, Mag);
	m_sAccelZ.SetFloatRange(-Mag, Mag);

	m_sAccelR.SetFloatRange(-Mag, Mag);
	m_sAccelA.SetFloatRange(-Mag, Mag);
	m_sAccelPhi.SetFloatRange(-Mag, Mag);

	m_sAccelX.SetPageSize(g_fFloatPrecision * 0.25f);
	m_sAccelY.SetPageSize(g_fFloatPrecision * 0.25f);
	m_sAccelZ.SetPageSize(g_fFloatPrecision * 0.25f);

	m_sAccelR.SetPageSize(g_fFloatPrecision * 0.25f);
	m_sAccelA.SetPageSize(g_fFloatPrecision * 0.25f);
	m_sAccelPhi.SetPageSize(g_fFloatPrecision * 0.25f);

	g_Output << "Initializing movement dialog...\n";

	UpdateAllControls();

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMovementDlg::UpdateAllControls()
{
	m_fVelocityX = g_pCurrentEmitter->m_Base.m_vVel.x;
	m_fVelocityY = g_pCurrentEmitter->m_Base.m_vVel.y;
	m_fVelocityZ = g_pCurrentEmitter->m_Base.m_vVel.z;

	m_fPolarPickupRange = g_pCurrentEmitter->PickupRange;

	m_fVelocityR = g_pCurrentEmitter->m_Base.m_vPolarVel.x;
	m_fVelocityA = g_pCurrentEmitter->m_Base.m_vPolarVel.y;
	m_fVelocityPhi = g_pCurrentEmitter->m_Base.m_vPolarVel.z;

	m_bRadialVelocityWave = g_pCurrentEmitter->m_bRadialVelocityWave;
	m_bThetaVelocityWave = g_pCurrentEmitter->m_bThetaVelocityWave;

	m_fAccelX = g_pCurrentEmitter->m_Base.m_vAccel.x;
	m_fAccelY = g_pCurrentEmitter->m_Base.m_vAccel.y;
	m_fAccelZ = g_pCurrentEmitter->m_Base.m_vAccel.z;

	m_fAccelR = g_pCurrentEmitter->m_Base.m_vPolarAccel.x;
	m_fAccelA = g_pCurrentEmitter->m_Base.m_vPolarAccel.y;
	m_fAccelPhi = g_pCurrentEmitter->m_Base.m_vPolarAccel.z;

	m_sVelocityX.SetFloatPos(m_fVelocityX);
	m_sVelocityY.SetFloatPos(m_fVelocityY);
	m_sVelocityZ.SetFloatPos(m_fVelocityZ);
	
	m_sVelocityR.SetFloatPos(m_fVelocityR);
	m_sVelocityA.SetFloatPos(m_fVelocityA);
	m_sVelocityPhi.SetFloatPos(m_fVelocityPhi);

	m_sPolarPickupRange.SetFloatPos(m_fPolarPickupRange);

	m_sAccelX.SetFloatPos(m_fAccelX);
	m_sAccelY.SetFloatPos(m_fAccelY);
	m_sAccelZ.SetFloatPos(m_fAccelZ);
	
	m_sAccelR.SetFloatPos(m_fAccelR);
	m_sAccelA.SetFloatPos(m_fAccelA);
	m_sAccelPhi.SetFloatPos(m_fAccelPhi);

	m_bApplyWind = g_pCurrentEmitter->m_bApplyWind;
}

void CMovementDlg::DoDataExchange(CDataExchange* pDX)
{
	DialogParentClass::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_VEL_POLAR_PICKUP_RANGE_SLIDER, m_sPolarPickupRange);

	DDX_Control(pDX, IDC_VEL_X_SLIDER, m_sVelocityX);
	DDX_Control(pDX, IDC_VEL_Y_SLIDER, m_sVelocityY);
	DDX_Control(pDX, IDC_VEL_Z_SLIDER, m_sVelocityZ);

	DDX_Control(pDX, IDC_VEL_RAD_SLIDER, m_sVelocityR);
	DDX_Control(pDX, IDC_VEL_ANG_SLIDER, m_sVelocityA);
	DDX_Control(pDX, IDC_VEL_PHI_SLIDER, m_sVelocityPhi);

	DDX_Control(pDX, IDC_ACC_X_SLIDER, m_sAccelX);
	DDX_Control(pDX, IDC_ACC_Y_SLIDER, m_sAccelY);
	DDX_Control(pDX, IDC_ACC_RAD_SLIDER, m_sAccelR);
	DDX_Control(pDX, IDC_ACC_ANG_SLIDER, m_sAccelA);

	DDX_Text(pDX,	IDC_VEL_POLAR_PICKUP_RANGE_EDIT, m_fPolarPickupRange);

	DDX_Text(pDX,	IDC_VEL_X_EDIT,		m_fVelocityX);
	DDX_Text(pDX,	IDC_VEL_Y_EDIT,		m_fVelocityY);
	DDX_Text(pDX,	IDC_VEL_Z_EDIT,		m_fVelocityZ);

	DDX_Text(pDX,	IDC_VEL_RAD_EDIT,	m_fVelocityR);
	DDX_Text(pDX,	IDC_VEL_ANG_EDIT,	m_fVelocityA);
	DDX_Text(pDX,	IDC_VEL_PHI_EDIT,	m_fVelocityPhi);

	DDX_Text(pDX,	IDC_ACC_X_EDIT,		m_fAccelX);
	DDX_Text(pDX,	IDC_ACC_Y_EDIT,		m_fAccelY);
	DDX_Text(pDX,	IDC_ACC_RAD_EDIT,	m_fAccelR);
	DDX_Text(pDX,	IDC_ACC_ANG_EDIT,	m_fAccelA);

	DDX_Check(pDX, IDC_VEL_RAD_WAVE, m_bRadialVelocityWave);
	DDX_Check(pDX, IDC_VEL_ANG_WAVE, m_bThetaVelocityWave);
	DDX_Check(pDX, IDC_APPLY_WIND, m_bApplyWind);
	DDX_Check(pDX, IDC_VEL_PHI_WAVE, m_bPhiVelocityWave);
	DDX_Control(pDX, IDC_ACC_Z_SLIDER, m_sAccelZ);
	DDX_Text(pDX, IDC_ACC_Z_EDIT, m_fAccelZ);
	DDX_Control(pDX, IDC_ACC_ANG_SLIDER2, m_sAccelPhi);
	DDX_Text(pDX, IDC_ACC_ANG_EDIT2, m_fAccelPhi);
}


BEGIN_MESSAGE_MAP(CMovementDlg, DialogParentClass)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_VEL_X_SLIDER, &CMovementDlg::OnNMCustomdrawVelXSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_VEL_Y_SLIDER, &CMovementDlg::OnNMCustomdrawVelYSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_VEL_Z_SLIDER, &CMovementDlg::OnNMCustomdrawVelZSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_VEL_POLAR_PICKUP_RANGE_SLIDER, &CMovementDlg::OnNMCustomdrawVelPolarPickupRangeSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_VEL_RAD_SLIDER, &CMovementDlg::OnNMCustomdrawVelRadSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_VEL_ANG_SLIDER, &CMovementDlg::OnNMCustomdrawVelAngSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ACC_X_SLIDER, &CMovementDlg::OnNMCustomdrawAccXSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ACC_Y_SLIDER, &CMovementDlg::OnNMCustomdrawAccYSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ACC_RAD_SLIDER, &CMovementDlg::OnNMCustomdrawAccRadSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ACC_ANG_SLIDER, &CMovementDlg::OnNMCustomdrawAccAngSlider)
	
	ON_EN_UPDATE(IDC_VEL_X_EDIT, &CMovementDlg::OnEnUpdateVelXEdit)
	ON_EN_UPDATE(IDC_VEL_Y_EDIT, &CMovementDlg::OnEnUpdateVelYEdit)
	ON_EN_UPDATE(IDC_VEL_POLAR_PICKUP_RANGE_EDIT, &CMovementDlg::OnEnUpdateVelPolarPickupRangeEdit)
	ON_EN_UPDATE(IDC_VEL_RAD_EDIT, &CMovementDlg::OnEnUpdateVelRadEdit)
	ON_EN_UPDATE(IDC_VEL_ANG_EDIT, &CMovementDlg::OnEnUpdateVelAngEdit)
	ON_EN_UPDATE(IDC_ACC_X_EDIT, &CMovementDlg::OnEnUpdateAccXEdit)
	ON_EN_UPDATE(IDC_ACC_Y_EDIT, &CMovementDlg::OnEnUpdateAccYEdit)
	ON_EN_UPDATE(IDC_ACC_RAD_EDIT, &CMovementDlg::OnEnUpdateAccRadEdit)
	ON_EN_UPDATE(IDC_ACC_ANG_EDIT, &CMovementDlg::OnEnUpdateAccAngEdit)
	
	ON_BN_CLICKED(IDC_VEL_ANG_WAVE, &CMovementDlg::OnBnClickedVelAngWave)
	ON_BN_CLICKED(IDC_VEL_RAD_WAVE, &CMovementDlg::OnBnClickedVelRadWave)
	ON_BN_CLICKED(IDC_VEL_ANG_WAVE_PARAMS, &CMovementDlg::OnBnClickedVelAngWaveParams)
	ON_BN_CLICKED(IDC_VEL_RAD_WAVE_PARAMS, &CMovementDlg::OnBnClickedVelRadWaveParams)
	ON_BN_CLICKED(IDC_APPLY_WIND, &CMovementDlg::OnBnClickedApplyWind)
	ON_BN_CLICKED(IDC_WIND_OPTIONS, &CMovementDlg::OnBnClickedWindOptions)
	ON_BN_CLICKED(IDC_GRAVITY_OPTIONS, &CMovementDlg::OnBnClickedGravityOptions)
	ON_EN_UPDATE(IDC_VEL_Z_EDIT, &CMovementDlg::OnEnUpdateVelZEdit)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_VEL_PHI_SLIDER, &CMovementDlg::OnNMCustomdrawVelPhiSlider)
	ON_EN_UPDATE(IDC_VEL_PHI_EDIT, &CMovementDlg::OnEnUpdateVelPhiEdit)
	ON_BN_CLICKED(IDC_VEL_PHI_WAVE, &CMovementDlg::OnBnClickedVelPhiWave)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ACC_Z_SLIDER, &CMovementDlg::OnNMCustomdrawAccZSlider)
	ON_EN_UPDATE(IDC_ACC_Z_EDIT, &CMovementDlg::OnEnUpdateAccZEdit)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ACC_ANG_SLIDER2, &CMovementDlg::OnNMCustomdrawAccAngSlider2)
	ON_EN_UPDATE(IDC_ACC_ANG_EDIT2, &CMovementDlg::OnEnUpdateAccAngEdit2)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
void CMovementDlg::OnNMCustomdrawVelPolarPickupRangeSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sPolarPickupRange.GetPos();

	if (pos != lastPos)
	{
		UpdateData();
		m_fPolarPickupRange = pos * g_fInvFloatPrecision;
		UpdateData(false);
		THREAD_SAFE(g_pCurrentEmitter->PickupRange = m_fPolarPickupRange);
		lastPos = pos;
	}

	*pResult = 0;
}

void CMovementDlg::OnNMCustomdrawVelXSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sVelocityX.GetPos();

	if (pos != lastPos)
	{
		UpdateData();
		m_fVelocityX = pos * g_fInvFloatPrecision;
		UpdateData(false);
		THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vVel.x = m_fVelocityX);
		lastPos = pos;
	}

	*pResult = 0;
}

void CMovementDlg::OnNMCustomdrawVelYSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sVelocityY.GetPos();

	if (pos != lastPos)
	{
		UpdateData();
		m_fVelocityY = pos * g_fInvFloatPrecision;
		UpdateData(false);
		THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vVel.y = m_fVelocityY);
		lastPos = pos;
	}

	*pResult = 0;
}

void CMovementDlg::OnNMCustomdrawVelZSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	static int lastPos = -1;
	int pos = m_sVelocityZ.GetPos();

	if (pos != lastPos)
	{
		UpdateData();
		m_fVelocityZ = pos * g_fInvFloatPrecision;
		UpdateData(false);
		THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vVel.z = m_fVelocityZ);
		lastPos = pos;
	}

	*pResult = 0;
}

void CMovementDlg::OnNMCustomdrawVelRadSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sVelocityR.GetPos();

	if (pos != lastPos)
	{
		UpdateData();
		m_fVelocityR = pos * g_fInvFloatPrecision;
		UpdateData(false);
		THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vPolarVel.x = m_fVelocityR);
		lastPos = pos;
	}

	*pResult = 0;
}

void CMovementDlg::OnNMCustomdrawVelAngSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sVelocityA.GetPos();

	if (pos != lastPos)
	{
		UpdateData();
		m_fVelocityA = pos * g_fInvFloatPrecision;
		UpdateData(false);
		THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vPolarVel.y = m_fVelocityA);
		lastPos = pos;
	}

	*pResult = 0;
}

void CMovementDlg::OnNMCustomdrawVelPhiSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	static int lastPos = -1;
	int pos = m_sVelocityPhi.GetPos();

	if (pos != lastPos)
	{
		UpdateData();
		m_fVelocityPhi = pos * g_fInvFloatPrecision;
		UpdateData(false);
		THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vPolarVel.z = m_fVelocityPhi);
		lastPos = pos;
	}

	*pResult = 0;
}

void CMovementDlg::OnNMCustomdrawAccXSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sAccelX.GetPos();

	if (pos != lastPos)
	{
		UpdateData();
		m_fAccelX = pos * g_fInvFloatPrecision;
		UpdateData(false);
		THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vAccel.x = m_fAccelX);
		lastPos = pos;
	}

	*pResult = 0;
}

void CMovementDlg::OnNMCustomdrawAccYSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sAccelY.GetPos();

	if (pos != lastPos)
	{
		UpdateData();
		m_fAccelY = pos * g_fInvFloatPrecision;
		UpdateData(false);
		THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vAccel.y = m_fAccelY);
		lastPos = pos;
	}

	*pResult = 0;
}

void CMovementDlg::OnNMCustomdrawAccZSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	static int lastPos = -1;
	int pos = m_sAccelZ.GetPos();

	if (pos != lastPos)
	{
		UpdateData();
		m_fAccelZ = pos * g_fInvFloatPrecision;
		UpdateData(false);
		THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vAccel.z = m_fAccelZ);
		lastPos = pos;
	}

	*pResult = 0;
}

void CMovementDlg::OnNMCustomdrawAccRadSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sAccelR.GetPos();

	if (pos != lastPos)
	{
		UpdateData();
		m_fAccelR = pos * g_fInvFloatPrecision;
		UpdateData(false);
		THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vPolarAccel.x = m_fAccelR);
		lastPos = pos;
	}

	*pResult = 0;
}

void CMovementDlg::OnNMCustomdrawAccAngSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sAccelA.GetPos();

	if (pos != lastPos)
	{
		UpdateData();
		m_fAccelA = pos * g_fInvFloatPrecision;
		UpdateData(false);
		THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vPolarAccel.y = m_fAccelA);
		lastPos = pos;
	}

	*pResult = 0;
}

void CMovementDlg::OnNMCustomdrawAccAngSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	static int lastPos = -1;
	int pos = m_sAccelPhi.GetPos();

	if (pos != lastPos)
	{
		UpdateData();
		m_fAccelPhi = pos * g_fInvFloatPrecision;
		UpdateData(false);
		THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vPolarAccel.z = m_fAccelPhi);
		lastPos = pos;
	}

	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////

void CMovementDlg::OnEnUpdateVelPolarPickupRangeEdit()
{
	UpdateData();
	m_sPolarPickupRange.SetFloatPos(m_fPolarPickupRange);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->PickupRange = m_fPolarPickupRange);
}

void CMovementDlg::OnEnUpdateVelXEdit()
{
	UpdateData();
	m_sVelocityX.SetFloatPos(m_fVelocityX);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vVel.x = m_fVelocityX);
}

void CMovementDlg::OnEnUpdateVelYEdit()
{
	UpdateData();
	m_sVelocityY.SetFloatPos(m_fVelocityY);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vVel.y = m_fVelocityY);
}

void CMovementDlg::OnEnUpdateVelZEdit()
{
	UpdateData();
	m_sVelocityZ.SetFloatPos(m_fVelocityZ);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vVel.z = m_fVelocityZ);
}

void CMovementDlg::OnEnUpdateVelRadEdit()
{
	UpdateData();
	m_sVelocityR.SetFloatPos(m_fVelocityR);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vPolarVel.x = m_fVelocityR);
}

void CMovementDlg::OnEnUpdateVelAngEdit()
{
	UpdateData();
	m_sVelocityA.SetFloatPos(m_fVelocityA);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vPolarVel.y = m_fVelocityA);
}

void CMovementDlg::OnEnUpdateVelPhiEdit()
{
	UpdateData();
	m_sVelocityPhi.SetFloatPos(m_fVelocityPhi);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vPolarVel.z = m_fVelocityPhi);
}

void CMovementDlg::OnEnUpdateAccXEdit()
{
	UpdateData();
	m_sAccelX.SetFloatPos(m_fAccelX);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vAccel.x = m_fAccelX);
}

void CMovementDlg::OnEnUpdateAccYEdit()
{
	UpdateData();
	m_sAccelY.SetFloatPos(m_fAccelY);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vAccel.y = m_fAccelY);
}

void CMovementDlg::OnEnUpdateAccZEdit()
{
	UpdateData();
	m_sAccelZ.SetFloatPos(m_fAccelZ);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vAccel.z = m_fAccelZ);
}

void CMovementDlg::OnEnUpdateAccRadEdit()
{
	UpdateData();
	m_sAccelR.SetFloatPos(m_fAccelR);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vPolarAccel.x = m_fAccelR);
}

void CMovementDlg::OnEnUpdateAccAngEdit()
{
	UpdateData();
	m_sAccelA.SetFloatPos(m_fAccelA);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vPolarAccel.y = m_fAccelA);
}

void CMovementDlg::OnEnUpdateAccAngEdit2()
{
	UpdateData();
	m_sAccelPhi.SetFloatPos(m_fAccelPhi);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_vPolarAccel.z = m_fAccelPhi);
}

void CMovementDlg::OnBnClickedVelAngWave()
{
	UpdateData();
	g_Output << "Apply Angular Velocity Sine: " << (m_bThetaVelocityWave ? "true" : "false") << "\n";
	THREAD_SAFE(g_pCurrentEmitter->m_bThetaVelocityWave = m_bThetaVelocityWave);
}

void CMovementDlg::OnBnClickedVelPhiWave()
{
	UpdateData();
	g_Output << "Apply Angular Velocity Sine: " << (m_bThetaVelocityWave ? "true" : "false") << "\n";
	THREAD_SAFE(g_pCurrentEmitter->m_bPhiVelocityWave = m_bPhiVelocityWave);
}

void CMovementDlg::OnBnClickedVelRadWave()
{
	UpdateData();
	g_Output << "Apply Radial Velocity Sine: " << (m_bRadialVelocityWave ? "true" : "false") << "\n";
	THREAD_SAFE(g_pCurrentEmitter->m_bRadialVelocityWave = m_bRadialVelocityWave);
}

void CMovementDlg::OnBnClickedVelAngWaveParams()
{
	theApp.m_pFrame->m_WavePropsDlg.ShowWindow(SW_SHOW);
}

void CMovementDlg::OnBnClickedVelRadWaveParams()
{
	theApp.m_pFrame->m_WavePropsDlg.ShowWindow(SW_SHOW);
}

void CMovementDlg::OnBnClickedApplyWind()
{
	UpdateData();
	g_Output << "Wind Enabled: " << (m_bApplyWind ? "true" : "false") << "\n";
	THREAD_SAFE(g_pCurrentEmitter->m_bAnchorRoot = m_bApplyWind);
}

void CMovementDlg::OnBnClickedWindOptions()
{
	theApp.m_pFrame->m_VectorPropsDlg.ShowWindow(SW_SHOW);
}

void CMovementDlg::OnBnClickedGravityOptions()
{
	theApp.m_pFrame->m_VectorPropsDlg.ShowWindow(SW_SHOW);
}
