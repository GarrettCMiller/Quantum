// Quantum.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Quantum.h"
#include "MainFrm.h"

#include "ThreadMgr.h"

#include "Terrain.h"

#include "EffectMgr.h"

#include "ProfileSample.h"

#include "D3DImage.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


Bool	CQuantumApp::m_bActive			= false;
Bool	CQuantumApp::m_bQuit			= false;
Bool	CQuantumApp::m_bInLoop			= false;

Bool	CQuantumApp::m_bDraggingTabs	= false;
CWnd*	CQuantumApp::m_pDraggingTab		= NULL;

CThread*	CQuantumApp::m_pThread			= NULL;

//EmitterList	CQuantumApp::m_Emitters;
// CQuantumApp

BEGIN_MESSAGE_MAP(CQuantumApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CQuantumApp::OnAppAbout)
END_MESSAGE_MAP()

Bool					bGraphHistory;
CDynamicArray<UInt32>	arrGraphIDs;

// CQuantumApp construction

CQuantumApp::CQuantumApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CQuantumApp::~CQuantumApp()
{
	g_ThreadMgr->DeleteInstance();
}


// The one and only CQuantumApp object

CQuantumApp theApp;

// CQuantumApp initialization

BOOL CQuantumApp::InitInstance()
{
	//_CrtSetBreakAlloc(241);

//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	//INITCOMMONCONTROLSEX InitCtrls;
	//InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	//InitCtrls.dwICC = ICC_WIN95_CLASSES;
	//InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("GWare"));

	WriteProfileInt("Settings", "VideoWidth", 1600);
	WriteProfileInt("Settings", "VideoHeight", 900);

	CProfileSample::ResetAll();
	CProfileSample::ms_bProfilerEnabled = false;

	//m_Emitters.Resize(5, false);

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	m_pFrame = new CMainFrame;
	
	if (!m_pFrame)
		return FALSE;
	m_pMainWnd = m_pFrame;
	// create and load the frame with its resources
  	m_pFrame->LoadFrame(IDR_MAINFRAME,
  		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
  		NULL);

	// The one and only window has been initialized, so show and update it
	//m_pFrame->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE);
	m_pFrame->ShowWindow(SW_SHOWMAXIMIZED);
	m_pFrame->UpdateWindow();

	g_ThreadMgr->CreateThread(LPTHREAD_START_ROUTINE(&CQuantumApp::MainUpdateLoop), NULL, true, &m_pThread);
		
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}

void DoMotion()
{
	static Timer motionTimer(true);
	static const float Range =	2.0f;
								//150.0f;

	if (motionTimer > 0.1f)
	{
// 		Vec2 disp;
// 
// 		//disp.SetSpeedDir(Random(), Random() * 360.0f);
// 		disp.ChooseRandomDirection();
// 
// 		disp *= Range;
// 
// 		pe.m_Base.m_vPos += disp;
// 		//pe.m_Base.m_vPos = disp;
// 
// 		pe.Emit(1);

		motionTimer.Reset();
	}
}

// CQuantumApp message handlers

CVectorGraphics::GraphParams2D	graphParams(0, 120);
CVectorGraphics	vgGraph;
CD3DImage graphImage;
CMatrix matProj;

void DrawGraph(CCameraView& graphCam);

void DoVoxels();

DWORD CQuantumApp::MainUpdateLoop(LPVOID data)
{
	EmitterList& m_Emitters = theApp.m_Emitters;

	CCameraView cam;
	CCameraView graphCam;

	grid.SetShaderTech("");

	Effect& fx = g_EffectMgr->GetEffect("Particle.fx");
	UInt32 np = 0;

	vgGraph.Init();

	matProj.PerspectiveFOV(PI / 2.0f, g_D3D->GetAspectRatio(), 0.5f, 1000.0f);
	g_D3D->SetPerspective(matProj);

	matProj = g_D3D->CreateOrthoMatrix(500.0f, 500.0f);

	
	g_D3D->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);

	//graphImage.Init(Vec2u(256, 256));

	//m_Emitters[0]->m_Base.m
	//g_SpriteMgr->LoadSprite("Fractal2.bmp");
	//m_Emitters[0]->DisplayBitmap("Fractal2.bmp");

	while (!m_bQuit)
	{
		try
		{
			if (m_bActive)
			{
				//PROFILE("Main Loop");

				m_bInLoop = true;

				g_D3D->UpdateCamera();

				if (!g_bVoxel)
				{
					//if (g_pRootEmitter->m_bActive)
					{
						for (int i = 0; i < m_Emitters.NumItems(); i++)
						{
							m_Emitters[i]->m_csCriticalSection.Lock();

							//DoMotion();

							m_Emitters[i]->Update();

							m_Emitters[i]->m_csCriticalSection.Unlock();
						}
					}
				}

				if (D3D::Exists() && g_D3D->BeginScene(true))
				{
					g_D3D->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
					grid.Render();
					g_D3D->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
				
					//DoVoxels();

					for (int i = 0; i < m_Emitters.NumItems(); i++)
					{
						m_Emitters[i]->m_csCriticalSection.Lock();
						m_Emitters[i]->PreRender();
						m_Emitters[i]->m_csCriticalSection.Unlock();
					}

					if (!g_bVoxel)
						if (bGraphHistory)
							DrawGraph(graphCam);

					g_D3D->EndScene();

					if (g_bVoxel)
					{
						cam.SetPosition(k_v3Zero, fCamDist, fCamDir, fCamAlt);
						cam.SetTarget(k_v3Zero);
					}
					else
					{
						cam.SetPosition(m_Emitters[0]->m_ForceMgr.m_vCenterOfMass, fCamDist, fCamDir, fCamAlt);
						cam.SetTarget(m_Emitters[0]->m_ForceMgr.m_vCenterOfMass);
					}

					g_D3D->SetCamera(cam);
				}

				m_bInLoop = false;
			}
		}
		catch(CException* e)
		{
			e->ReportError();
		}

		CProfileSample::Output();
	}

	//Just in case
	m_bInLoop = false;

	m_pThread->Shutdown();

	return 0;
}

void DoVoxels()
{
}

void DrawGraph(CCameraView& graphCam)
{
#if 1
	static Effect& fx = g_EffectMgr->GetEffect("Particle.fx");
	static EmitterList& m_Emitters = theApp.m_Emitters;
	UInt32 np = 0;

	UInt32 particleIDToGraph = 22;

	g_D3D->PushRenderTarget(graphImage);

	g_D3D->Clear(Color::km_DarkGray);

	static Int32 fC = 0;

	Float camxp = 0.9f + (Max(0, fC - 450) * 0.005f);
						
	graphCam.SetPosition(Vec3(camxp, 0.0f));
	//cam.SetTarget(Vec3(0.0f, 0.0f));
	g_D3D->SetCamera(graphCam);
	g_D3D->UpdateCamera();

	fx.Shader->SetTechnique("Graph");
	fx.Shader->SetMatrix("matViewProj", &(matProj * g_D3D->matView));
	fx.Shader->Begin(&np, 0);
	fx.Shader->BeginPass(0);

	vgGraph.DrawLine(CVertex(Vec3(-1.0f, 0.0f, 0.6f), Vec3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f, Color::km_Gray), CVertex(Vec3(500.0f, 0.0f, 0.6f), Vec3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f, Color::km_Gray));

	m_Emitters[0]->m_csCriticalSection.Lock();

	for (UInt32 pe = 0; pe < arrGraphIDs.Size(); pe++)
	{
		vgGraph.GraphData2D(m_Emitters[0]->GetParticle(arrGraphIDs[pe]).m_ptArr, graphParams);
	
		fC = /*Max(UInt32(fC), */m_Emitters[0]->GetParticle(arrGraphIDs[pe]).m_ptArr.Size()/*)*/;
	}
	
	m_Emitters[0]->m_csCriticalSection.Unlock();

	fx.Shader->EndPass();
	fx.Shader->End();

	g_D3D->PopRenderTarget();

	g_D3D->BeginSpriteDevice(false);

	static Float xpos = g_D3D->GetScreenWidth()-50.0f-graphImage.ImageSize().X;

	g_D3D->GetSpriteObject()->Draw(graphImage.m_pTexture, NULL, &k_v3Zero, &Vec3(xpos, 50.0f), 0xFFFFFFFF);

	g_D3D->EndSpriteDevice();
#endif
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	EnableActiveAccessibility();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CQuantumApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CQuantumApp message handlers


BOOL CQuantumApp::ProcessMessageFilter(int code, LPMSG lpMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	BOOL ret = CWinApp::ProcessMessageFilter(code, lpMsg);

	/*if (lpMsg->message == WM_LBUTTONDOWN || lpMsg->message == WM_MOUSEMOVE)
	{
		CWnd	*pWnd		= CWnd::FromHandle(lpMsg->hwnd),
			*pParentWnd	= NULL;

		if (pWnd)
			pParentWnd = pWnd->GetParent();
		else return ret;

		//if (pParentWnd && pParentWnd->GetSafeHwnd() == static_cast<CMainFrame*>(m_pMainWnd)->m_PropsDock.GetSafeHwnd() && 
		//	PropSheet_GetTabControl(pParentWnd->GetSafeHwnd()) == pWnd->GetSafeHwnd() &&
		//	ret == false)
		//{
		//	switch (lpMsg->message)
		//	{
		//	case WM_LBUTTONDOWN:
		//		{
		//			m_bDraggingTabs = true;
		//			m_pDraggingTab = pWnd;
		//			//pParentWnd->SetCapture();
		//		}
		//		break;

		//	case WM_MOUSEMOVE:
		//		{
		//			if (!(lpMsg->wParam & MK_LBUTTON))
		//			{
		//				m_bDraggingTabs = false;
		//				m_pDraggingTab = NULL;
		//			}
		//		}
		//	}
		//}
	}*/

	return ret;
}

int CQuantumApp::Run()
{
	// TODO: Add your specialized code here and/or call the base class
	int ret = 0;

	try
	{
		ret = CWinApp::Run();
	}
	catch (CException* e)
	{
		e->ReportError();
		delete e;
	}

	return ret;
}
