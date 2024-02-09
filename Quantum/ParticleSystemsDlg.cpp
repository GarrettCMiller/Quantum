// ParticleSystemsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Quantum.h"
#include "ParticleSystemsDlg.h"

#include "MainFrm.h"
// CParticleSystemsDlg dialog

extern CParticleEmitter g_defaultEmitter;

IMPLEMENT_DYNAMIC(CParticleSystemsDlg, DialogParentClass)

CParticleSystemsDlg::CParticleSystemsDlg(/*CWnd* pParent*/ /*=NULL*/)
	: DialogParentClass(CParticleSystemsDlg::IDD/*, pParent*/)
{

}

CParticleSystemsDlg::~CParticleSystemsDlg()
{
}

void CParticleSystemsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PARTICLE_SYSTEM_TREE, m_ParticleSystems);
}


BEGIN_MESSAGE_MAP(CParticleSystemsDlg, DialogParentClass)
	ON_BN_CLICKED(IDC_NEW_SYSTEM, &CParticleSystemsDlg::OnBnClickedNewSystem)
	ON_BN_CLICKED(IDC_DELETE_ALL, &CParticleSystemsDlg::OnBnClickedDeleteAll)
	ON_NOTIFY(TVN_SELCHANGED, IDC_PARTICLE_SYSTEM_TREE, &CParticleSystemsDlg::OnTvnSelchangedParticleSystemTree)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

BOOL CParticleSystemsDlg::OnInitDialog()
{
	static bool k_bImageListValid = false;

	DialogParentClass::OnInitDialog();

	m_ParticleSystems.DeleteAllItems();

	m_ParticleSystems.SetBkColor(0x000000);
	m_ParticleSystems.SetTextColor(0x00FF00);
	m_ParticleSystems.SetLineColor(0xFF0000);

	if (!k_bImageListValid)
	{
		list.Create(16, 16, ILC_COLOR16, 3, 3);

		list.Add(AfxGetApp()->LoadIcon(IDI_SYSTEM));
		list.Add(AfxGetApp()->LoadIcon(IDI_EMITTER));
		list.Add(AfxGetApp()->LoadIcon(IDI_PARTICLE));

		m_ParticleSystems.SetImageList(&list, TVSIL_NORMAL);
		k_bImageListValid = true;
	}

	m_ParticleSystems.InsertItem("Root", 0, 0);

	g_Output << "Initializing particle system dialog ...\n";

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////

// CParticleSystemsDlg message handler
void CParticleSystemsDlg::OnBnClickedNewSystem()
{
	g_Output << "Creating new particle system...\n";

	CParticleEmitter& emitter = *g_pCurrentEmitter;
	CParticleEmitter* pEmitter = new CParticleEmitter(emitter);

	static int id = 0;
	static char temp[16];

	pEmitter->m_bActive = false;
	pEmitter->m_bRender = false;

	pEmitter->m_strName = "Default";
	pEmitter->m_strName += _itoa(id++, temp, 10);

	pEmitter->InitEmitter();

	theApp.m_Emitters.Push(pEmitter);

	m_ParticleSystems.EnsureVisible(m_ParticleSystems.InsertItem(pEmitter->m_strName.c_str(), 1, 1, m_ParticleSystems.GetRootItem()/*GetSelectedItem()*/));
	m_ParticleSystems.Invalidate();
}

void CParticleSystemsDlg::OnBnClickedDeleteAll()
{
	if (MessageBox("Are you sure you want to delete all particle systems?\n\nOnce you delete all systems, you can't undo and get the systems back.", "Are you sure you want to delete all particle systems?", MB_YESNO | MB_ICONSTOP) == IDYES)
	{
		g_Output << "Deleting all particle systems...\n";
		m_ParticleSystems.DeleteAllItems();
		m_ParticleSystems.InsertItem("Root", 0, 0);
		theApp.m_pFrame->m_OutputDlg.Invalidate(true);
	}
}

void CParticleSystemsDlg::OnTvnSelchangedParticleSystemTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	int iIndex = -1;

	HTREEITEM item = m_ParticleSystems.GetSelectedItem();

	String name = m_ParticleSystems.GetItemText(item);//pNMTreeView->itemOld/*->itemNew*/.pszText;
	theApp.m_pFrame->m_PropsDock.SetWindowTextA((String("Property Window Dock - ") + name).c_str());

	for (iIndex = 0; iIndex < theApp.m_Emitters.NumItems(); iIndex++)
	{
		if (theApp.m_Emitters[iIndex]->m_strName == name)
		{
			g_pCurrentEmitter = theApp.m_Emitters[iIndex];

			if (::IsWindow(theApp.m_pFrame->m_GenPropsDlg.GetSafeHwnd()))
				theApp.m_pFrame->m_GenPropsDlg.UpdateAllControls();

			if (::IsWindow(theApp.m_pFrame->m_MovementDlg.GetSafeHwnd()))
				theApp.m_pFrame->m_MovementDlg.UpdateAllControls();
			
			if (::IsWindow(theApp.m_pFrame->m_ColorDlg.GetSafeHwnd()))
				theApp.m_pFrame->m_ColorDlg.UpdateAllControls();
		}
	}

	*pResult = 0;
}
