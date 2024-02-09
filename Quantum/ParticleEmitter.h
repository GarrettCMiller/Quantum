#pragma once

#include "afxmt.h"
#include "Particle.h"

#include "SpriteMgr.h"

#include "Flag.h"

#include "CollisionMgr.h"
#include "ForceMgr.h"

#include "VectorGraphics.h"

#include "CritSection.h"

//////////////////////////////////////////////////////////////////////////

#define DEFAULT_PARTICLE_VERTEX_BUFFER_FLAGS	D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS
//  extern const Vec3 kpA;
//  extern const Vec3 kpB;
//  extern Vec3 kpC;
//  extern Vec3 kpD;

//////////////////////////////////////////////////////////////////////////

class CParticleEmitter
{
	friend struct Particle;

public:
	enum EResetFlags
	{
		eResetNone,		//All Flag<>s require a NONE flag

		eResetPos,
		eResetColor,

		eResetAll,

		eResetTypeCount
	};

	//////////////////////////////////////////////////////////////////////////

	explicit CParticleEmitter(Vec3 pos = k_v3Zero);
	
	CParticleEmitter(const Particle &base)
	{
		m_pVertexBuffer = NULL;

		OneTimeInit();
		m_Base = base;
	}

	CParticleEmitter(const CParticleEmitter& rhs)
	{
		m_pVertexBuffer = NULL;

		OneTimeInit(&rhs);
	}

	CParticleEmitter& operator = (const CParticleEmitter& rhs)
	{
		OneTimeInit(&rhs);
	}
	
	~CParticleEmitter(void);

	void ReleaseAll()
	{
		m_csCriticalSection.Reset();
		SAFE_RELEASE(m_pVertexBuffer);
		activeParticles.Clear();
		freeParticles.Clear();
		//m_Platforms.ReleasePointers();
	}

	void OneTimeInit(const CParticleEmitter* pTemplate = NULL);

	bool CreateVertexBuffer();
	void InitParticles();
	void ResetActiveParticles(Flag<EResetFlags>	resetFlags = eResetPos | eResetColor);
	void SetGlobalBirthPos(Vec3 birthPos = k_v3Zero);

	void Update();

	void Render(UInt32 count, Bool Shader = false);
	Bool PreRender();

	void SetActive(UInt32 numActive, Bool goOneTime = true, Bool _active = true)
	{
		NumActiveParticles = (numActive ? numActive : NumParticles);
		m_bActive = _active;
		m_bGoOneTime = goOneTime;
	}

	void SetActive(Bool bActive = true, Bool bRender = true, Bool bReset = false)
	{
		m_bActive = m_Base.m_bActive = bActive;
		m_bRender = m_Base.m_bRender = bRender;
	
		if (bReset)
			ResetActiveParticles(eResetAll);
	}

	void SetupParticles(UINT numParticles);

	void InitEmitter();

	void Emit(Int32 nCount = -1);

	void HandleEmission();

	void DisplayBitmap(String path, Color keyColor = Color::km_Transparent);
	void RenderFromTexture(D3DTexture texture, Color keyColor = Color::km_Transparent);

	void SeekBirthplace();

	void EnableCollisions(Bool bEnable = true)
	{
		m_bApplyCollisions = bEnable;

		/*if (bEnable)
			m_RenderLists.Resize(4);
		else
			m_RenderLists.ReleaseBuffer();*/
	}

	void EnableColorBlending(Bool bEnable = true)
	{
		m_bBlendColor = bEnable;

		if (bEnable)
		{
			g_D3D->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			g_D3D->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}
		else
		{
			g_D3D->GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE);//SRCCOLOR );
			g_D3D->GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
		}
	}

	Bool ColorBlendingEnabled() const	{ return m_bBlendColor; }

	void SetTexture(UInt32 texID = 0, UInt32 texSlot = 0)
	{
		m_TexID[texSlot] = texID;
		
		//Set the texture
		g_D3D->GetDevice()->SetTexture(0, g_SpriteMgr->GetTexture(texID));
	}

	UInt32 m_NumPlatforms;

	struct TPlatform
	{
		D3DMesh		mesh;
		Vec3		pos;
		Vec3		rot;
	};

	void CreatePlatforms(UInt32 numPlatforms)
	{
		m_NumPlatforms = numPlatforms;

		m_Platforms.Resize(numPlatforms);

		for (UInt32 i = 0; i < numPlatforms; i++)
		{
			D3DXCreateBox(g_D3D->GetDevice(), 50.0f, 5.0f, 50.0f, &m_Platforms[i].mesh, NULL);
		}
	}

	void RenderPlatforms();

	void ClearParticleHistory()
	{
		for (UInt32 i = 0; i < particles.Size(); i++)
			particles[i].m_ptArr.Clear();
	}

	const Particle& GetParticle(UInt32 index) const
	{
		return particles[index];
	}

	//////////////////////////////////////////////////////////////////////////
	//The base particle is what represents the particle emitter in "particle" terms
	//All particles begin as this base particle with slightly modified velocities
	Particle m_Base;

	CVectorGraphics		m_vgParticleHistory;

	Bool	m_bInitialized;
	Bool	m_bActive;
	Bool	m_bRender;

	Bool	m_bAnchorRoot;

	//Are point sprites turned on?
	Bool	m_bPointSprites;


	//Self-explanatory
	Bool	m_bEnableTexture2;
	Bool	m_bEnableTexture3;

	Bool	m_bGoOneTime;

	//Whether or not the device supports point sprites (not fully checked against)
	static Bool	m_bDeviceSupportsPS;

	//Apply a sine wave to the radial velocity
	Bool	m_bRadialVelocityWave;
	//Apply a sine wave to the theta component of the angular velocity?
	Bool	m_bThetaVelocityWave;
	//Apply a sine wave to the phi component of the angular velocity?
	Bool	m_bPhiVelocityWave;

	Bool	m_bApplyWind;
	Bool	m_bApplyEMForce;
	Bool	m_bApplyGravity;
	Bool	m_bApplyStrongForce;

	Bool	spinA,	
			spinB,	
			spinC,	
			spinD;

	Bool	m_bApplyCollisions;

	Bool	m_bRandomizeColor;
	Bool	m_bRandomizeMass;
	Bool	m_bRandomizeCharge;

	static const Bool	bHasShader = false;

	//The total number o' particles this emitter keeps track of
	Int32		NumParticles;

	Int32		NumActiveParticles;

	Int32		m_nLifeDecay;

	//Temp iterator
	Int32		m_nCurrentParticle;

	EEmitShape EmitShape;
	EEmitDistribution	EmitDist;

	//Maximum point sprite size supported by hardware
	static Float	fMaxPointSize;

	//How far away a particle must be before polar vel/accel is applied to it
	Float	PickupRange;

	Float	EmitRate;
	Int32	EmitCount;
	Float	EmitRadius;

	Bool	bEmitPerFrame;

	Float	MinSpawnAngleTheta;
	Float	MaxSpawnAngleTheta;
	Float	MinSpawnAnglePhi;
	Float	MaxSpawnAnglePhi;

	Float	m_fColorMultiplier;

	Float	m_fTimeMultiplier;

	Vec3	BaseVel;

	Int32	Wrapmode;
	UInt32	m_Seed;
	Float	Radius;

	Vec3	pA;
	Vec3	pB;
	Vec3	pC;
	Vec3	pD;

	Float	m_fRadialVelocityWave_Freq;
	Float	m_fRadialVelocityWave_Base;
	Float	m_fRadialVelocityWave_Amp;

	Float	m_fThetaVelocityWave_Freq;
	Float	m_fThetaVelocityWave_Base;
	Float	m_fThetaVelocityWave_Amp;

	Float	m_fPhiVelocityWave_Freq;
	Float	m_fPhiVelocityWave_Base;
	Float	m_fPhiVelocityWave_Amp;

	CDynamicArray<TPlatform>	m_Platforms;

	String	m_strName;

	//CCriticalSection			m_csCriticalSection;
	CCritSection			m_csCriticalSection;

private:

	UInt32						m_dwParticleUniqueID;
	//The vertex buffer that holds the point sprite vertices
	LPDIRECT3DVERTEXBUFFER9		m_pVertexBuffer;

public:
	CCollisionMgr<Particle>		m_CMgr;
	CForceMgr<Particle>			m_ForceMgr;

private:

	//typedef DblLinkedList<DWORD>	TParticleList;
	typedef CDynamicArray<UInt32>	TParticleList;

	//The actual particles
	CDynamicArray<Particle>		particles;
	TParticleList				activeParticles;
	TParticleList				freeParticles;

	CDynamicArray<Particle*>	sortedParticles;

	CDynamicArray<TParticleList >	m_RenderLists;

	//CHashTable<Particle*>		m_Lists;

	//The id's for the sprites/textures being loaded
	CDynamicArray<Int32>	m_TexID;

	Bool	m_bBlendColor;

	//HACK
public:
	UInt32	_id;

};
