// GenProps.cpp : implementation file
//

#include "stdafx.h"
#include "Quantum.h"
#include "GenProps.h"

#include "MainFrm.h"

#include "GlobalFuncs.h"

// CGenProps dialog

EmitterList& g_Emitters = theApp.m_Emitters;

extern CDynamicArray<UInt32>	arrGraphIDs;
extern Bool						bGraphHistory;

IMPLEMENT_DYNAMIC(CGenProps, DialogParentClass)

CGenProps::CGenProps(/*CWnd* pParent*/ /*=NULL*/)
	: DialogParentClass(CGenProps::IDD/*, pParent*/)
	, m_nMaxParticles(1000)
	, m_fEmitRate(1)
	, m_fSize(1)
	, m_nMinLife(25)
	, m_nMaxLife(75)
	, m_fMinSpawnAngleTheta(0)
	, m_fMaxSpawnAngleTheta(0)
	, m_bParticleEmitterActive(false)
	, m_EmitShape(0)
	, m_fEmitRadius(0)
	, m_bVisible(FALSE)
	, m_fMinSpawnAnglePhi(0)
	, m_fMaxSpawnAnglePhi(0)
	, m_bPaused(false)
	, m_nRandomSeed(0)
	, m_fTimeMultiplier(1.0f)
	, m_bGraphHistory(false)
	, m_nEmitDist(0)
{

}

CGenProps::~CGenProps()
{
}

BOOL CGenProps::OnInitDialog()
{
	DialogParentClass::OnInitDialog();

	m_sMaxParticles.SetRange(1, MAX_PARTICLES, true);
	//m_sMaxParticles.SetLineSize()
	m_sEmitRate.SetFloatRange(0.1, 100);		//0.1 - 100.0
	m_sSize.SetFloatRange(0.1, 40);			//0.1 - 200.0

	m_sMinLife.SetRange(1, 10000, true);
	m_sMaxLife.SetRange(1, 10000, true);

	m_sEmitRadius.SetFloatRange(0, 10000);

	m_sMinSpawnAngleTheta.SetFloatRange(0, 360);	//0.0 - 360.0
	m_sMaxSpawnAngleTheta.SetFloatRange(0, 360);	//0.0 - 360.0

	m_sMinSpawnAnglePhi.SetFloatRange(0, 180);	//0.0 - 180.0
	m_sMaxSpawnAnglePhi.SetFloatRange(0, 180);	//0.0 - 180.0

	static_cast<CComboBox*>(GetDlgItem(IDC_PROPS_SPAWN_SHAPE))->AddString("Spherical");
	static_cast<CComboBox*>(GetDlgItem(IDC_PROPS_SPAWN_SHAPE))->AddString("Rectangular");
	static_cast<CComboBox*>(GetDlgItem(IDC_PROPS_SPAWN_SHAPE))->AddString("Arc");
	static_cast<CComboBox*>(GetDlgItem(IDC_PROPS_SPAWN_SHAPE))->AddString("Above Ground Sphere");
	static_cast<CComboBox*>(GetDlgItem(IDC_PROPS_SPAWN_SHAPE))->AddString("Four Spheres");
	static_cast<CComboBox*>(GetDlgItem(IDC_PROPS_SPAWN_SHAPE))->AddString("Galactic Disk");
	static_cast<CComboBox*>(GetDlgItem(IDC_PROPS_SPAWN_SHAPE))->AddString("Two Spheres");

	m_nEmitDist = eED_Even;
    
	g_Output << "Initializing general properties dialog...\n";

	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CGenProps::UpdateAllControls()
{
	m_fMinSpawnAngleTheta = g_pCurrentEmitter->MinSpawnAngleTheta;
	m_fMaxSpawnAngleTheta = g_pCurrentEmitter->MaxSpawnAngleTheta;

	m_fMinSpawnAnglePhi = g_pCurrentEmitter->MinSpawnAnglePhi;
	m_fMaxSpawnAnglePhi = g_pCurrentEmitter->MaxSpawnAnglePhi;

	//m_bParticleEmitterActive = g_pCurrentEmitter->m_bActive;

	m_nEmitDist = g_pCurrentEmitter->EmitDist;

	m_nMaxParticles = g_pCurrentEmitter->NumParticles;
	
	//m_fEmitRate = 
	m_fSize = g_pCurrentEmitter->m_Base.m_fSize;
	m_nMaxLife = g_pCurrentEmitter->m_Base.m_nMaxLife;
	m_nMinLife = g_pCurrentEmitter->m_Base.m_nMinLife;

	m_fTimeMultiplier = g_pCurrentEmitter->m_fTimeMultiplier;

	m_nRandomSeed = g_pCurrentEmitter->m_Seed;

	m_fEmitRadius = g_pCurrentEmitter->EmitRadius;

	m_bPaused = g_pCurrentEmitter->m_bActive;
	m_bVisible = g_pCurrentEmitter->m_bRender;

	m_sEmitRadius.SetPos(m_fEmitRadius * g_fFloatPrecision);

	m_sMaxParticles.SetPos(m_nMaxParticles);
	m_sEmitRate.SetPos(m_fEmitRate * g_fFloatPrecision);
	m_sSize.SetPos(m_fSize * g_fFloatPrecision);

	m_sMinLife.SetPos(m_nMinLife);
	m_sMaxLife.SetPos(m_nMaxLife);

	m_sMinSpawnAngleTheta.SetPos(m_fMinSpawnAngleTheta * g_fFloatPrecision);
	m_sMaxSpawnAngleTheta.SetPos(m_fMaxSpawnAngleTheta * g_fFloatPrecision);

	m_sMinSpawnAnglePhi.SetPos(m_fMinSpawnAnglePhi * g_fFloatPrecision);
	m_sMaxSpawnAnglePhi.SetPos(m_fMaxSpawnAnglePhi * g_fFloatPrecision);

	UpdateData(false);
}

void CGenProps::DoDataExchange(CDataExchange* pDX)
{
	DialogParentClass::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_EMITTER_ACTIVE, m_bParticleEmitterActive);
	DDX_Check(pDX, IDC_EMITTER_VISIBLE, m_bVisible);
	DDX_Check(pDX, IDC_EMITTER_PAUSED, m_bPaused);

	DDX_Check(pDX, IDC_GRAPH_HISTORY, m_bGraphHistory);

	DDX_Control(pDX, IDC_PROPS_MAX_PARTICLES_SLIDER, m_sMaxParticles);
	DDX_Control(pDX, IDC_PROPS_EMIT_RATE_SLIDER, m_sEmitRate);
	DDX_Control(pDX, IDC_PROPS_SIZE_SLIDER, m_sSize);

	DDX_Control(pDX, IDC_PROPS_MIN_LIFE_SLIDER, m_sMinLife);
	DDX_Control(pDX, IDC_PROPS_MAX_LIFE_SLIDER, m_sMaxLife);

	DDX_Control(pDX, IDC_PROPS_EMIT_RADIUS_SLIDER, m_sEmitRadius);

	DDX_Control(pDX, IDC_PROPS_MIN_SPAWN_ANGLE_THETA_SLIDER, m_sMinSpawnAngleTheta);
	DDX_Control(pDX, IDC_PROPS_MAX_SPAWN_ANGLE_THETA_SLIDER, m_sMaxSpawnAngleTheta);
	DDX_Control(pDX, IDC_PROPS_MIN_SPAWN_ANGLE_PHI_SLIDER, m_sMinSpawnAnglePhi);
	DDX_Control(pDX, IDC_PROPS_MAX_SPAWN_ANGLE_PHI_SLIDER, m_sMaxSpawnAnglePhi);

	DDX_Text(pDX, IDC_PROPS_MAX_PARTICLES_EDIT, m_nMaxParticles);
	DDX_Text(pDX, IDC_PROPS_EMIT_RATE_EDIT, m_fEmitRate);
	DDX_Text(pDX, IDC_PROPS_SIZE_EDIT, m_fSize);

	DDX_Text(pDX, IDC_PROPS_MIN_LIFE_EDIT, m_nMinLife);
	DDX_Text(pDX, IDC_PROPS_MAX_LIFE_EDIT, m_nMaxLife);

	DDX_Text(pDX, IDC_PROPS_EMIT_RADIUS_EDIT, m_fEmitRadius);

	DDX_Text(pDX, IDC_PROPS_MIN_SPAWN_ANGLE_THETA_EDIT, m_fMinSpawnAngleTheta);
	DDX_Text(pDX, IDC_PROPS_MAX_SPAWN_ANGLE_THETA_EDIT, m_fMaxSpawnAngleTheta);
	DDX_Text(pDX, IDC_PROPS_MIN_SPAWN_ANGLE_PHI_EDIT, m_fMinSpawnAnglePhi);
	DDX_Text(pDX, IDC_PROPS_MAX_SPAWN_ANGLE_PHI_EDIT, m_fMaxSpawnAnglePhi);

	DDX_Text(pDX, IDC_RANDOM_SEED, m_nRandomSeed);

	DDX_Text(pDX, IDC_TIME_MULTIPLIER, m_fTimeMultiplier);
	DDX_Text(pDX, IDC_GRAPH_IDS, m_strParticleID);

	DDX_CBIndex(pDX, IDC_PROPS_SPAWN_SHAPE, m_EmitShape);
	DDX_CBIndex(pDX, IDC_SPAWN_DIST, m_nEmitDist);
}


BEGIN_MESSAGE_MAP(CGenProps, DialogParentClass)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROPS_MAX_PARTICLES_SLIDER, &CGenProps::OnNMCustomdrawPropsMaxParticlesSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROPS_EMIT_RATE_SLIDER, &CGenProps::OnNMCustomdrawPropsEmitRateSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROPS_SIZE_SLIDER, &CGenProps::OnNMCustomdrawPropsSizeSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROPS_MIN_LIFE_SLIDER, &CGenProps::OnNMCustomdrawPropsMinLifeSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROPS_MAX_LIFE_SLIDER, &CGenProps::OnNMCustomdrawPropsMaxLifeSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROPS_EMIT_RADIUS_SLIDER, &CGenProps::OnNMCustomdrawPropsEmitRadiusSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROPS_MIN_SPAWN_ANGLE_THETA_SLIDER, &CGenProps::OnNMCustomdrawPropsMinSpawnAngleSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROPS_MAX_SPAWN_ANGLE_THETA_SLIDER, &CGenProps::OnNMCustomdrawPropsMaxSpawnAngleSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROPS_MIN_SPAWN_ANGLE_PHI_SLIDER, &CGenProps::OnNMCustomdrawPropsMinSpawnAnglePhiSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROPS_MAX_SPAWN_ANGLE_PHI_SLIDER, &CGenProps::OnNMCustomdrawPropsMaxSpawnAnglePhiSlider)
	
	ON_EN_UPDATE(IDC_PROPS_MAX_PARTICLES_EDIT, &CGenProps::OnEnUpdatePropsMaxParticlesEdit)
	ON_EN_UPDATE(IDC_PROPS_EMIT_RATE_EDIT, &CGenProps::OnEnUpdatePropsEmitRateEdit)
	ON_EN_UPDATE(IDC_PROPS_SIZE_EDIT, &CGenProps::OnEnUpdatePropsSizeEdit)
	ON_EN_UPDATE(IDC_PROPS_MIN_LIFE_EDIT, &CGenProps::OnEnUpdatePropsMinLifeEdit)
	ON_EN_UPDATE(IDC_PROPS_MAX_LIFE_EDIT, &CGenProps::OnEnUpdatePropsMaxLifeEdit)
	ON_EN_UPDATE(IDC_PROPS_EMIT_RADIUS_EDIT, &CGenProps::OnEnUpdatePropsEmitRadiusEdit)
	ON_EN_UPDATE(IDC_PROPS_MIN_SPAWN_ANGLE_THETA_EDIT, &CGenProps::OnEnUpdatePropsMinSpawnAngleEdit)
	ON_EN_UPDATE(IDC_PROPS_MAX_SPAWN_ANGLE_THETA_EDIT, &CGenProps::OnEnUpdatePropsMaxSpawnAngleEdit)
	
	ON_EN_UPDATE(IDC_GRAPH_IDS, &CGenProps::OnEnUpdatePropsParticleIDEdit)
	
	ON_EN_UPDATE(IDC_RANDOM_SEED, &CGenProps::OnEnUpdatePropsRandomSeedEdit)

	ON_EN_UPDATE(IDC_TIME_MULTIPLIER, &CGenProps::OnEnUpdatePropsTimeMultiplierEdit)
	
	ON_BN_CLICKED(IDC_EMITTER_ACTIVE, &CGenProps::OnBnClickedEmitterActive)
	ON_BN_CLICKED(IDC_RESET_ALL, &CGenProps::OnBnClickedResetAll)
	ON_BN_CLICKED(IDC_EMITTER_VISIBLE, &CGenProps::OnBnClickedEmitterVisible)
	ON_BN_CLICKED(IDC_RESET_CURRENT_EMITTER, &CGenProps::OnBnClickedResetCurrentEmitter)
	ON_BN_CLICKED(IDC_EMITTER_PAUSED, &CGenProps::OnBnClickedEmitterPaused)

	ON_BN_CLICKED(IDC_GRAPH_HISTORY, &CGenProps::OnBnClickedGraphHistory)
	
	ON_CBN_SELCHANGE(IDC_PROPS_SPAWN_SHAPE, &CGenProps::OnCbnSelchangePropsSpawnShape)
	ON_CBN_SELCHANGE(IDC_SPAWN_DIST, &CGenProps::OnCbnSelchangeSpawnDist)
	
	ON_WM_LBUTTONDOWN()
	
	ON_EN_UPDATE(IDC_PROPS_MIN_SPAWN_ANGLE_PHI_EDIT, &CGenProps::OnEnUpdatePropsMinSpawnAnglePhiEdit)
	ON_EN_UPDATE(IDC_PROPS_MAX_SPAWN_ANGLE_PHI_EDIT, &CGenProps::OnEnUpdatePropsMaxSpawnAnglePhiEdit)
END_MESSAGE_MAP()


// CGenProps message handlers

void CGenProps::OnNMCustomdrawPropsMaxParticlesSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	static int lastPos = -1;
	int pos = m_sMaxParticles.GetPos();

	if (pos != lastPos)
	{
		if (lastPos != -1)
		{
			UpdateData();
			m_nMaxParticles = pos;
			UpdateData(false);
			if (pos != g_pCurrentEmitter->NumParticles)
				THREAD_SAFE(g_pCurrentEmitter->SetupParticles(m_nMaxParticles));
		}

		lastPos = pos;
	}

	*pResult = 0;
}

void CGenProps::OnNMCustomdrawPropsEmitRateSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sEmitRate.GetPos();

	if (pos != lastPos)
	{
		if (pos != -1)
		{
			UpdateData();
			m_fEmitRate = pos * g_fInvFloatPrecision;
			UpdateData(false);
			THREAD_SAFE(g_pCurrentEmitter->EmitRate = m_fEmitRate);
		}

		lastPos = pos;
	}

	*pResult = 0;
}

void CGenProps::OnNMCustomdrawPropsSizeSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
		
	static int lastPos = -1;
	int pos = m_sSize.GetPos();

	if (pos != lastPos)
	{
		if (pos != -1)
		{
			UpdateData();
			m_fSize = pos * g_fInvFloatPrecision;
			UpdateData(false);
			THREAD_SAFE(g_pCurrentEmitter->m_Base.m_fSize = m_fSize);
		}

		lastPos = pos;
	}

	*pResult = 0;
}

void CGenProps::OnNMCustomdrawPropsMinLifeSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sMinLife.GetPos();

	if (pos != lastPos)
	{
		if (pos != -1)
		{
			UpdateData();
			m_nMinLife = pos;
			UpdateData(false);

			THREAD_SAFE(g_pCurrentEmitter->m_Base.m_nMinLife = m_nMinLife);

			if (m_nMaxLife < m_nMinLife)
			{
				m_nMaxLife = m_nMinLife;
				m_sMaxLife.SetPos(m_nMinLife);
			}
		}

		lastPos = pos;
	}

	*pResult = 0;
}

void CGenProps::OnNMCustomdrawPropsMaxLifeSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sMaxLife.GetPos();

	if (pos != lastPos)
	{
		if (pos != -1)
		{
			UpdateData();
			m_nMaxLife = pos;
			UpdateData(false);

			THREAD_SAFE(g_pCurrentEmitter->m_Base.m_nMaxLife = m_nMaxLife);

			if (m_nMaxLife < m_nMinLife)
			{
				m_nMinLife = m_nMaxLife;
				m_sMinLife.SetPos(m_nMaxLife);
			}
		}

		lastPos = pos;
	}

	*pResult = 0;
}

void CGenProps::OnNMCustomdrawPropsEmitRadiusSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	static int lastPos = -1;
	int pos = m_sEmitRadius.GetPos();

	if (pos != lastPos)
	{
		if (pos != -1)
		{
			UpdateData();
			m_fEmitRadius = pos * g_fInvFloatPrecision;
			UpdateData(false);
			THREAD_SAFE(g_pCurrentEmitter->EmitRadius = m_fEmitRadius);
		}

		lastPos = pos;
	}

	*pResult = 0;
}

void CGenProps::OnNMCustomdrawPropsMinSpawnAngleSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sMinSpawnAngleTheta.GetPos();

	if (pos != lastPos)
	{
		if (pos != -1)
		{
			UpdateData();
			m_fMinSpawnAngleTheta = pos * g_fInvFloatPrecision;
			UpdateData(false);

			THREAD_SAFE(g_pCurrentEmitter->MinSpawnAngleTheta = m_fMinSpawnAngleTheta);

			if (m_fMaxSpawnAngleTheta < m_fMinSpawnAngleTheta)
			{
				m_fMaxSpawnAngleTheta = m_fMinSpawnAngleTheta;
				m_sMaxSpawnAngleTheta.SetPos(m_fMinSpawnAngleTheta * g_fFloatPrecision);
			}
		}

		lastPos = pos;
	}

	*pResult = 0;
}

void CGenProps::OnNMCustomdrawPropsMaxSpawnAngleSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sMaxSpawnAngleTheta.GetPos();

	if (pos != lastPos)
	{
		if (pos != -1)
		{
			UpdateData();
			m_fMaxSpawnAngleTheta = pos * g_fInvFloatPrecision;
			UpdateData(false);

			THREAD_SAFE(g_pCurrentEmitter->MaxSpawnAngleTheta = m_fMaxSpawnAngleTheta);

			if (m_fMaxSpawnAngleTheta < m_fMinSpawnAngleTheta)
			{
				m_fMinSpawnAngleTheta = m_fMaxSpawnAngleTheta;
				m_sMinSpawnAngleTheta.SetPos(m_fMaxSpawnAngleTheta * g_fFloatPrecision);
			}
		}

		lastPos = pos;
	}

	*pResult = 0;
}

void CGenProps::OnNMCustomdrawPropsMinSpawnAnglePhiSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	static int lastPos = -1;
	int pos = m_sMinSpawnAnglePhi.GetPos();

	if (pos != lastPos)
	{
		if (pos != -1)
		{
			UpdateData();
			m_fMinSpawnAnglePhi = pos * g_fInvFloatPrecision;
			UpdateData(false);

			THREAD_SAFE(g_pCurrentEmitter->MinSpawnAnglePhi = m_fMinSpawnAnglePhi);

			if (m_fMaxSpawnAnglePhi < m_fMinSpawnAnglePhi)
			{
				m_fMaxSpawnAnglePhi = m_fMinSpawnAnglePhi;
				m_sMaxSpawnAnglePhi.SetPos(m_fMinSpawnAnglePhi * g_fFloatPrecision);
			}
		}

		lastPos = pos;
	}

	*pResult = 0;
}

void CGenProps::OnNMCustomdrawPropsMaxSpawnAnglePhiSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	static int lastPos = -1;
	int pos = m_sMaxSpawnAnglePhi.GetPos();

	if (pos != lastPos)
	{
		if (pos != -1)
		{
			UpdateData();
			m_fMaxSpawnAnglePhi = pos * g_fInvFloatPrecision;
			UpdateData(false);

			THREAD_SAFE(g_pCurrentEmitter->MaxSpawnAnglePhi = m_fMaxSpawnAnglePhi);

			if (m_fMaxSpawnAnglePhi < m_fMinSpawnAnglePhi)
			{
				m_fMinSpawnAnglePhi = m_fMaxSpawnAnglePhi;
				m_sMinSpawnAnglePhi.SetPos(m_fMaxSpawnAnglePhi * g_fFloatPrecision);
			}
		}

		lastPos = pos;
	}

	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void CGenProps::OnEnUpdatePropsMaxParticlesEdit()
{
	UpdateData();
	m_sMaxParticles.SetPos(m_nMaxParticles);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->SetupParticles(m_nMaxParticles));
}

void CGenProps::OnEnUpdatePropsEmitRateEdit()
{
	UpdateData();
	m_sEmitRate.SetPos(m_fEmitRate * g_fFloatPrecision);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->EmitRate = m_fEmitRate);
}

void CGenProps::OnEnUpdatePropsSizeEdit()
{
	UpdateData();
	m_sSize.SetPos(m_fSize * g_fFloatPrecision);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_fSize = m_fSize);
}

void CGenProps::OnEnUpdatePropsMinLifeEdit()
{
	UpdateData();
	m_sMinLife.SetPos(m_nMinLife);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_nMinLife = m_nMinLife);
}

void CGenProps::OnEnUpdatePropsMaxLifeEdit()
{
	UpdateData();
	m_sMaxLife.SetPos(m_nMaxLife);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_nMaxLife = m_nMaxLife);
}

void CGenProps::OnEnUpdatePropsEmitRadiusEdit()
{
	UpdateData();
	m_sEmitRadius.SetPos(m_fEmitRadius * g_fFloatPrecision);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->EmitRadius = m_fEmitRadius);
}

void CGenProps::OnEnUpdatePropsMinSpawnAngleEdit()
{
	UpdateData();
	m_sMinSpawnAngleTheta.SetPos(m_fMinSpawnAngleTheta * g_fFloatPrecision);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->MinSpawnAngleTheta = m_fMinSpawnAngleTheta);
}

void CGenProps::OnEnUpdatePropsMaxSpawnAngleEdit()
{
	UpdateData();
	m_sMaxSpawnAngleTheta.SetPos(m_fMaxSpawnAngleTheta * g_fFloatPrecision);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->MaxSpawnAngleTheta = m_fMaxSpawnAngleTheta);
}

void CGenProps::OnEnUpdatePropsMinSpawnAnglePhiEdit()
{
	UpdateData();
	m_sMinSpawnAnglePhi.SetPos(m_fMinSpawnAnglePhi * g_fFloatPrecision);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->MinSpawnAnglePhi = m_fMinSpawnAnglePhi);
}

void CGenProps::OnEnUpdatePropsMaxSpawnAnglePhiEdit()
{
	UpdateData();
	m_sMaxSpawnAnglePhi.SetPos(m_fMaxSpawnAnglePhi * g_fFloatPrecision);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->MaxSpawnAnglePhi = m_fMaxSpawnAnglePhi);
}

void CGenProps::OnBnClickedEmitterActive()
{
	//UpdateData();
	m_bParticleEmitterActive = !m_bParticleEmitterActive;
	
	g_Output << "Particle emitter enabled: " << (m_bParticleEmitterActive ? "true" : "false") << "\n";
	//g_Output << "Enabling profiler...\n";
	//CProfileSample::ms_bProfilerEnabled = m_bParticleEmitterActive;
	//g_Output << "Resetting all profiler samples...\n";
	//CProfileSample::ResetAll();
	//for (int i = 0; i < theApp.m_Emitters.NumItems(); i++)
	
	//It's being retarded and won't toggle variable
	//Since I don't have the resource editor with this version of Visual Studio...
	//m_bParticleEmitterActive = !m_bParticleEmitterActive;

	THREAD_SAFE(g_pCurrentEmitter->SetActive(static_cast<Bool>(m_bParticleEmitterActive), m_bParticleEmitterActive, true));

	m_bVisible = g_pCurrentEmitter->m_bRender;
	UpdateData(false);
}

void CGenProps::OnBnClickedEmitterPaused()
{
	UpdateData();
	
	g_Output << "Particle emitter paused: " << (m_bPaused ? "true" : "false") << "\n";
	//g_Output << "Enabling profiler...\n";
	//CProfileSample::ms_bProfilerEnabled = m_bParticleEmitterActive;
	//g_Output << "Resetting all profiler samples...\n";
	//CProfileSample::ResetAll();
	//for (int i = 0; i < theApp.m_Emitters.NumItems(); i++)
	
	//It's being retarded and won't toggle variable
	//Since I don't have the resource editor with this version of Visual Studio...
	//m_bParticleEmitterActive = !m_bParticleEmitterActive;

	THREAD_SAFE(g_pCurrentEmitter->SetActive(static_cast<Bool>(!m_bPaused), true, false));
}

void CGenProps::OnCbnSelchangePropsSpawnShape()
{
	UpdateData();
	CString type;

	static_cast<CComboBox*>(GetDlgItem(IDC_PROPS_SPAWN_SHAPE))->GetLBText(m_EmitShape, type);
	
	g_Output << "Particle spawn shape: " << type << "\n";
	
	THREAD_SAFE(g_pCurrentEmitter->EmitShape = EEmitShape(m_EmitShape));
}

void CGenProps::OnBnClickedResetAll()
{
	g_Output << "Resetting all emitters...\n";

	CParticleEmitter* pTemp = g_pCurrentEmitter;

	for (int i = 0; i < theApp.m_Emitters.NumItems(); i++)
	{
		g_pCurrentEmitter = theApp.m_Emitters[i];
		THREAD_SAFE(g_pCurrentEmitter->ResetActiveParticles(CParticleEmitter::eResetAll));
	}

	g_pCurrentEmitter = pTemp;
}

void CGenProps::OnBnClickedGraphHistory()
{
	UpdateData();
	bGraphHistory = m_bGraphHistory;
}

BOOL CGenProps::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

	//if ()

	return CPropertyPage::OnSetActive();
}

void CGenProps::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CPropertyPage::OnLButtonDown(nFlags, point);
}

void CGenProps::OnBnClickedEmitterVisible()
{
	UpdateData();
	g_Output << "Emitter visible: " << (m_bVisible ? "true" : "false") << "\n";
	THREAD_SAFE(g_pCurrentEmitter->m_bRender = m_bVisible);
}

void CGenProps::OnBnClickedResetCurrentEmitter()
{
	g_Output << "Resetting current emitter...\n";
	THREAD_SAFE(g_pCurrentEmitter->ResetActiveParticles(CParticleEmitter::eResetAll));
}

void CGenProps::OnEnUpdatePropsRandomSeedEdit()
{
	UpdateData();
	g_Output << "New random seed: " << m_nRandomSeed << "\n";
	THREAD_SAFE(g_pCurrentEmitter->m_Seed = m_nRandomSeed);
}

void CGenProps::OnEnUpdatePropsTimeMultiplierEdit()
{
	UpdateData();//* g_fFloatPrecision
	g_Output << "Time multiplier: " << m_fTimeMultiplier << "\n";
	THREAD_SAFE(g_pCurrentEmitter->m_fTimeMultiplier = m_fTimeMultiplier);
}

void CGenProps::OnEnUpdatePropsParticleIDEdit()
{
	UpdateData();

	String ids = m_strParticleID;

	arrGraphIDs.Clear();

	TStringArray arrIDs = Tokenize(ids, ",");

	if (!arrIDs.IsEmpty())
	{
		for (UInt32 i = 0; i < arrIDs.Size(); i++)
		{
			if (IsANumber(arrIDs[i]))
				arrGraphIDs.Push(atoi(arrIDs[i].c_str()));
		}
	}

	if (!arrGraphIDs.IsEmpty())
		THREAD_SAFE(g_pCurrentEmitter->_id = arrGraphIDs[0];)
}
void CGenProps::OnCbnSelchangeSpawnDist()
{
	UpdateData();
	THREAD_SAFE(g_pCurrentEmitter->EmitDist = static_cast<EEmitDistribution>(m_nEmitDist));
	g_Output << "New emission distribution: ";
	switch (static_cast<EEmitDistribution>(m_nEmitDist))
	{
	case eED_Boundary:	g_Output << "Boundary"; break;
	case eED_Even:		g_Output << "Even"; break;
	case eED_Random:	g_Output << "Random"; break;
	}

	g_Output << "\n";
}
