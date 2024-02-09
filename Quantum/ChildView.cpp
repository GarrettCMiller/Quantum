// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Quantum.h"
#include "MainFrm.h"
#include "ChildView.h"
#include "D3DWrapper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView

CChildView::CChildView()
{
	EnableActiveAccessibility();
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	//ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	//ON_COMMAND(ID_OPEN_IMAGE, &CChildView::OnOpenImage)
	ON_COMMAND(ID_FILE_SAVESTATE, &CChildView::OnSaveState)
	ON_COMMAND(ID_FILE_LOADSTATE, &CChildView::OnLoadState)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH), NULL);

	return TRUE;
}

// void CChildView::OnPaint() 
// {
// 	CPaintDC dc(this); // device context for painting
// 	
// 	// TODO: Add your message handler code here
// 	
// 	// Do not call CWnd::OnPaint() for painting messages
// }


LRESULT CChildView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		break;

	case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_LEFT:
				{
					fCamDir -= 5.0f;
				
					if (fCamDir < 0.0f)
						fCamDir += 360.0f;

					fCamDir = fmodf(fCamDir, 360.0f);

					CCameraView cam = CCameraView(g_D3D->GetCamera());
					cam.SetPosition(k_v3Zero, fCamDist, fCamDir, fCamAlt);
					cam.SetTarget(k_v3Zero);
					g_D3D->SetCamera(cam);
				}
				break;

			case VK_RIGHT:
				{
					fCamDir += 5.0f;
				
					if (fCamDir < 0.0f)
						fCamDir += 360.0f;

					fCamDir = fmodf(fCamDir, 360.0f);

					CCameraView cam = CCameraView(g_D3D->GetCamera());
					cam.SetPosition(k_v3Zero, fCamDist, fCamDir, fCamAlt);
					cam.SetTarget(k_v3Zero);
					g_D3D->SetCamera(cam);
				}
				break;

			case VK_UP:
				{
					fCamAlt -= 5.0f;
				
					fCamAlt = Clamp(fCamAlt, 1.0f, 179.0f);

					CCameraView cam = CCameraView(g_D3D->GetCamera());
					cam.SetPosition(k_v3Zero, fCamDist, fCamDir, fCamAlt);
					cam.SetTarget(k_v3Zero);
					g_D3D->SetCamera(cam);
				}
				break;

			case VK_DOWN:
				{
					fCamAlt += 5.0f;
				
					fCamAlt = Clamp(fCamAlt, 1.0f, 179.0f);

					CCameraView cam = CCameraView(g_D3D->GetCamera());
					cam.SetPosition(k_v3Zero, fCamDist, fCamDir, fCamAlt);
					cam.SetTarget(k_v3Zero);
					g_D3D->SetCamera(cam);
				}
				break;

			case VK_HOME:
				{
					fCamDist *= 0.99f;
				
					//fCamDist = Clamp(fCamDist, 0.5f, 1000.0f);

					CCameraView cam = CCameraView(g_D3D->GetCamera());
					cam.SetPosition(k_v3Zero, fCamDist, fCamDir, fCamAlt);
					cam.SetTarget(k_v3Zero);
					g_D3D->SetCamera(cam);
				}
				break;

			case VK_END:
				{
					fCamDist *= 1.01f;
				
					//fCamDist = Clamp(fCamDist, 0.5f, 1000.0f);

					CCameraView cam = CCameraView(g_D3D->GetCamera());
					cam.SetPosition(k_v3Zero, fCamDist, fCamDir, fCamAlt);
					cam.SetTarget(k_v3Zero);
					g_D3D->SetCamera(cam);
				}
				break;
			}
		}
	
	default:
		return CWnd::WindowProc(message, wParam, lParam);
	}

	return 1;
}

CPoint lastPoint(-1, -1);
Vec3 eyePt;

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	static const float mx = 0.001f, my = 0.001f;

	if (nFlags & MK_LBUTTON || nFlags & MK_RBUTTON)
	{
		if (lastPoint.x != -1)
		{
			int dx = lastPoint.x - point.x,
				dy = lastPoint.y - point.y;

			CCameraView cam = CCameraView(g_D3D->GetCamera());

			if (nFlags & MK_LBUTTON)
			{
				if (dx != 0)
					fCamDir -= dx / abs(dx);// * mx;
			
				if (dy != 0)
					fCamAlt -= dy / abs(dy);// * my;

				if (fCamDir < 0.0f)
					fCamDir += 360.0f;

				fCamDir = fmodf(fCamDir, 360.0f);

				fCamAlt = Clamp(fCamAlt, 1.0f, 179.0f);

				cam.SetPosition(eyePt, fCamDist, fCamDir, fCamAlt);
				cam.SetTarget(eyePt);
			}
			else
			{
				cam.MoveRight(dx);
				cam.MoveUp(dy);

				eyePt = cam.GetTarget();

				cam.SetPosition(eyePt, fCamDist, fCamDir, fCamAlt);
				cam.SetTarget(eyePt);
			}

			
			g_D3D->SetCamera(cam);
		}

		lastPoint = point;
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	g_pCurrentEmitter->m_csCriticalSection.Lock();
	
	g_Output << "Randomizing particle velocities...\n";

	//pe.m_Base.m_vPolarVel.x = 5.0f;
// 	pe.m_Base.m_vAccel.x = 0.1f;
// 	pe.m_Base.m_vAccel.y = -0.1f;

	//pe.m_Base.m_vPolarVel.y = 1.0f;

	g_pCurrentEmitter->ResetActiveParticles(CParticleEmitter::eResetPos);
	//pe.SetGlobalBirthPos(Vec2(128.0f, 128.0f));
	//pe.SetGlobalBirthPos(Vec2(512.0f, 512.0f));
	//pe.m_bApplyGravity = true;

	g_pCurrentEmitter->m_csCriticalSection.Unlock();

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CChildView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	g_pCurrentEmitter->m_csCriticalSection.Lock();

// 	pe.m_Base.m_vPolarVel = k_vZero;
// 
// 	pe.InitParticles();
// 
// 	pe.DisplayBitmap("Marine.bmp");

// 	pe.m_Base.m_vAccel = k_vZero;
// 	pe.ResetActiveParticles(true, false);

	g_Output << "Particles seeking target...\n";

	g_pCurrentEmitter->SeekBirthplace();

	g_pCurrentEmitter->m_csCriticalSection.Unlock();

	CWnd::OnRButtonDblClk(nFlags, point);
}

// void CChildView::OnOpenImage()
// {
// 	pe.m_csCriticalSection.Lock();
// 
// 	CFileDialog dlg(true, NULL, NULL, OFN_FILEMUSTEXIST, NULL, this);
// 
// 	if (dlg.DoModal() == IDOK)
// 	{
// 		String name = dlg.GetFileName();
// 
// 		g_SpriteMgr->LoadSprite(name);
// 		pe.ResetActiveParticles();
// 		pe.DisplayBitmap(name);
// 	}
// 
// 	pe.m_csCriticalSection.Unlock();
// }

void CChildView::OnSaveState()
{
	CFileDialog dlg(false, "pfx", "Temp.pfx", OFN_OVERWRITEPROMPT, "*.pfx", this);

	if (dlg.DoModal() == IDOK)
	{
		String name = dlg.GetFileName();

		FILE* f = fopen(name.c_str(), "wb");

		if (f)
		{
			g_pCurrentEmitter->m_csCriticalSection.Lock();

			//GenProps tab
			fwrite(&g_pCurrentEmitter->m_Seed,				sizeof(UInt32), 1, f);
			fwrite(&g_pCurrentEmitter->EmitDist,			sizeof(int), 1, f);
			fwrite(&g_pCurrentEmitter->NumParticles,		sizeof(Int32), 1, f);
			fwrite(&g_pCurrentEmitter->m_Base.m_fSize,		sizeof(Float), 1, f);
			fwrite(&g_pCurrentEmitter->EmitShape,			sizeof(int), 1, f);
			fwrite(&g_pCurrentEmitter->MinSpawnAngleTheta,	sizeof(Float), 1, f);
			fwrite(&g_pCurrentEmitter->MaxSpawnAngleTheta,	sizeof(Float), 1, f);
			fwrite(&g_pCurrentEmitter->MinSpawnAnglePhi,	sizeof(Float), 1, f);
			fwrite(&g_pCurrentEmitter->MaxSpawnAnglePhi,	sizeof(Float), 1, f);

			//Movement tab
			fwrite(&g_pCurrentEmitter->m_Base.m_vVel,		sizeof(Float), 1, f);
			fwrite(&g_pCurrentEmitter->m_Base.m_vPolarVel,	sizeof(Float), 1, f);
			fwrite(&g_pCurrentEmitter->m_Base.m_vAccel,		sizeof(Float), 1, f);
			fwrite(&g_pCurrentEmitter->m_Base.m_vPolarAccel,sizeof(Float), 1, f);
			fwrite(&g_pCurrentEmitter->m_bApplyGravity,		sizeof(Bool), 1, f);
			fwrite(&g_pCurrentEmitter->m_bApplyWind,		sizeof(Bool), 1, f);

			//Color tab
			fwrite(&g_pCurrentEmitter->m_fColorMultiplier,	sizeof(Float), 1, f);

			g_pCurrentEmitter->m_csCriticalSection.Unlock();

			fclose(f);
		}
	}
}

void CChildView::OnLoadState()
{
	CFileDialog dlg(true, "pfx", "*.pfx", OFN_FILEMUSTEXIST, "*.pfx", this);

	if (dlg.DoModal() == IDOK)
	{
		String name = dlg.GetFileName();

		FILE* f = fopen(name.c_str(), "rb");

		if (f)
		{
			g_pCurrentEmitter->m_csCriticalSection.Lock();

			//GenProps tab
			fread(&g_pCurrentEmitter->m_Seed,				sizeof(UInt32), 1, f);
			fread(&g_pCurrentEmitter->EmitDist,				sizeof(int), 1, f);
			fread(&g_pCurrentEmitter->NumParticles,			sizeof(Int32), 1, f);
			fread(&g_pCurrentEmitter->m_Base.m_fSize,		sizeof(Float), 1, f);
			fread(&g_pCurrentEmitter->EmitShape,			sizeof(int), 1, f);
			fread(&g_pCurrentEmitter->MinSpawnAngleTheta,	sizeof(Float), 1, f);
			fread(&g_pCurrentEmitter->MaxSpawnAngleTheta,	sizeof(Float), 1, f);
			fread(&g_pCurrentEmitter->MinSpawnAnglePhi,		sizeof(Float), 1, f);
			fread(&g_pCurrentEmitter->MaxSpawnAnglePhi,		sizeof(Float), 1, f);

			//Movement tab
			fread(&g_pCurrentEmitter->m_Base.m_vVel,		sizeof(Float), 1, f);
			fread(&g_pCurrentEmitter->m_Base.m_vPolarVel,	sizeof(Float), 1, f);
			fread(&g_pCurrentEmitter->m_Base.m_vAccel,		sizeof(Float), 1, f);
			fread(&g_pCurrentEmitter->m_Base.m_vPolarAccel,	sizeof(Float), 1, f);
			fread(&g_pCurrentEmitter->m_bApplyGravity,		sizeof(Bool), 1, f);
			fread(&g_pCurrentEmitter->m_bApplyWind,			sizeof(Bool), 1, f);

			//Color tab
			fread(&g_pCurrentEmitter->m_fColorMultiplier,	sizeof(Float), 1, f);

			g_pCurrentEmitter->m_csCriticalSection.Unlock();
			
			fclose(f);
		}
	}
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	lastPoint = CPoint(-1, -1);

	CWnd	*pWnd		= theApp.m_pDraggingTab,
			*pParentWnd	= NULL;

	if (pWnd)
		pParentWnd = pWnd->GetParent();

	if (pParentWnd && pParentWnd->GetSafeHwnd() == static_cast<CMainFrame*>(theApp.m_pMainWnd)->m_PropsDock.GetSafeHwnd() && 
		PropSheet_GetTabControl(pParentWnd->GetSafeHwnd()) == pWnd->GetSafeHwnd())
	{
		if (theApp.m_bDraggingTabs)
		{
			CString strString;
			pWnd->GetWindowText(strString);

			CPropertySheet* pSheet = static_cast<CPropertySheet *>(pParentWnd);
			CPropertyPage* pPage = pSheet->GetPage(pSheet->GetActiveIndex());
			pSheet->RemovePage(pPage);

			CWnd* pDialog = NULL;

			if (dynamic_cast<CGenProps*>(pPage))
			{
				pDialog = &theApp.m_pFrame->m_GenPropsDlg;
				theApp.m_pFrame->m_GenPropsDlg.Create(IDD_PARTICLE_PROPERTIES, theApp.m_pFrame);
			}
			else if (dynamic_cast<CMovementDlg*>(pPage))
			{
				pDialog = &theApp.m_pFrame->m_MovementDlg;
				theApp.m_pFrame->m_MovementDlg.Create(IDD_PARTICLE_MOVEMENT, theApp.m_pFrame);
			}
			else if (dynamic_cast<CColorDialog*>(pPage))
			{
				pDialog = &theApp.m_pFrame->m_ColorDlg;
				theApp.m_pFrame->m_ColorDlg.Create(IDD_PARTICLE_COLOR, theApp.m_pFrame);
			}
			else if (dynamic_cast<CAppearanceDlg*>(pPage))
			{
				pDialog = &theApp.m_pFrame->m_AppearanceDlg;
				theApp.m_pFrame->m_AppearanceDlg.Create(IDD_PARTICLE_APPEARANCE, theApp.m_pFrame);
			}
			else if (dynamic_cast<CViewOptionsDlg*>(pPage))
			{
				pDialog = &theApp.m_pFrame->m_ViewOptionsDlg;
				theApp.m_pFrame->m_ViewOptionsDlg.Create(IDD_VIEW_OPTIONS, theApp.m_pFrame);
			}
			else if (dynamic_cast<CParticleSystemsDlg*>(pPage))
			{
				pDialog = &theApp.m_pFrame->m_ViewOptionsDlg;
				theApp.m_pFrame->m_ParticleSystemsDlg.Create(IDD_PARTICLE_SYSTEMS, theApp.m_pFrame);
			}
			else
				ASSERT(0);

			ASSERT(pDialog);

			pDialog->SetWindowPos(NULL, point.x, point.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			pDialog->ShowWindow(SW_SHOW);

			//ReleaseCapture();
		}
	}

	CWnd::OnLButtonUp(nFlags, point);
}
