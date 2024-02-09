// CColorDlg message handlers
// ColorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Quantum.h"
#include "ColorDlg.h"

#define GetSpinCtrl(name)	((CSpinButtonCtrl*) GetDlgItem(name))

// CColorDlg dialog

IMPLEMENT_DYNAMIC(CColorDlg, DialogParentClass)

CColorDlg::CColorDlg(/*CWnd* pParent *//*=NULL*/)
	: DialogParentClass(CColorDlg::IDD/*, pParent*/)
	, m_Red(255)
	, m_Green(255)
	, m_Blue(255)
	, m_Alpha(255)
	, m_FinalRed(0)
	, m_FinalGreen(0)
	, m_FinalBlue(0)
	, m_FinalAlpha(0)
	, m_bColorBlend(FALSE)
	, m_bRandomizeColor(FALSE)
	, m_ActiveColorMode(Particle::mk_eColorMode)
	, m_fColorMultiplier(1.0f)
{

}

CColorDlg::~CColorDlg()
{
}

BOOL CColorDlg::OnInitDialog()
{
	DialogParentClass::OnInitDialog();

	m_sliderRed.SetRange(0, 255, true);
	m_sliderGreen.SetRange(0, 255, true);
	m_sliderBlue.SetRange(0, 255, true);
	m_sliderAlpha.SetRange(0, 255, true);

	static const int k_MaxDecay = 255;
	m_sliderFinalRed.SetRange(-k_MaxDecay, k_MaxDecay, true);
	m_sliderFinalGreen.SetRange(-k_MaxDecay, k_MaxDecay, true);
	m_sliderFinalBlue.SetRange(-k_MaxDecay, k_MaxDecay, true);
	m_sliderFinalAlpha.SetRange(-k_MaxDecay, k_MaxDecay, true);

	static_cast<CComboBox*>(GetDlgItem(IDC_ACTIVE_COLOR_MODE))->AddString("None");
	
	static_cast<CComboBox*>(GetDlgItem(IDC_ACTIVE_COLOR_MODE))->AddString("Mass");
	static_cast<CComboBox*>(GetDlgItem(IDC_ACTIVE_COLOR_MODE))->AddString("Charge");
	
	static_cast<CComboBox*>(GetDlgItem(IDC_ACTIVE_COLOR_MODE))->AddString("Velocity");
	static_cast<CComboBox*>(GetDlgItem(IDC_ACTIVE_COLOR_MODE))->AddString("Acceleration");

	static_cast<CComboBox*>(GetDlgItem(IDC_ACTIVE_COLOR_MODE))->AddString("Momentum");

	static_cast<CComboBox*>(GetDlgItem(IDC_ACTIVE_COLOR_MODE))->AddString("Kinetic Energy");
	static_cast<CComboBox*>(GetDlgItem(IDC_ACTIVE_COLOR_MODE))->AddString("Potential Energy");

	g_Output << "Initializing color dialog...\n";

	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CColorDlg::UpdateAllControls()
{
	m_bColorBlend		= g_pCurrentEmitter->ColorBlendingEnabled();
	m_bRandomizeColor	= g_pCurrentEmitter->m_bRandomizeColor;

	m_Red				= g_pCurrentEmitter->m_Base.m_cColor.GetRed();
	m_Green				= g_pCurrentEmitter->m_Base.m_cColor.GetGreen();
	m_Blue				= g_pCurrentEmitter->m_Base.m_cColor.GetBlue();
	m_Alpha				= g_pCurrentEmitter->m_Base.m_cColor.GetAlpha();

	m_FinalRed			= g_pCurrentEmitter->m_Base.m_cDecay.GetRed();
	m_FinalGreen		= g_pCurrentEmitter->m_Base.m_cDecay.GetGreen();
	m_FinalBlue			= g_pCurrentEmitter->m_Base.m_cDecay.GetBlue();
	m_FinalAlpha		= g_pCurrentEmitter->m_Base.m_cDecay.GetAlpha();

	m_fColorMultiplier	= g_pCurrentEmitter->m_fColorMultiplier;

	m_ActiveColorMode	= Particle::mk_eColorMode;

	/*if (m_fColorMultiplier < 0.0f)
		m_fColorMultiplier = 1.0f;*/

	m_sliderRed.SetPos(m_Red);
	m_sliderGreen.SetPos(m_Green);
	m_sliderBlue.SetPos(m_Blue);
	m_sliderAlpha.SetPos(m_Alpha);

	m_sliderFinalRed.SetPos(m_FinalRed);
	m_sliderFinalGreen.SetPos(m_FinalGreen);
	m_sliderFinalBlue.SetPos(m_FinalBlue);
	m_sliderFinalAlpha.SetPos(m_FinalAlpha);

	UpdateData(false);
}

void CColorDlg::DoDataExchange(CDataExchange* pDX)
{
	DialogParentClass::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COLOR_RED_SLIDER, m_sliderRed);
	DDX_Control(pDX, IDC_COLOR_GREEN_SLIDER, m_sliderGreen);
	DDX_Control(pDX, IDC_COLOR_BLUE_SLIDER, m_sliderBlue);
	DDX_Control(pDX, IDC_COLOR_ALPHA_SLIDER, m_sliderAlpha);

	DDX_Control(pDX, IDC_DECAY_RED_SLIDER, m_sliderFinalRed);
	DDX_Control(pDX, IDC_DECAY_GREEN_SLIDER, m_sliderFinalGreen);
	DDX_Control(pDX, IDC_DECAY_BLUE_SLIDER, m_sliderFinalBlue);
	DDX_Control(pDX, IDC_DECAY_ALPHA_SLIDER, m_sliderFinalAlpha);

	DDX_Text(pDX, IDC_COLOR_RED_EDIT, m_Red);
	DDX_Text(pDX, IDC_COLOR_GREEN_EDIT, m_Green);
	DDX_Text(pDX, IDC_COLOR_BLUE_EDIT, m_Blue);
	DDX_Text(pDX, IDC_COLOR_ALPHA_EDIT, m_Alpha);

	DDX_Text(pDX, IDC_DECAY_RED_EDIT, m_FinalRed);
	DDX_Text(pDX, IDC_DECAY_GREEN_EDIT, m_FinalGreen);
	DDX_Text(pDX, IDC_DECAY_BLUE_EDIT, m_FinalBlue);
	DDX_Text(pDX, IDC_DECAY_ALPHA_EDIT, m_FinalAlpha);

	DDX_Text(pDX, IDC_COLOR_MULTIPLIER, m_fColorMultiplier);

	DDX_Check(pDX, IDC_COLOR_BLEND, m_bColorBlend);
	DDX_Check(pDX, IDC_RANDOMIZE_COLOR, m_bRandomizeColor);

	DDX_CBIndex(pDX, IDC_ACTIVE_COLOR_MODE, m_ActiveColorMode);
}


BEGIN_MESSAGE_MAP(CColorDlg, DialogParentClass)
 	ON_NOTIFY(NM_CUSTOMDRAW, IDC_COLOR_RED_SLIDER, &CColorDlg::OnNMCustomdrawColorRedSlider)
 	ON_NOTIFY(NM_CUSTOMDRAW, IDC_COLOR_GREEN_SLIDER, &CColorDlg::OnNMCustomdrawColorGreenSlider)
 	ON_NOTIFY(NM_CUSTOMDRAW, IDC_COLOR_BLUE_SLIDER, &CColorDlg::OnNMCustomdrawColorBlueSlider)
 	ON_NOTIFY(NM_CUSTOMDRAW, IDC_COLOR_ALPHA_SLIDER, &CColorDlg::OnNMCustomdrawColorAlphaSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_DECAY_RED_SLIDER, &CColorDlg::OnNMCustomdrawDecayRedSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_DECAY_GREEN_SLIDER, &CColorDlg::OnNMCustomdrawDecayGreenSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_DECAY_BLUE_SLIDER, &CColorDlg::OnNMCustomdrawDecayBlueSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_DECAY_ALPHA_SLIDER, &CColorDlg::OnNMCustomdrawDecayAlphaSlider)
	
	ON_EN_UPDATE(IDC_COLOR_RED_EDIT, &CColorDlg::OnEnUpdateColorRedEdit)
	ON_EN_UPDATE(IDC_COLOR_GREEN_EDIT, &CColorDlg::OnEnUpdateColorGreenEdit)
	ON_EN_UPDATE(IDC_COLOR_BLUE_EDIT, &CColorDlg::OnEnUpdateColorBlueEdit)
	ON_EN_UPDATE(IDC_COLOR_ALPHA_EDIT, &CColorDlg::OnEnUpdateColorAlphaEdit)
	
	ON_EN_UPDATE(IDC_COLOR_MULTIPLIER, &CColorDlg::OnEnUpdateColorMultiplierEdit)
	
	ON_BN_CLICKED(IDC_COLOR_BLEND, &CColorDlg::OnBnClickedColorBlend)
	ON_EN_UPDATE(IDC_DECAY_RED_EDIT, &CColorDlg::OnEnUpdateDecayRedEdit)
	ON_EN_UPDATE(IDC_DECAY_GREEN_EDIT, &CColorDlg::OnEnUpdateDecayGreenEdit)
	ON_EN_UPDATE(IDC_DECAY_BLUE_EDIT, &CColorDlg::OnEnUpdateDecayBlueEdit)
	ON_EN_UPDATE(IDC_DECAY_ALPHA_EDIT, &CColorDlg::OnEnUpdateDecayAlphaEdit)
	ON_BN_CLICKED(IDC_RANDOMIZE_COLOR, &CColorDlg::OnBnClickedRandomizeColor)

	ON_CBN_SELCHANGE(IDC_ACTIVE_COLOR_MODE, &CColorDlg::OnCbnSelchangeActiveColorMode)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// CColorDlg message handlers
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void CColorDlg::OnNMCustomdrawColorRedSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sliderRed.GetPos();

	if (pos != lastPos)
	{
		UpdateData();
		m_Red = pos;
		UpdateData(false);
		THREAD_SAFE(g_pCurrentEmitter->m_Base.m_cColor.SetRed(m_Red));
		lastPos = pos;
	}

	*pResult = 0;
}

void CColorDlg::OnNMCustomdrawColorGreenSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sliderGreen.GetPos();

	if (pos != lastPos)
	{
		UpdateData();
		m_Green = pos;
		UpdateData(false);
		THREAD_SAFE(g_pCurrentEmitter->m_Base.m_cColor.SetGreen(m_Green));
		lastPos = pos;
	}

	*pResult = 0;
}

void CColorDlg::OnNMCustomdrawColorBlueSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sliderBlue.GetPos();

	if (pos != lastPos)
	{
		UpdateData();
		m_Blue = pos;
		UpdateData(false);
		THREAD_SAFE(g_pCurrentEmitter->m_Base.m_cColor.SetBlue(m_Blue));
		lastPos = pos;
	}

	*pResult = 0;
}

void CColorDlg::OnNMCustomdrawColorAlphaSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sliderAlpha.GetPos();

	if (pos != lastPos)
	{
		UpdateData();
		m_Alpha = pos;
		UpdateData(false);
		THREAD_SAFE(g_pCurrentEmitter->m_Base.m_cColor.SetAlpha(m_Alpha));
		lastPos = pos;
	}

	*pResult = 0;
}

////////////////////////////////
////////////////////////////////

void CColorDlg::OnNMCustomdrawDecayRedSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sliderFinalRed.GetPos();

	if (pos != lastPos)
	{
		UpdateData();
		m_FinalRed = pos;
		UpdateData(false);
		THREAD_SAFE(g_pCurrentEmitter->m_Base.m_cDecay.SetRed(static_cast<UInt8>(m_FinalRed)));
		lastPos = pos;
	}

	*pResult = 0;
}

void CColorDlg::OnNMCustomdrawDecayGreenSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sliderFinalGreen.GetPos();

	if (pos != lastPos)
	{
		UpdateData();
		m_FinalGreen = pos;
		UpdateData(false);
		THREAD_SAFE(g_pCurrentEmitter->m_Base.m_cDecay.SetGreen(static_cast<UInt8>(m_FinalGreen)));
		lastPos = pos;
	}

	*pResult = 0;
}

void CColorDlg::OnNMCustomdrawDecayBlueSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sliderFinalBlue.GetPos();

	if (pos != lastPos)
	{
		UpdateData();
		m_FinalBlue = pos;
		UpdateData(false);
		THREAD_SAFE(g_pCurrentEmitter->m_Base.m_cDecay.SetBlue(static_cast<UInt8>(m_FinalBlue)));
		lastPos = pos;
	}

	*pResult = 0;
}

void CColorDlg::OnNMCustomdrawDecayAlphaSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	static int lastPos = -1;
	int pos = m_sliderFinalAlpha.GetPos();

	if (pos != lastPos)
	{
		UpdateData();
		m_FinalAlpha = pos;
		UpdateData(false);
		THREAD_SAFE(g_pCurrentEmitter->m_Base.m_cDecay.SetAlpha(static_cast<UInt8>(m_FinalAlpha)));
		lastPos = pos;
	}

	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void CColorDlg::OnEnUpdateColorRedEdit()
{
	UpdateData();
	m_sliderRed.SetPos(m_Red);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_cColor.SetRed(m_Red));
}

void CColorDlg::OnEnUpdateColorGreenEdit()
{
	UpdateData();
	m_sliderGreen.SetPos(m_Green);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_cColor.SetGreen(m_Green));
}

void CColorDlg::OnEnUpdateColorBlueEdit()
{
	UpdateData();
	m_sliderBlue.SetPos(m_Blue);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_cColor.SetBlue(m_Blue));
}

void CColorDlg::OnEnUpdateColorAlphaEdit()
{
	UpdateData();
	m_sliderAlpha.SetPos(m_Alpha);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_cColor.SetAlpha(m_Alpha));
}

//////////////////////////////////////////////////////////////////////////

void CColorDlg::OnBnClickedColorBlend()
{
	UpdateData();
	g_Output << "Color blending: " << (m_bColorBlend ? "true" : "false") << "\n";
	THREAD_SAFE(g_pCurrentEmitter->EnableColorBlending(m_bColorBlend));
}

void CColorDlg::OnEnUpdateDecayRedEdit()
{
	UpdateData();
	m_sliderFinalRed.SetPos(m_FinalRed);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_cDecay.SetRed(static_cast<UInt8>(m_FinalRed)));
}

void CColorDlg::OnEnUpdateDecayGreenEdit()
{
	UpdateData();
	m_sliderFinalGreen.SetPos(m_FinalGreen);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_cDecay.SetGreen(static_cast<UInt8>(m_FinalGreen)));
}

void CColorDlg::OnEnUpdateDecayBlueEdit()
{
	UpdateData();
	m_sliderFinalBlue.SetPos(m_FinalBlue);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_cDecay.SetBlue(static_cast<UInt8>(m_FinalBlue)));
}

void CColorDlg::OnEnUpdateDecayAlphaEdit()
{
	UpdateData();
	m_sliderFinalAlpha.SetPos(m_FinalAlpha);
	UpdateData(false);
	THREAD_SAFE(g_pCurrentEmitter->m_Base.m_cDecay.SetAlpha(static_cast<UInt8>(m_FinalAlpha)));
}

void CColorDlg::OnBnClickedRandomizeColor()
{
	UpdateData();
	g_Output << "Random colors: " << (m_bRandomizeColor ? "true" : "false") << "\n";
	THREAD_SAFE(g_pCurrentEmitter->m_bRandomizeColor = m_bRandomizeColor);
}

void CColorDlg::OnCbnSelchangeActiveColorMode()
{
	UpdateData();
	CString type;

	static_cast<CComboBox*>(GetDlgItem(IDC_ACTIVE_COLOR_MODE))->GetLBText(m_ActiveColorMode, type);
	
	g_Output << "Active color mode: " << type << "\n";
	
	THREAD_SAFE(Particle::mk_eColorMode = Particle::EColorMode(m_ActiveColorMode));
}

void CColorDlg::OnEnUpdateColorMultiplierEdit()
{
	UpdateData();
	THREAD_SAFE(g_pCurrentEmitter->m_fColorMultiplier = m_fColorMultiplier);
}