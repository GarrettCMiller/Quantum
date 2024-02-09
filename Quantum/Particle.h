///////////////////////////////////////////////////////////////////////////////
//		File:				Particle.h
//
//		Programmer:			Garrett Miller (GM)
//
//		Date created:		September 10, 2004
//
//		Description:		A particle and particle emitter class
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "stdafx.h"

#include "D3DWrapper.h"
#include "Vec.h"
#include "Queue.h"
#include "MiscMath.h"
#include "Timer.h"
#include "Palette.h"
#include "DynamicArray.h"

#include "VectorGraphics.h"

#include "CritSection.h"

//////////////////////////////////////////////////////////////////////////

#define MAX_LIFE			50								//Actually the initial max for particle life span
#define ALPHA_BLEND			127								//Initial alpha value for bitmapped particles
#define MAX_TEXTURES		3								//Max textures for bitmapped particles
#define MAX_PARTICLES		100000/*1024*1024*/						//Absolute max for particles (be careful using near this amount!)
#define COLOR_ADJ			255.0f							//Divide by this value for the D3DCOLOR struct

#define NUM_PARTICLES		200
#define NUM_EMITTERS		5

//////////////////////////////////////////////////////////////////////////

//Globals
extern Vec3 g_vGravity;
extern Vec3 g_vWind;

//Forward declaration
class CParticleEmitter;
class Effect;
//////////////////////////////////////////////////////////////////////////

enum EEmitShape
{
	Emit_Spherical,
	Emit_Rectangular,
	Emit_Arc,
	//Emit_Curve,
	//Emit_Curve2,
	//Emit_Curve3,

	Emit_HighSphere,
	Emit_FourSpheres,
	Emit_GalaxyDisk,

	Emit_TwoSpheres,

	Emit_Count
};

enum EEmitDistribution
{
	eED_Boundary,
	eED_Even,
	eED_Random,

	eED_Count
};

//Our particle class
struct Particle
{
	struct CritSection : public CCritSection
	{
		CritSection() : CCritSection(), m_pTarget(NULL) {	}
		~CritSection()					{	}

		void Lock(Particle* pThis, Particle* pTarget = NULL)
		{
			SetTarget(pTarget);

			if (pTarget)
			{
				/*while (pTarget->m_CritSection.IsLocked())
				{
					Sleep(10);
				}*/
			}

			CCritSection::Lock();
		}

		void SetTarget(Particle* pTarget)
		{
			m_pTarget = pTarget;
		}

		void Unlock()
		{
			SetTarget(NULL);

			CCritSection::Unlock();
		}

		Particle* m_pTarget;
	} 
		m_CritSection;

	//The emitter from whence it comes
	CParticleEmitter*	m_pParent;

	UInt32			m_dwUniqueID;
	static UInt32	ms_dwUniqueID;

	static const bool		mk_bDoPolarCoords = true;

	//The position
	Vec3		m_vPos;
	Vec3		m_vBirthPos;

	Vec3		m_vTargetPos;

	Vec3		m_vForceSum;
	
	//The velocity
	Vec3		m_vVel;
	//The polar velocity (Radial and angular velocity)
	Vec3		m_vPolarVel;
	
	//The acceleration
	Vec3		m_vAccel;
	//The polar acceleration
	Vec3		m_vPolarAccel;

	Vec3		m_vAccelDecay;

	Color		m_cColor;		//Color and transparency
	Color		m_cDecay;		//Color and transparency decay rates
				
	Int32		m_nLife,		//Life span of particle
				m_nMinLife,
				m_nMaxLife;

	Float		m_fRotSpeed;	//Speed of that rotation
	Float		m_fRotation;	//Actual rotation of the bitmap (bitmapped particles only)

	Float		m_fSize;		//Radius/size
	Float		m_fShrinkRate;	//Rate of size decay (bitmapped particles only)

	Vec2		m_fInitAngles;

	Float		m_fDelay;
	Bool		m_bReady;
	Bool		m_bAutoReset;
	Bool		m_bSeeking;
	Bool		m_bActive;
	Bool		m_bRender;
	Timer		m_tDelayTimer;

	Bool	m_bElasticCollision;

	//Physics
	Float		m_fMass;
	Float		m_fElecCharge;
	Float		m_fKineticEnergy;
	Float		m_fPotentialEnergy;
	//Vec3		m_v

	D3DMesh		m_Mesh;

	struct ParticleState
	{
		Vec3	pos;
		Vec3	vel;
		Float	kineticEnergy;
		Float	potentialEnergy;

		Bool operator == (const ParticleState& rhs) const
		{
			return (memcmp(this, &rhs, sizeof(ParticleState)) == 0);
		}

		Bool operator != (const ParticleState& rhs) const
		{
			return !((*this) == rhs);
		}
	};

	enum EColorMode
	{
		eCM_Custom,

		eCM_Mass,

		eCM_Charge,
		
		eCM_Velocity,
		eCM_Acceleration,
		
		eCM_Momentum,

		eCM_KineticEnergy,
		eCM_PotentialEnergy
	};

	static EColorMode	mk_eColorMode ;//=		//eCM_Velocity;
												//eCM_Mass;
												//eCM_Charge;
												//eCM_KineticEnergy;
												//eCM_Momentum;

	//CDynamicArray<ParticleState>	m_History;
	Queue<ParticleState>	m_History;
	CVectorGraphics::TPointArray				m_ptArr;

	//////////////////////////////////////////////
	//////////////////////////////////////////////

	//Default constructor
	Particle(	Vec3 pos	= Vec3(0, 0, 0), 
				Vec3 vel	= Vec3(Random(1.0f, -1.0f), Random(1.0f, -1.0f), Random(1.0f, -1.0f)), 
				Color color	= Color(static_cast<UInt8>(Random(255)), static_cast<UInt8>(Random(255)), static_cast<UInt8>(Random(255))));

	~Particle()
	{
		SAFE_RELEASE(m_Mesh);
	}

	//Sets the particle back to its initial condition for re-emission
	void Reset();

	void Init();

	//The one and only handling function
	void Update();

	//Bitmapped particles only
	void Draw();

	//Creates a random particle
	void Randomize();

	Bool IsReady()						{	return m_bReady;	}

	Bool Collision(Particle* pOther);

	Int32 OnCollision(Particle* pOther);

	Float DistanceToCamera();

	void DoLife();
	void DoMovement();
	void DoPolarCoords();
	void DoRotation();
	void DoColor();

	Bool InitMesh();
	void Render(Effect* pFX = NULL);
	void RenderMesh();

	Color PhysicsColoring(const ParticleState& state);

	static const Int32 ParticleMax = MAX_PARTICLES;
	static Int32 ParticlesSoFar;

	static void ResetParticleCount() 	{	ParticlesSoFar = 0;	}
	static Bool TooManyParticles()		{	return ParticlesSoFar >= ParticleMax;	}
};

//////////////////////////////////////////////////////////////////////////