// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Quantum.h"

#include "MainFrm.h"

#include "D3Dwrapper.h"

#include "ParticleEmitter.h"

#include "EffectMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CParticleEmitter g_defaultEmitter;

extern Float g_Mass;

CButton btnCancel;
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_WM_ACTIVATE()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_OPEN_IMAGE, &CMainFrame::OnOpenImage)
	ON_COMMAND(ID_VIEW_GENERALOPTIONS, &CMainFrame::OnViewGeneraloptions)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GENERALOPTIONS, &CMainFrame::OnUpdateViewGeneraloptions)
	ON_COMMAND(ID_VIEW_MOVEMENTOPTIONS, &CMainFrame::OnViewMovementoptions)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOVEMENTOPTIONS, &CMainFrame::OnUpdateViewMovementoptions)
	ON_COMMAND(ID_VIEW_COLOROPTIONS, &CMainFrame::OnViewColoroptions)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOROPTIONS, &CMainFrame::OnUpdateViewColoroptions)
	ON_COMMAND(ID_VIEW_APPEARANCEOPTIONS, &CMainFrame::OnViewAppearanceoptions)
	ON_UPDATE_COMMAND_UI(ID_VIEW_APPEARANCEOPTIONS, &CMainFrame::OnUpdateViewAppearanceoptions)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	EnableActiveAccessibility();
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
	delete g_pDefaultEmitter;
	g_D3D->DeleteInstance();
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	// 	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
	// 		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	// 		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	// 	{
	// 		TRACE0("Failed to create toolbar\n");
	// 		return -1;      // fail to create
	// 	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);

	////////////////////////////////////////////
	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	AfxInitRichEdit2();

#ifndef _DEBUG
	CreateUtilityDock();
#endif

	CRichEditCtrl* temp = static_cast<CRichEditCtrl*>(&m_OutputDlg.m_Console);

	g_Output << "Attaching to GLib main logger...\n";
	OutputResult(g_Log.AttachMFC(temp));

	g_Output << "Attaching to GLib resource logger...\n";
	OutputResult(g_ResLog.AttachMFC(temp));

	g_Output << "Retrieving window handle for Direct3D initialization...\n";
	OutputResult((g_hWnd = m_wndView.GetSafeHwnd()) == NULL);

	static Float pSize = 2.0f;

	g_D3D->Init(GetSafeHwnd(), 1680, 1050);

	g_D3D->SetDeviceWindow(g_hWnd);
	g_D3D->GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);

	D3DLight light;
	ZeroMemory(&light, sizeof(light));
	light.Diffuse = Color::km_White;
	light.Ambient = Color::km_Gray;
	light.Direction = Vec3(0.0f, -1.0f, 0.0f);
	light.Type = D3DLIGHT_DIRECTIONAL;

	g_D3D->GetDevice()->SetLight(0, &light);
	g_D3D->GetDevice()->LightEnable(0, TRUE);
	g_D3D->GetDevice()->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(5,5,5));

	g_D3D->SetClipRange(50000.0f, 0.1f);

	grid.Init(100, 100, 5.0f);

	CCameraView cam;
	cam.SetPosition(k_v3Zero, fCamDist, fCamDir, fCamAlt);
	cam.SetTarget(k_v3Zero);
	g_D3D->SetCamera(cam);

	//Make sure we load local images
	g_SpriteMgr->m_ImagesBasePath = "";

	Effect& fx = g_EffectMgr->GetEffect(g_EffectMgr->LoadEffect("CustomVars.fx"));

	g_EffectMgr->LoadEffect("Particle.fx");

	Int32 numParticles = 50;
	Int32 cc = 0;

	fx.Shader->GetInt("g_NumParticles", &numParticles);
	fx.Shader->GetFloat("g_Mass", &g_Mass);
	fx.Shader->GetInt("g_ColorCollisions", &cc);

	theApp.m_Emitters.Resize(5, true);
	
	static const UInt32 k_NumEmitters = 2;

	g_D3D->GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);

	g_pRootEmitter/*g_pDefaultEmitter*/	= new CParticleEmitter;
	//g_Output << "Initializing " << k_NumEmitters << " particle emitters...\n";

	//for (it.Begin(); !it.End(); ++it)
	{
		g_pCurrentEmitter = g_pRootEmitter;//g_pDefaultEmitter;

		g_pCurrentEmitter->NumParticles			= numParticles;

		g_Output << "Initializing root emitter with " << g_pCurrentEmitter->NumParticles << " particles...\n";

		//Make sure we init before the other vars get set,
		//otherwise they might get overwritten
		g_pCurrentEmitter->InitEmitter();

		//g_pCurrentEmitter->m_bPointSprites		= true;

		g_pCurrentEmitter->m_fTimeMultiplier	= 1.0f;

		//g_pCurrentEmitter->m_
		Particle::mk_eColorMode					= Particle::eCM_KineticEnergy;

		g_pCurrentEmitter->EmitDist				= eED_Even;
		g_pCurrentEmitter->m_bApplyWind			= false;
		g_pCurrentEmitter->m_bApplyCollisions	= false;
		g_pCurrentEmitter->m_bApplyEMForce		= false;
		g_pCurrentEmitter->m_bApplyGravity		= true;

		g_pCurrentEmitter->m_bRandomizeCharge	= false;

		g_pCurrentEmitter->EmitRadius			= 0.0f;

		//g_pCurrentEmitter->m_Base.m_vPolarVel.x	= 1.0f;
		g_pCurrentEmitter->m_Base.m_fSize		= 1.0f;
		
		g_pCurrentEmitter->m_Base.m_nMinLife	= 10000;
		g_pCurrentEmitter->m_Base.m_nMaxLife	= 10000;
		
		g_pCurrentEmitter->m_Base.m_cColor		= Color(1.0f, 1.0f, 1.0f, 1.0f);

		g_Output << "Root emitter initialized!\n";
	}

	g_pRootEmitter->m_ForceMgr.m_bColorCollisions = (cc != 0);

	//g_pRootEmitter		= new CParticleEmitter(*g_pDefaultEmitter);
	g_pRootEmitter->m_strName = "Root";
	theApp.m_Emitters.Push(g_pRootEmitter);

	//Reset current emitter pointer
	//g_pCurrentEmitter = &theApp.m_Emitters[0];
	//g_pCurrentEmitter->InitEmitter();

	//emitters[1].m_Base.m_fSize	= 2;
	//emitters[1].m_Base.m_cColor	= Color(0.0f, 1.0f, 0.0f);

	theApp.m_bActive	= true;

	return 0;
}

//////////////////////////////////////////////////////////////////////////

void CMainFrame::CreateUtilityDock()
{
	m_UtilDock.Construct("Utility Window Dock", this);

	m_UtilDock.AddPage(&m_OutputDlg);
	m_UtilDock.AddPage(&m_StatsDlg);
	m_UtilDock.AddPage(&m_ViewOptionsDlg);

	m_UtilDock.Create(this,	WS_POPUP | WS_BORDER | WS_CAPTION | WS_VISIBLE | WS_SYSMENU, 
		WS_EX_ACCEPTFILES | WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW);

	g_Output << "Creating utility dock...\n";

	m_UtilDock.ShowWindow(SW_SHOW);

	g_Output << "Utility dock created!\n";
}

//////////////////////////////////////////////////////////////////////////

void CMainFrame::SetDlgProps()
{
	static bool bDone = false;

	if (bDone)
		return;

	bDone = true;

	CRect client;

	g_Output << "Initializing property dialogs...\n";

	// 	m_GenPropsDlg.Create(				IDD_PARTICLE_PROPERTIES,	this);
	// 	m_MovementDlg.Create(				IDD_PARTICLE_MOVEMENT,		this);
	// 	m_ColorDlg.Create(					IDD_PARTICLE_COLOR,			this);
	// 	m_AppearanceDlg.Create(				IDD_PARTICLE_APPEARANCE,	this);
	// 	m_ViewOptionsDlg.Create(			IDD_VIEW_OPTIONS,			this);
	// 	m_ParticleSystemsDlg.Create(		IDD_PARTICLE_SYSTEMS,		this);

	m_PropsDock.Construct(				"Property Window Dock", this);

	m_PropsDock.AddPage(				&m_GenPropsDlg);
	m_PropsDock.AddPage(				&m_MovementDlg);
	m_PropsDock.AddPage(				&m_ColorDlg);
	m_PropsDock.AddPage(				&m_AppearanceDlg);
	m_PropsDock.AddPage(				&m_ParticleSystemsDlg);
	m_PropsDock.AddPage(				&m_PhysPropsDlg);

	//m_PropsDock.DoModal();
	m_PropsDock.Create(					this, 
										WS_POPUP | WS_BORDER | WS_CAPTION | WS_VISIBLE | WS_SYSMENU, 
										WS_EX_ACCEPTFILES | WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW);

	// 	m_PropsDock.GetWindowRect(&client);
	// 	m_PropsDock.SetWindowPos(&wndTop, 50, 50, client.Width(), client.Height() + 50, /*SWP_NOMOVE | */SWP_NOZORDER);

	// 	client.right = 100;
	// 	client.bottom = 20;
	// 	client.left = 0;
	// 	client.top = 0;
	// 	client.MoveToXY(50, 550);
	// 	btnCancel.Create("Defaults", BS_TEXT, client, &m_PropsDock, 69420);
	// 	btnCancel.ShowWindow(SW_SHOW);

	//m_GenPropsDlg.SetModified(true);

	//m_OutputDlg.ShowWindow(				SW_SHOW);
	// 	m_GenPropsDlg.ShowWindow(			SW_SHOW);
	// 	m_MovementDlg.ShowWindow(			SW_SHOW);
	// 	m_ColorDlg.ShowWindow(				SW_SHOW);
	// 	m_AppearanceDlg.ShowWindow(			SW_SHOW);
	// 	m_ViewOptionsDlg.ShowWindow(		SW_SHOW);
	// 	m_ParticleSystemsDlg.ShowWindow(	SW_SHOW);
	//m_PropsDock.ShowWindow(				SW_SHOW);

	int x = 5, y = 5;

	int xoffset = 7, yoffset = 500;

	m_wndView.GetClientRect(&client);
	ClientToScreen(&client);

	//m_OutputDlg.SetWindowPos(			&wndTop, client.right - xoffset, 50, 0, 0, SWP_NOSIZE);
	m_PropsDock.SetWindowPos(			&wndTop, xoffset, client.Height() / 4, 0, 0, SWP_NOSIZE);
	// 	m_GenPropsDlg.SetWindowPos(			&wndTop, client.right - 2 * xoffset - x, client.top + 5, 0, 0, SWP_NOSIZE);
	// 	m_MovementDlg.SetWindowPos(			&wndTop, client.right - 2 * xoffset - x, client.bottom - yoffset, 0, 0, SWP_NOSIZE);
	// 	m_ColorDlg.SetWindowPos(			&wndTop, client.right - xoffset, client.bottom - yoffset, 0, 0, SWP_NOSIZE);
	// 	m_AppearanceDlg.SetWindowPos(		&wndTop, client.left + xoffset + x, client.top + 5, 0, 0, SWP_NOSIZE);
	// 	m_ViewOptionsDlg.SetWindowPos(		&wndTop, client.left + 5 , client.bottom - yoffset, 0, 0, SWP_NOSIZE);
	// 	m_ParticleSystemsDlg.SetWindowPos(	&wndTop, client.left + 5, client.top + 5, 0, 0, SWP_NOSIZE);
}

//////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnClose()
{
	theApp.m_bQuit = true;

	while (theApp.m_bInLoop)
	{
		g_Output << "Waiting for particle thread to exit...\n";
		Sleep(30);
	}

	theApp.m_Emitters.FreePointers();

	g_Log.DetachMFC();
	g_ResLog.DetachMFC();
	g_Output.DetachMFC();

	CFrameWnd::OnClose();
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	//theApp.m_bActive = !(nState == WA_INACTIVE);
}

BOOL CMainFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	static float k_Adj = 1.0f / 120.0f;
	static float kfM = 10.0f;

	fCamDist -= (zDelta * k_Adj * kfM);

	CCameraView cam = CCameraView(g_D3D->GetCamera());

	Vec3 vPos = cam.GetPosition();

	if (cam.HasTarget())
		vPos = cam.GetTarget();

	//kfM *= (zDelta * k_Adj < 0 ? 1.1f : 0.99f);

	cam.SetPosition(vPos, fCamDist, fCamDir, fCamAlt);
	cam.SetTarget(vPos);
	g_D3D->SetCamera(cam);

	g_Output << "Camera distance: " << fCamDist << "\n";


	return CFrameWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CMainFrame::OnOpenImage()
{
	g_pCurrentEmitter->m_csCriticalSection.Lock();

	g_Output << "Building particle image...\n";

	CFileDialog dlg(true, NULL, NULL, OFN_FILEMUSTEXIST, NULL, this);

	if (dlg.DoModal() == IDOK)
	{
		String name = dlg.GetFileName();

		g_Output << "Loading image " << name << "...\n";

		Sprite& image = g_SpriteMgr->GetSprite(g_SpriteMgr->LoadSprite(name));

		int width = NextLargestPowerOfTwo(image.width),
			height = NextLargestPowerOfTwo(image.height);

		g_D3D->CenterCameraOn(Vec2(width / 2, height / 2));

		g_pCurrentEmitter->NumParticles = width * height;

		g_Output << "Reinitializing particles...\n";

		g_pCurrentEmitter->InitParticles();

		g_pCurrentEmitter->DisplayBitmap(name);
	}

	g_pCurrentEmitter->m_csCriticalSection.Unlock();
}
void CMainFrame::OnViewGeneraloptions()
{
// 	if (m_GenPropsDlg.IsWindowVisible())
// 		m_GenPropsDlg.ShowWindow(SW_HIDE);
// 	else
// 		m_GenPropsDlg.ShowWindow(SW_SHOW);
}

void CMainFrame::OnUpdateViewGeneraloptions(CCmdUI *pCmdUI)
{
	//pCmdUI->SetCheck(m_GenPropsDlg.IsWindowVisible());
}

/////////////////////////////////////////////

void CMainFrame::OnViewMovementoptions()
{
// 	if (m_MovementDlg.IsWindowVisible())
// 		m_MovementDlg.ShowWindow(SW_HIDE);
// 	else
// 		m_MovementDlg.ShowWindow(SW_SHOW);
}

void CMainFrame::OnUpdateViewMovementoptions(CCmdUI *pCmdUI)
{
	//pCmdUI->SetCheck(m_MovementDlg.IsWindowVisible());
}

/////////////////////////////////////////////

void CMainFrame::OnViewColoroptions()
{
// 	if (m_ColorDlg.IsWindowVisible())
// 		m_ColorDlg.ShowWindow(SW_HIDE);
// 	else
// 		m_ColorDlg.ShowWindow(SW_SHOW);
}

//////////////////////////////////////////////////////////////////////////

void CMainFrame::OnUpdateViewColoroptions(CCmdUI *pCmdUI)
{
	//pCmdUI->SetCheck(m_ColorDlg.IsWindowVisible());
}

//////////////////////////////////////////////////////////////////////////

void CMainFrame::OnViewAppearanceoptions()
{
// 	if (m_AppearanceDlg.IsWindowVisible())
// 		m_AppearanceDlg.ShowWindow(SW_HIDE);
// 	else
// 		m_AppearanceDlg.ShowWindow(SW_SHOW);
}

//////////////////////////////////////////////////////////////////////////

void CMainFrame::OnUpdateViewAppearanceoptions(CCmdUI *pCmdUI)
{
	//pCmdUI->SetCheck(m_AppearanceDlg.IsWindowVisible());
}

//////////////////////////////////////////////////////////////////////////

// LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
// {
// 	int ret = CFrameWnd::WindowProc(message, wParam, lParam);
// 
// 	switch (message)
// 	{
// 	case WM_CREATE:
// 		{
// 			SetDlgProps();
// 		}
// 	}
// 
// 	return ret;
// }

//////////////////////////////////////////////////////////////////////////

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	SetDlgProps();
	// TODO: Add your message handler code here
}

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CFrameWnd::OnLButtonDown(nFlags, point);
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (message == WM_LBUTTONDOWN)
		return 0;

	return CFrameWnd::WindowProc(message, wParam, lParam);
}

void CMainFrame::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CFrameWnd::OnLButtonUp(nFlags, point);
}
