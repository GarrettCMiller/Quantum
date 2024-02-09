// AppearanceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Quantum.h"
#include "AppearanceDlg.h"


// CAppearanceDlg dialog

IMPLEMENT_DYNAMIC(CAppearanceDlg, DialogParentClass)

CAppearanceDlg::CAppearanceDlg(/*CWnd* pParent*/ /*=NULL*/)
	: DialogParentClass(CAppearanceDlg::IDD/*, pParent*/)
	, m_strTexturePath(_T(""))
{

}

CAppearanceDlg::~CAppearanceDlg()
{
}

void CAppearanceDlg::DoDataExchange(CDataExchange* pDX)
{
	DialogParentClass::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEXTURE_PATH, m_strTexturePath);
	DDX_Control(pDX, IDC_RECENT_TEXTURES, m_lbTextures);
}


BEGIN_MESSAGE_MAP(CAppearanceDlg, DialogParentClass)
	ON_BN_CLICKED(IDC_BROWSE_TEXTURE, &CAppearanceDlg::OnBnClickedBrowseTexture)
	ON_BN_CLICKED(IDC_APPLY_TEXTURE, &CAppearanceDlg::OnBnClickedApply)
	ON_BN_CLICKED(IDC_ADD_TEXTURE, &CAppearanceDlg::OnBnClickedAddTexture)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_APPLY_SEL_TEX, &CAppearanceDlg::OnBnClickedApplySelTex)
	ON_BN_CLICKED(IDC_DELETE_SEL_TEX, &CAppearanceDlg::OnBnClickedDeleteSelTex)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// CAppearanceDlg message handlers
//////////////////////////////////////////////////////////////////////////

BOOL CAppearanceDlg::OnInitDialog()
{
	DialogParentClass::OnInitDialog();

	m_strCurrentTexture = "Particle.bmp";
	
	m_lbTextures.AddString(m_strCurrentTexture);

	GetDlgItem(IDC_CURRENT_TEXTURE_PATH)->SetWindowText(m_strCurrentTexture);

	g_Output << "Initializing appearance dialog...\n";

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAppearanceDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CDC* pDC = GetDC();

	LPDIRECT3DTEXTURE9 tex = g_SpriteMgr->GetTexture(String(m_strCurrentTexture));
	LPDIRECT3DSURFACE9 surf = NULL;

	ASSERT(tex);
	
	tex->GetSurfaceLevel(0, &surf);

	ASSERT(surf);
	
	if (surf)
	{
		CDC temp;
		HDC hTemp;

		surf->GetDC(&hTemp);
			temp.Attach(hTemp);
				pDC->BitBlt(17, 25, 64, 64, &temp, 0, 0, SRCCOPY);
			temp.Detach();
		surf->ReleaseDC(hTemp);
	}

	surf->Release();
}

//////////////////////////////////////////////////////////////////////////

void CAppearanceDlg::OnBnClickedBrowseTexture()
{
	CFileDialog dlg(true, NULL, NULL, OFN_FILEMUSTEXIST);

	g_Output << "Setting new particle texture...\n";
	g_Output << "Browsing images...\n";

	if (dlg.DoModal() == IDOK)
	{
		m_strTexturePath = dlg.GetFileName();
		g_Output << "Texture chosen: " << m_strTexturePath << "...\n";
		UpdateData(false);
	}
}

//////////////////////////////////////////////////////////////////////////

void CAppearanceDlg::OnBnClickedApply()
{
	UpdateData();

	if (!m_strTexturePath.IsEmpty())
	{
		g_Output << "Applying texture " << m_strTexturePath << "...\n";
		THREAD_SAFE(g_pCurrentEmitter->SetTexture(g_SpriteMgr->LoadSprite((String)m_strTexturePath)));
		m_strCurrentTexture = m_strTexturePath;
		AddTextureString(String(m_strTexturePath.GetBuffer()));
	}
}

//////////////////////////////////////////////////////////////////////////

void CAppearanceDlg::OnBnClickedAddTexture()
{
	UpdateData();

	if (!m_strTexturePath.IsEmpty())
	{
		g_Output << "Adding new texture to list! Path: " << m_strTexturePath << "\n";
		AddTextureString(String(m_strTexturePath.GetBuffer()));
	}
}

//////////////////////////////////////////////////////////////////////////

void CAppearanceDlg::OnBnClickedApplySelTex()
{
	UpdateData();

	CString texpath;
	m_lbTextures.GetText(m_lbTextures.GetCurSel(), texpath);

	if (!texpath.IsEmpty())
	{
		g_Output << "Applying texture " << texpath << "...\n";
		THREAD_SAFE(g_pCurrentEmitter->SetTexture(g_SpriteMgr->LoadSprite((String)texpath)));
		m_strCurrentTexture = texpath;
	}
}

//////////////////////////////////////////////////////////////////////////

void CAppearanceDlg::OnBnClickedDeleteSelTex()
{
	UpdateData();

	//g_Output << "Removing texture " << 
	m_lbTextures.DeleteString(m_lbTextures.GetCurSel());
}

//////////////////////////////////////////////////////////////////////////

bool CAppearanceDlg::AddTextureString(String path)
{
	//Makre sure the string isn't already in the list
	if (m_lbTextures.FindStringExact(-1, path.c_str()) == LB_ERR)
	{
		m_lbTextures.AddString(m_strTexturePath);
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////