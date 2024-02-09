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

#include "D3D.h"	//D3D wrapper
#include "Vec.h"
#include "MiscMath.h"
#include "Timer.h"

//////////////////////////////////////////////////////////////////////////

#define MAX_LIFE			50								//Actually the initial max for particle life span
#define ALPHA_BLEND			127								//Initial alpha value for bitmapped particles
#define MAX_TEXTURES		3								//Max textures for bitmapped particles
#define MAX_PARTICLES		1000							//Absolute max for particles (be careful using near this amount!)
#define COLOR_ADJ			255.0f							//Divide by this value for the D3DCOLOR struct

#define Cam3DWidth			100.0f							//I'm still getting the 3D camera stuff down

//SCW & SCH defined in "stdafx.h" (screen width/height)
#define XTo3D(x)			((Cam3DWidth / SCW) * x - (Cam3DWidth / 2))		//Converting the particle's 2d x coord to a 3d coord for point sprite rendering
#define YTo3D(y)			((Cam3DWidth / SCH) * y - (Cam3DWidth / 2))		//Converting the particle's 2d y coord to a 3d coord for point sprite rendering

//////////////////////////////////////////////////////////////////////////

//Globals
extern Vec2 Gravity;
extern Vec2 Wind;

//Forward declaration
struct ParticleEmitter;

//////////////////////////////////////////////////////////////////////////

//Our particle class
struct Particle
{
	//The emitter from whence it comes
	ParticleEmitter*	parent;

	//The id's for the sprites/textures being loaded
	int			TexID[MAX_TEXTURES];

	//The position
	Vec2		Pos;
	
	//The velocity
	Vec2		Vel;
	//The polar velocity (Radial and angular velocity)
	Vec2		PolarVel;
	
	//The acceleration
	Vec2		Accel;
	//The polar acceleration
	Vec2		PolarAccel;

	int			Alpha,				//Color and transparency
				Red,					
				Green,				
				Blue;
	
	int			AlphaDecay,			//Color and transparency decay rates
				RedDecay,
				GreenDecay,
				BlueDecay;

	int			Life;				//Life span of particle

	int			RotSpeed;			//Speed of that rotation
	float		Rot;				//Actual rotation of the bitmap (bitmapped particles only)

	float		Size;				//Radius/size
	float		ShrinkRate;			//Rate of size decay (bitmapped particles only)


	float		fDelay;
	bool		bReady;
	Timer		DelayTimer;

	//Default constructor
	Particle(	float x = 0, float y = 0, float xv = Random(1.0f, -1.0f), float yv = Random(1.0f, -1.0f), 
				float size = 1, int red = Random(255), int green = Random(255), int blue = Random(255), int alpha = ALPHA_BLEND);

	//Sets the particle back to its initial condition for re-emission
	void Reset();

	//The one and only handling function
	void Handle();

	//Bitmapped particles only
	void Draw();

	//Creates a random particle
	void Randomize();

	bool IsReady()			{	return bReady;	}

	static const int ParticleMax = MAX_PARTICLES;
	static int ParticlesSoFar;

	static void ResetParticleCount() 	{	ParticlesSoFar = 0;	}
	static bool TooManyParticles()		{	return ParticlesSoFar >= ParticleMax;	}
};

//////////////////////////////////////////////////////////////////////////

//The source of particle emission
struct ParticleEmitter
{
	//////////////////////////////////////////////////////////
	// Function:			constructor
	//
	// Last modified:	September 14, 2004
	//
	// Input:				x					-	the x position of the emitter
	//							y					-	the y position of the emitter
	//							explosion		-	whether it's an explosion or ring
	//							pointsprites	-	use point sprites or not
	//
	// Output:			None
	//
	// Returns:			Nothing
	//
	// Purpose:			
	//
	//////////////////////////////////////////////////////////
	ParticleEmitter(float x = 0, float y = 0, bool explosion = true, bool pointsprites = false) : Explosion(explosion), PointSprites(pointsprites)
	{
		EnableTexture2	= false;
		EnableTexture3	= false;

		NumParticles	= 1000;
		Base			= Particle(x, y);
		Base.parent		= this;

        PickupRange		= 10;

		bUseSinAV		= false;
		bUseSinRV		= false;

		m_pVertexBuffer = NULL;
	}

	//////////////////////////////////////////////////////////
	// Function:			constructor
	//
	// Last modified:	September 14, 2004
	//
	// Input:				base				-	the particle to set as the base
	//
	// Output:			None
	//
	// Returns:			Nothing
	//
	// Purpose:			
	//
	//////////////////////////////////////////////////////////
	ParticleEmitter(Particle &base)
	{
		Base			= base;
		Base.parent		= this;
		m_pVertexBuffer = NULL;
	}

	//////////////////////////////////////////////////////////
	// Function:			destructor
	//
	// Last modified:	September 14, 2004
	//
	// Input:				None
	//
	// Output:			None
	//
	// Returns:			Nothing
	//
	// Purpose:			
	//
	//////////////////////////////////////////////////////////
	~ParticleEmitter()
	{
		SAFE_RELEASE(m_pVertexBuffer);
	}

	//////////////////////////////////////////////////////////
	// Function:			FToDW()
	//
	// Last modified:	September 14, 2004
	//
	// Input:				None
	//
	// Output:			None
	//
	// Returns:			Nothing
	//
	// Purpose:			
	//
	//////////////////////////////////////////////////////////
	inline DWORD FToDW( float f ) { return *((DWORD*)&f); }

	//////////////////////////////////////////////////////////
	// Function:			InitParticles()
	//
	// Last modified:	September 14, 2004
	//
	// Input:				None
	//
	// Output:			None
	//
	// Returns:			Nothing
	//
	// Purpose:			One time initialization of particles and emitter
	//
	//////////////////////////////////////////////////////////
	void InitParticles();

	//////////////////////////////////////////////////////////
	// Function:			Handle()
	//
	// Last modified:	September 14, 2004
	//
	// Input:				None
	//
	// Output:			None
	//
	// Returns:			Nothing
	//
	// Purpose:			
	//
	//////////////////////////////////////////////////////////
	void Handle();

	//////////////////////////////////////////////////////////
	// Function:			RenderPS()
	//
	// Last modified:	September 14, 2004
	//
	// Input:				None
	//
	// Output:			None
	//
	// Returns:			Nothing
	//
	// Purpose:			
	//
	//////////////////////////////////////////////////////////
	void RenderPS();

	void SetActive(UINT numActive = 0, bool goOneTime = true, bool _active = true)
	{
		NumActiveParticles = (numActive ? numActive : NumParticles);
		Active = _active;
		GoOneTime = goOneTime;
	}

	void InitEmitter();

	//The base particle is what represents the particle emitter in "particle" terms
	//All particles begin as this base particle with slightly modified velocities
	Particle		Base;

	bool	Active;

	//Are point sprites turned on?
	bool	PointSprites;


	//Is it an explosion or ring based particle system?
	bool	Explosion;

	//Self-explanatory
	bool	EnableTexture2;
	bool	EnableTexture3;

	bool	GoOneTime;

	//Whether or not the device supports point sprites (not fully checked against)
	bool  bDeviceSupportsPSIZE;

	//Apply a sine wave to the angular velocity?
	bool	bUseSinAV;

	//Apply a sine wave to the radial velocity
	bool	bUseSinRV;

	bool bApplyGravity;
	bool bApplyWind;

	//The total number o' particles this emitter keeps track of
	int	NumParticles;

	int	NumActiveParticles;

	//Maximum point sprite size supported by hardware
	float fMaxPointSize;

	//How far away a particle must be before polar vel/accel is applied to it
	float PickupRange;

	//The actual particles
	DblLinkedList<Particle>			particles;
	
	//The vertex buffer that holds the point sprite vertices
	LPDIRECT3DVERTEXBUFFER9			m_pVertexBuffer;
};