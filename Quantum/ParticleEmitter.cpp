#include "StdAfx.h"

#include "ParticleEmitter.h"

#include "MiscMath.h"

#include "D3DWrapper.h"

#include "EffectMgr.h"

#include "ProfileSample.h"

//////////////////////////////////////////////////////////////////////////

const Vec3 kpA(1.5f, 1.0f, 0.0f);
const Vec3 kpB(-1.5f, 1.0f, 0.0f);
const Vec3 kpC(0.0f, -1.0f, 0.0f);
const Vec3 kpD(-1.0f, -1.0f, 0.0f);

static TVertexArray kVerts;

Bool CParticleEmitter::m_bDeviceSupportsPS = false;
Float CParticleEmitter::fMaxPointSize = 50.0f;

D3DMesh k_CenterOfMass;

static Bool kb_UseSprites = false;

static HRESULT hr;
//////////////////////////////////////////////////////////////////////////

CParticleEmitter::CParticleEmitter(Vec3 pos /* = k_v3Zero */) 
	:	m_bPointSprites(kb_UseSprites), 
		m_bActive(false), 
		m_bRandomizeColor(false),
		m_Seed(0),
		m_fColorMultiplier(1.0f),
		m_bRandomizeMass(false),
		m_bRandomizeCharge(false),
		m_fTimeMultiplier(0.5f),
		m_bAnchorRoot(false),
		_id(0)
{
	m_bEnableTexture2	= false;
	m_bEnableTexture3	= false;

	NumParticles		= 1000;
	m_Base				= Particle(pos, k_v3Zero, Color::km_White);
	m_Base.m_pParent	= this;

	PickupRange			= 0;

	m_bThetaVelocityWave			= false;
	m_bRadialVelocityWave			= false;

	m_pVertexBuffer			= NULL;

	EmitRadius				= 0.0f;
	EmitRate				= 0;

	Wrapmode				= -1;
	Radius					= 1.0f;

	MinSpawnAngleTheta		= 0;
	MaxSpawnAngleTheta		= 360.0f;

	MinSpawnAnglePhi		= 0;
	MaxSpawnAnglePhi		= 180.0f;

	spinA				= true;
	spinB				= true;
	spinC				= true;
	spinD				= true;

	EmitShape			= Emit_Spherical;

	m_bApplyCollisions	= false;

	m_bGoOneTime		= false;

	m_bApplyGravity		= false;
	m_bApplyWind		= false;
	m_bApplyEMForce		= false;

	m_RenderLists.Resize(4);
	m_TexID.Resize(3);
	m_TexID[0] = -1;

	m_bInitialized = false;

	//particles.Resize(MAX_PARTICLES);
}

//////////////////////////////////////////////////////////////////////////

void CParticleEmitter::OneTimeInit(const CParticleEmitter* pTemplate /* = NULL */)
{
	if (pTemplate)
	{
		ReleaseAll();

		m_Base = pTemplate->m_Base;

		m_bInitialized			= pTemplate->m_bInitialized;
		m_bActive 				= pTemplate->m_bActive;
		m_bRender 				= pTemplate->m_bRender;

		m_bPointSprites			= pTemplate->m_bPointSprites;

		EmitDist				= pTemplate->EmitDist;

		m_bGoOneTime			= pTemplate->m_bGoOneTime;

		m_bThetaVelocityWave	= pTemplate->m_bThetaVelocityWave;
		m_bRadialVelocityWave	= pTemplate->m_bRadialVelocityWave;

		m_bApplyGravity			= pTemplate->m_bApplyGravity;
		m_bApplyWind			= pTemplate->m_bApplyWind;

		m_bApplyCollisions		= pTemplate->m_bApplyCollisions;

		spinA 					= pTemplate->spinA;
		spinB 					= pTemplate->spinB;
		spinC 					= pTemplate->spinC;
		spinD 					= pTemplate->spinD;

		m_bRandomizeColor		= pTemplate->m_bRandomizeColor;

		NumParticles			= pTemplate->NumParticles;

		NumActiveParticles		= 0;

		m_nLifeDecay			= pTemplate->m_nLifeDecay;

		m_nCurrentParticle		= 0;

		EmitShape				= pTemplate->EmitShape;

		PickupRange				= pTemplate->PickupRange;

		EmitRate				= pTemplate->EmitRate;
		EmitCount				= pTemplate->EmitCount;
		EmitRadius				= pTemplate->EmitRadius;

		bEmitPerFrame			= pTemplate->bEmitPerFrame;

		MinSpawnAngleTheta		= pTemplate->MinSpawnAngleTheta;
		MaxSpawnAngleTheta		= pTemplate->MaxSpawnAngleTheta;

		BaseVel					= pTemplate->BaseVel;

		Wrapmode				= pTemplate->Wrapmode;
		Radius					= pTemplate->Radius;

		pA 						= pTemplate->pA;
		pB 						= pTemplate->pB;
		pC 						= pTemplate->pC;
		pD 						= pTemplate->pD;

		m_strName				= pTemplate->m_strName;
	}
	else
	{
		m_bEnableTexture2		= false;
		m_bEnableTexture3		= false;

		NumParticles			= 1000;
		m_Base					= Particle(k_v3Zero, k_v3Zero, Color::km_White);
		m_Base.m_pParent		= this;

		PickupRange				= 10;

		m_bThetaVelocityWave	= false;
		m_bRadialVelocityWave	= false;

		EmitRadius				= 0.0f;
		EmitRate				= 0;

		Wrapmode				= -1;
		Radius					= 1.0f;

		MinSpawnAngleTheta		= 0;
		MaxSpawnAngleTheta		= 360.0f;

		spinA					= true;
		spinB					= true;
		spinC					= true;
		spinD					= true;

		EmitShape				= Emit_Spherical;

		m_bApplyCollisions		= false;

		m_bGoOneTime			= false;

		m_bApplyGravity			= false;
		m_bApplyWind			= false;
	}

	m_RenderLists.Resize(4, false);
	m_TexID.Resize(3);
	m_TexID[0] = -1;

	m_bInitialized = false;
}

///////////////////////////////////////////////////////////////////////////

CParticleEmitter::~CParticleEmitter(void)
{
	ReleaseAll();
}

//////////////////////////////////////////////////////////////////////////

//Renders the point sprites every frame
Bool CParticleEmitter::PreRender()
{
	if (!m_bInitialized || !m_bRender /*|| !m_bActive*/)
		return false;

	PROFILE_THIS;

	LPDIRECT3DDEVICE9	dev = g_D3D->GetDevice();

	Effect& fx = g_EffectMgr->GetEffect("Particle.fx");
	UInt32 np = 0;

	D3DMatrix mat;
	mat = g_D3D->matView * g_D3D->matProj;

	fx.Shader->SetTechnique("Particle");
	fx.Shader->SetMatrix("matViewProj", &mat);
	fx.Shader->SetVector("g_vLightPos", &Vec4(150.0f, 150.0f, 150.0f, 0.0f));
	fx.Shader->SetVector("g_cAmbient", &Vec4(0.2f, 0.2f, 0.2f, 1.0f));

	if (m_bApplyCollisions)
	{
		dev->SetRenderState(D3DRS_POINTSIZE, ReinterpretCast<DWORD>(1.0f));

		dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		//dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		fx.Shader->SetTechnique("ParticleHistory");

		fx.Shader->Begin(&np, 0);
		fx.Shader->BeginPass(0);

		//m_vgParticleHistory.DrawTriangleFan(kVerts);
		//m_vgParticleHistory.DrawTriangleList(kVerts);
		//m_vgParticleHistory.DrawTriangleStrip(kVerts);

		Particle* current = NULL;
		Color dcolor = Color::km_Black;

		static CDynamicArray<Float> distances(particles.Size());

		//UInt32 i = _id;
		for (UInt32 i = 0; i < particles.Size(); i++)
		{
			current = &(particles[i]);

			//for (UInt32 other = /*i + 1*/0; other < particles.Size(); other++)
			//{
			//	if (i == other)
			//		continue;

			//	Float d = Distance(current->m_vPos, particles[other].m_vPos);
			//	
			//	if (d > distances[other])
			//	{
			//		dcolor.ConvertFromHSV(Clamp((d - distances[other]) * 0.125f, 0.0f, 1.0f) * 360.0f, 1.0f, 1.0f);
			//		//dcolor.SetRed();
			//	}
			//	else
			//	{
			//		dcolor = Color::km_Blue;
			//	}

			//	distances[other] = d;

			//	m_vgParticleHistory.CreateLine(current->m_vPos, particles[other].m_vPos, dcolor);
			//}

			if (!current->m_ptArr.IsEmpty())
			{
				m_vgParticleHistory.DrawLineStrip(current->m_ptArr, false);
			}
		}

		fx.Shader->EndPass();
		fx.Shader->End();

		//EnableColorBlending(m_bBlendColor);
	}

	if (m_bPointSprites)
	{
		//Set the texture
		dev->SetTexture(0, g_SpriteMgr->GetTexture(m_TexID[0]));

		for (Int32 renderPass = 0; renderPass < m_RenderLists.Size(); renderPass++)
		{
			PROFILE("CParticleEmitter::PreRender() - RenderList loop");

			if (m_RenderLists[renderPass].IsEmpty())
				continue;

			{
				PROFILE("CParticleEmitter::PreRender() - Allocations");

				CVertex*	pv = NULL;

				TParticleList& list = m_RenderLists[renderPass];	//freeParticles;//activeParticles;

				static Bool bDrawBase = false;

				UINT np = min(static_cast<UInt32>(NumParticles), list.NumItems()) + (bDrawBase ? 1 : 0);

				//np += np * (particles[0].m_History.Size() - 1);

				//Lock the vertex buffer for vertex adjustment
				{
					PROFILE("CParticleEmitter::PreRender() - LockBuffer");

					if (FAILED(m_pVertexBuffer->Lock(0, np * sizeof(CVertex), (void**)&pv, D3DLOCK_DISCARD)))
					{
						MessageBox(NULL, "Can't lock buffer!", "", MB_OK);
						return false;
					}
				}

	// 			if (bDrawBase)
	// 			{
	// 				pv->color		= m_Base.m_cColor;
	// 				pv->position	= m_Base.m_vPos;
	// 
	// 				pv++;
	// 			}

				Particle* current = NULL;

				{
					PROFILE("CParticleEmitter::PreRender() - pre-Particle Setup Loop");
					//Run through the particle list
					for (UInt32 p = /*bDrawBase ? 1 :*/ 0; p < np /*&& !pt.End()*/; p++, pv++ /*, ++pt*/)
					{
						PROFILE("CParticleEmitter::PreRender() - Particle Setup Loop");

						current = sortedParticles[p];//&(particles[p]);

						//Set up the current "vertex" for the point sprite
						pv->color		=	D3DXCOLOR(current->m_cColor);
						//D3DXCOLOR(current->m_cColor.Red(), current->m_cColor.Green(), current->m_cColor.Blue(), current->m_cColor.Alpha());
						pv->position	=	current->m_vPos;	
						//D3DXVECTOR3(current->m_vPos.x, current->m_vPos.y, current->m_vPos.z);

						//if (current->m_dwUniqueID == 0 && 
						//		//!current->m_ptArr.IsEmpty()
						//		current->m_ptArr.Size() > 10
						//		)
						//{
						//	//m_vgParticleHistory.DrawLineStrip(current->m_ptArr, false);
						//	m_vgParticleHistory.CreateLine(current->m_vPos, k_v3Zero);
						//}
					}
				}

				{
					PROFILE("CParticleEmitter::PreRender() - UnlockBuffer");
					m_pVertexBuffer->Unlock();
				}

				{
					PROFILE("CParticleEmitter::PreRender() - SetRenderState");
					static UInt32 listIndex = 0;
					listIndex = list[0];
					dev->SetRenderState(D3DRS_POINTSIZE, ReinterpretCast<DWORD>(particles[listIndex].m_fSize));
				}

				{
					PROFILE("CParticleEmitter::PreRender() - Render");
					Render(np, bHasShader);
				}

				{
					//PROFILE("CParticleEmitter::PreRender() - List Clear");
					list.Clear();
					sortedParticles.Clear();
				}
			}
		}

		g_D3D->GetDevice()->SetTexture(0, NULL);
	}
	else
	{
		fx.Shader->SetTechnique("Particle");
		fx.Shader->Begin(&np, 0);

		for (UInt32 p = 0; p < NumParticles; p++)
		{
			particles[p].Render(&fx);
		}

		fx.Shader->End();
	}

	//g_D3D->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

// 	if (m_bSpecialBlendMode)
// 	{
// 		g_D3D->GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE);//SRCCOLOR );
// 		g_D3D->GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
// 	}

	//	g_D3D->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	//	g_D3D->GetDevice()->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	//	g_D3D->GetDevice()->SetRenderState(D3DRS_POINTSCALEENABLE, false);

	//g_D3D->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);

	Vec3 pos = m_ForceMgr.m_vCenterOfMass;
	
	/*D3DXMatrixTranslation(&mat, pos.x, pos.y, pos.z);
	g_D3D->SetWorldMatrix(mat);*/

	g_D3D->SetWorldMatrix(mat);

	static Bool bFilled = false;
	static const Float fLen = 100.0f;
	static CVectorGraphics::TPointArray arrCenter;
	
	if (!bFilled)
	{
		arrCenter.Push(CVertex(Vec3(-fLen, 0.0f, 0.0f)));
		arrCenter.Push(CVertex(Vec3(fLen, 0.0f, 0.0f)));

		arrCenter.Push(CVertex(Vec3(0.0f, -fLen, 0.0f)));
		arrCenter.Push(CVertex(Vec3(0.0f, fLen, 0.0f)));

		arrCenter.Push(CVertex(Vec3(0.0f, 0.0f, -fLen)));
		arrCenter.Push(CVertex(Vec3(0.0f, 0.0f, fLen)));
		
		bFilled = true;
	}
	
	m_vgParticleHistory.DrawLineList(arrCenter);
	//k_CenterOfMass->DrawSubset(0);

	//RenderPlatforms();

	D3DXMatrixIdentity(&mat);
	g_D3D->SetWorldMatrix(mat);

	//RenderPlatforms();

	return true;
}

void CParticleEmitter::Render(UInt32 count,Bool /*Shader*/ /* = false */)
{
	PROFILE_THIS;

	//Set which vertex buffer to run through for rendering
	if (FAILED(hr = g_D3D->GetDevice()->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(CVertex))))
	{
		g_Log.LOG_ERROR_DX_DESC(hr, "SetStreamSource");
		return;
	}
	//if (Shader)
	//{
	//	if (SUCCEEDED(g_D3D->lpShader->SetTechnique(g_D3D->hTech2))) 
	//	{
	//		UINT numPasses;
	//		g_D3D->lpShader->Begin(&numPasses, 0);

	//		for (UINT i = 0; i < numPasses; i++) 
	//		{
	//			g_D3D->lpShader->BeginPass(i); // Set the pass

	//			//Draw the point sprites
	//			g_D3D->GetDevice()->DrawPrimitive(D3DPT_POINTLIST, 0, count);

	//			g_D3D->lpShader->EndPass();
	//		}

	//		g_D3D->lpShader->End();
	//	}
	//}
	//else
	//{
	//Draw the point sprites
	if (FAILED(hr = g_D3D->GetDevice()->DrawPrimitive(D3DPT_POINTLIST, 0, count)))
	{
		g_Log.LOG_ERROR_DX_DESC(hr, "DrawPrim");
		return;
	}

	//}
}

//////////////////////////////////////////////////////////////////////////

void CParticleEmitter::InitParticles()
{
	PROFILE_THIS;

	activeParticles.Clear();
	freeParticles.Clear();

	Particle::ms_dwUniqueID = 0;

	Particle curr;
	UInt32 numParticles = particles.Size();

	for (UInt32 i = 0; i < numParticles; i++)
	{
		curr.m_pParent = m_Base.m_pParent;
		curr.m_dwUniqueID = m_dwParticleUniqueID++;
		//ASSERT(curr.m_dwUniqueID < 5000);
		curr.Init();
		//curr.InitMesh();
		particles.Push(curr);
		freeParticles.Push(i);
	}
}

//////////////////////////////////////////////////////////////////////////

void CParticleEmitter::SetupParticles(UINT numParticles)
{
	NumParticles = numParticles;

	m_dwParticleUniqueID	= 0;

	m_bRender				= m_bActive;

	m_Base.m_bActive		= m_bActive;
	m_Base.m_bRender		= m_bRender;
	activeParticles.		Resize(numParticles);
	freeParticles.			Resize(numParticles);
	particles.				Resize(NumParticles);

	UInt32 numLists			= m_RenderLists.Size();

	for (UInt32 i = 0; i <	numLists; i++)
	{
		m_RenderLists[i].	Resize(numParticles);
		m_RenderLists[i].	SetToEmpty();
	}

	InitParticles();
}

//////////////////////////////////////////////////////////////////////////

void CParticleEmitter::ResetActiveParticles(Flag<EResetFlags> resetFlags /* = eResetPos | eResetColor */)
{
	PROFILE_THIS;

	Particle* pParticle = NULL;

	TParticleList& list = freeParticles;

	Bool bAll	= resetFlags.TestFlag(eResetAll);

	srand(m_Seed);

	for (Int32 i = 0; i < list.NumItems(); i++)
	{
		pParticle = &(particles[list[i]]);

		if (bAll)
		{
			pParticle->Init();
		}
		else
		{
			Bool	bVel	= resetFlags.TestFlag(eResetPos),
					bColor	= resetFlags.TestFlag(eResetColor);

			if (bVel)
			{
				//pParticle->m_vVel = m_Base.m_vVel * Random(m_Base.m_vVel.X, m_Base.m_vVel.X * 0.1f);
				pParticle->m_vAccel = m_Base.m_vAccel;
				pParticle->m_vVel.SetSpeedDir(Random(m_Base.m_vPolarVel.x, m_Base.m_vPolarVel.x * 0.1f), Random() * 359.0f, Random() * 179.0f);
			}

			if (bColor)
			{
				pParticle->m_cColor = m_Base.m_cColor;
				pParticle->m_cDecay = m_Base.m_cDecay;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void CParticleEmitter::SetGlobalBirthPos(Vec3 birthPos /* = k_vZero */)
{
	TParticleList& list = activeParticles;
	Particle* pParticle = NULL;

	for (Int32 i = 0; i < list.NumItems(); i++)
		particles[list[i]].m_vBirthPos = birthPos;
}

//////////////////////////////////////////////////////////////////////////

void CParticleEmitter::InitEmitter()
{
	PROFILE_THIS;

	ASSERT(!m_pVertexBuffer);

	LPDIRECT3DDEVICE9	dev = g_D3D->GetDevice();

	EnableColorBlending(false);

	//Set all point sprite flags
	dev->SetRenderState(D3DRS_ZWRITEENABLE,			false);
	dev->SetRenderState(D3DRS_POINTSPRITEENABLE,	true);
	dev->SetRenderState(D3DRS_POINTSCALEENABLE,		true);
	dev->SetRenderState(D3DRS_POINTSIZE_MIN,		ReinterpretCast<DWORD>(1.0f));
	dev->SetRenderState(D3DRS_POINTSCALE_A,			ReinterpretCast<DWORD>(0.0f));
	dev->SetRenderState(D3DRS_POINTSCALE_B,			ReinterpretCast<DWORD>(0.0f));
	dev->SetRenderState(D3DRS_POINTSCALE_C,			ReinterpretCast<DWORD>(1.0f));

	CreateVertexBuffer();

	m_vgParticleHistory.Init();

	CreatePlatforms(1);

	fMaxPointSize = 0.0f;
	m_bDeviceSupportsPS = false;

	D3DCAPS9 d3dCaps;
	dev->GetDeviceCaps(&d3dCaps);

	//Check if hardware supports point g_SpriteMgr
	if (d3dCaps.FVFCaps & D3DFVFCAPS_PSIZE)
		m_bDeviceSupportsPS = true;
	else
		m_bDeviceSupportsPS = false;

	fMaxPointSize = d3dCaps.MaxPointSize;

	////////////////////////////////////////////////////

	//Set the default texture (if one hasn't been set)
	//if (m_TexID[0] < 0)
		//SetTexture(g_SpriteMgr->LoadSprite("Particle.bmp"));

	m_Base.m_vVel = k_v3Zero;
	m_Base.m_pParent = this;

	SetupParticles(NumParticles);

	m_nLifeDecay	= 1;

	PickupRange		= 10;

	pA				= kpA;
	pB				= kpB;
	pC				= kpC;
	pD				= kpD;

	m_bInitialized	= true;
}

//////////////////////////////////////////////////////////////////////////

bool CParticleEmitter::CreateVertexBuffer()
{
	HRESULT hr;

	SAFE_RELEASE(m_pVertexBuffer);

	//Create the vertex buffer
	hr = g_D3D->GetDevice()->CreateVertexBuffer(MAX_PARTICLES * sizeof(CVertex), 
											DEFAULT_PARTICLE_VERTEX_BUFFER_FLAGS, 
											CVertex::FVF, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);

	SAFE_RELEASE(k_CenterOfMass);

	D3DXCreateSphere(g_D3D->GetDevice(), 1.0f, 10, 10, &k_CenterOfMass, NULL);

	return !FAILED(hr);
}

//////////////////////////////////////////////////////////////////////////

void CParticleEmitter::Update()
{
	//static unsigned __int64 frameCounter = 0;
	//frameCounter++;

	PROFILE_THIS;

	if (!m_bActive)
		return;

	//Protect against bad pointers to the parent (I've had my share of these)
	//ASSERT(!BAD_PTR(m_Base.m_pParent));

	// 	if (m_bActive && NumActiveParticles <= 0)
	// 	{
	// 		m_bActive = false;
	// 		return;
	// 	}

	//HandleEmission();

	//Emit(EmitRate);

	/*if (m_bApplyCollisions)
		m_CMgr.Process();*/

	TParticleList& list = freeParticles;

	for (m_nCurrentParticle = 0; 
		m_nCurrentParticle < NumParticles && m_nCurrentParticle < list.NumItems(); 
		m_nCurrentParticle++)
	{
		//PROFILE_NO_HELPER("ParticleEmitter::Update() - Main Loop");
		//pSamp.Start();

		//Particle::ParticlesSoFar++;

		// 		if (Particle::TooManyParticles())
		// 			return;
		Int32 index = list[m_nCurrentParticle];
		ASSERT(index >= 0);
		Particle& particle = particles[index];

		particle.m_bActive = m_bActive;
		particle.m_bRender = m_bRender;

		particle.Update();

		//pSamp.Stop();
	}

	if (m_bApplyGravity)
		m_ForceMgr.Process(eForce_Gravity);

	if (m_bApplyEMForce)
		m_ForceMgr.Process(eForce_Electric);

	m_ForceMgr.Empty();
}

//////////////////////////////////////////////////////////////////////////

void CParticleEmitter::HandleEmission()
{
	Emit(-1);
}

//////////////////////////////////////////////////////////////////////////

void CParticleEmitter::Emit(Int32 nCount /* = -1 */)
{
	Particle* pp = NULL;
	UInt32 id = 0;

	Int32 numToSpawn = nCount == -1 ? EmitRate : nCount;

	for (EmitCount = 0; 
		EmitCount < numToSpawn && freeParticles.NumItems() > 0; 
		EmitCount++)
	{
		id = freeParticles[0];
		freeParticles.Pop(id);

		pp = &(particles[id]);

		pp->Init();
		activeParticles.Push(id);
	}
}

//////////////////////////////////////////////////////////////////////////

void CParticleEmitter::DisplayBitmap(String path, Color keyColor /* = Color::km_Transparent */)
{
	Sprite& image = g_SpriteMgr->GetSprite(path);
	RenderFromTexture(image.texture, keyColor);
}

void CParticleEmitter::RenderFromTexture(D3DTexture texture, Color keyColor /* = Color::km_Transparent */)
{
	static const Float gapsize = m_Base.m_fSize * 1.0f;
	static const Float xGap = gapsize, yGap = gapsize;

	LPDIRECT3DSURFACE9 surf;
	HDC hdc;

	if (FAILED(hr = texture->GetSurfaceLevel(0, &surf)))
	{
		g_Log.LOG_ERROR_DX_DESC(hr, "Couldn't get surface!");
		return;
	}

	if (FAILED(hr = surf->GetDC(&hdc)))
	{
		g_Log.LOG_ERROR_DX_DESC(hr, "Couldn't get DC!");
		return;
	}

	D3DSURFACE_DESC desc;
	surf->GetDesc(&desc);

	Int32	width = NextLargestPowerOfTwo(desc.Width),
			height = NextLargestPowerOfTwo(desc.Height),
			color = 0;

	Float	halfwidth = width / 2,
			halfheight = height / 2;

	for (Int32 y = 0; y < height; y++)
	{
		for (Int32 x = 0; x < width; x++)
		{
			color = GetPixel(hdc, x, y);

			if (keyColor == Color::km_Transparent || keyColor == color)
			{
				m_Base.m_vPos = Vec3((x - halfwidth) * xGap, (y - halfheight) * yGap, 0.0f);
				m_Base.m_cColor = color;

				Emit(1);
			}
		}
	}

	m_Base.m_vPos = k_v3Zero;

	surf->ReleaseDC(hdc);
	surf->Release();

#if 0

	DblLinkedList<Particle*>::Iterator	i(activeParticles);
	Particle* pParticle = NULL;

	Vec2	screenRes(500, 500);

	for (i.Begin(); !i.End(); ++i)
	{
		pParticle = i.Current();

		pParticle->m_vPos.RandomUnitRectangle();
		pParticle->m_vPos *= screenRes;
	}

#endif
}

//////////////////////////////////////////////////////////////////////////

void CParticleEmitter::SeekBirthplace()
{
	TParticleList& list = activeParticles;
	Particle* pParticle = NULL;

	for (Int32 i = 0; i < list.NumItems(); i++)
	{
		pParticle = &(particles[list[i]]);

		pParticle->m_bSeeking = true;
		pParticle->m_vVel = pParticle->m_vTargetPos - pParticle->m_vPos;
		//pParticle->m_vVel.Normalize();
		pParticle->m_vVel *= 0.07f;//5;
	}
}



void CParticleEmitter::RenderPlatforms()
{
	D3DMatrix mat, pos;
	TPlatform* platform = NULL;

	Effect& fx = g_EffectMgr->GetEffect("Particle.fx");

	mat = g_D3D->matView * g_D3D->matProj;

	fx.Shader->SetTechnique("Platform");
	fx.Shader->SetMatrix("matViewProj", &mat);

	UInt32 np = 0;

	fx.Shader->Begin(&np, 0);

	D3DXMatrixIdentity(&mat);

	for (UInt32 i = 0; i < m_NumPlatforms; i++)
	{
		platform = &m_Platforms[i];

		/*D3DXMatrixTranslation(&pos, platform->pos.x, platform->pos.y, platform->pos.z);
		D3DXMatrixRotationAxis(&mat, &k_v3ZAxis, 30.0f * Deg2Rad);

		mat *= pos;

		g_D3D->SetWorldMatrix(mat);*/

		fx.Shader->BeginPass(0);

		platform->mesh->DrawSubset(0);

		fx.Shader->EndPass();
	}

	fx.Shader->End();
}