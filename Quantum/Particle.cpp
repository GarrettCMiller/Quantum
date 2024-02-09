///////////////////////////////////////////////////////////////////////////////
//		File:				Particle.cpp
//
//		Programmer:			Garrett Miller (GM)
//
//		Date created:		September 10, 2004
//
//		Description:		Implementation of a particle system
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "SpriteMgr.h"
#include "Particle.h"
#include "d3dwrapper.h"

#include "Globals.h"

#include "EffectMgr.h"

#include "MathFuncs.h"

//////////////////////////////////////////////////////////////////////////

//No gravity to begin with
//Vec2 Gravity		= Vec2(0.0f, 0.01f);
Vec3 g_vGravity		= Vec3(0.0f, -0.01f, 0.0f);
//No wind either
Vec3 g_vWind		= Vec3(0.0f, 0.0f, 0.0f);

Int32 Particle::ParticlesSoFar = 0;

// Float Particle::m_fRadialVelocityWave_Freq	= 1.0f;
// Float Particle::m_fRadialVelocityWave_Base	= 0.5f;
// Float Particle::m_fRadialVelocityWave_Amp	= 0.5f;
// 
// Float Particle::m_fThetaVelocityWave_Freq	= 1.0f;
// Float Particle::m_fThetaVelocityWave_Base	= 0.0f;
// Float Particle::m_fThetaVelocityWave_Amp	= 0.25f;

UInt32 Particle::ms_dwUniqueID	= 0;

Particle::EColorMode Particle::mk_eColorMode = Particle::eCM_KineticEnergy;

Float g_Mass = 1.0f;

HRESULT hr;

//////////////////////////////////////////////////////////////////////////

//Set that particle up
Particle::Particle(Vec3 pos, Vec3 vel, Color color)
			:	m_vPos(pos), m_vVel(vel),
				m_cColor(color),
				m_fRotation(0),	m_fRotSpeed(0),
				m_fSize(1),
				m_fShrinkRate(0.0f),
				m_nMinLife(20),	m_nMaxLife(MAX_LIFE),
				m_bSeeking(false),
				m_bActive(true), m_bRender(true),
				m_Mesh(NULL)
{
	m_nLife		= Random(m_nMaxLife, m_nMinLife);
	m_bAutoReset = true;
	m_cColor.SetAlpha(1.0f);
	m_cDecay = Color(0.0f, 0.0f, 0.0f, 0.0f);
	m_vAccelDecay = Vec3(1.1f, 1.1f, 1.1f);
	m_fMass = 1.0f;
}

//////////////////////////////////////////////////////////////////////////

Bool Particle::InitMesh()
{
	D3DMesh tMesh;

	SAFE_RELEASE(m_Mesh);

	D3DXCreateSphere(g_D3D->GetDevice(), m_fSize, 16, 16, &tMesh, NULL);
	//D3DXCreateBox(g_D3D->GetDevice(), m_fSize, m_fSize, m_fSize, &tMesh, NULL);

	if (FAILED(tMesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, CVertex::FVF, g_D3D->GetDevice(), &m_Mesh)))
		return false;

	tMesh->Release();

	CVertex* pVerts;

	if (SUCCEEDED(m_Mesh->LockVertexBuffer(0, (LPVOID *)&pVerts)))
	{
		UInt32 numVerts = m_Mesh->GetNumVertices();

		for (UInt32 i = 0; i < numVerts; i++, pVerts++)
		{
			Float dir = 0.0f;
			dir = atan2f(pVerts->normal.z, pVerts->normal.x) * Rad2Deg;

			pVerts->color = Color::km_White;

			pVerts->tu	= dir / 360.0f;
			pVerts->tv	= 1.0f - asinf(pVerts->normal.y) / PI + 0.5f;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

void Particle::Init()
{
	PROFILE_THIS;

	static const Int32 mode = 2;

	Float scalar = 50.0;

	//HACK
	UInt32 tempID = m_dwUniqueID;

	//Set the particle back to the "base" state
	*this = Particle(m_pParent->m_Base);

	m_dwUniqueID = tempID;

	if (g_bVoxel)
	{
		return;
	}

	//parent->EmitCount++;

	Float speed = 0;
	Float theta = 0, phi = 0;

	switch (m_pParent->EmitDist)
	{
	case eED_Even:	//HACK
	case eED_Boundary:
		{
			speed = m_pParent->m_Base.m_vPolarVel.x;
			theta = m_fInitAngles.X = Random() * Float(m_pParent->MaxSpawnAngleTheta - m_pParent->MinSpawnAngleTheta) + m_pParent->MinSpawnAngleTheta;
			phi = m_fInitAngles.Y = RandomG() * Float(m_pParent->MaxSpawnAnglePhi - m_pParent->MinSpawnAnglePhi) + m_pParent->MinSpawnAnglePhi;
			
			m_vVel.SetSpeedDir(speed, theta, phi);

			if (m_pParent->m_Base.m_vPolarVel.z != 0)
			{
				m_vVel = m_vVel.Cross(k_v3YAxis);
				m_vVel.Normalize();
				m_vVel *= speed;
			}

			m_vVel += m_pParent->m_Base.m_vVel;

			
			m_vPolarVel = m_pParent->m_Base.m_vPolarVel;
			//m_vVel.SetSpeedDir(m_vVel.Magnitude(), theta, phi);
			//m_vVel.SetSpeedDir(m_vVel.x, theta, phi);
			//m_vPolarVel.SetSpeedDir(m_vPolarVel.x, theta, phi);
		}
		break;

	//case eED_Even:
		//{
		//	speed = Random() * m_pParent->m_Base.m_vPolarVel.x + 0.0001f;//keep from getting no vel
		//	theta = m_fInitAngles.X = Random() * Float(m_pParent->MaxSpawnAngleTheta - m_pParent->MinSpawnAngleTheta) + m_pParent->MinSpawnAngleTheta;
		//	phi = m_fInitAngles.Y = RandomG() * Float(m_pParent->MaxSpawnAnglePhi - m_pParent->MinSpawnAnglePhi) + m_pParent->MinSpawnAnglePhi;
		//	m_vVel.SetSpeedDir(speed, theta, phi);

		//	if (m_pParent->m_Base.m_vPolarVel.z != 0)
		//	{
		//		m_vVel = m_vVel.Cross(k_v3YAxis);
		//		m_vVel.Normalize();
		//		m_vVel *= speed;
		//	}

		//	m_vVel += m_pParent->m_Base.m_vVel;
		//}
		//break;
	
	case eED_Random:
		{
			speed = Random() * m_pParent->m_Base.m_vPolarVel.x + 0.0001f;//keep from getting no vel
			theta = m_fInitAngles.X = Random() * Float(m_pParent->MaxSpawnAngleTheta - m_pParent->MinSpawnAngleTheta) + m_pParent->MinSpawnAngleTheta;
			phi = m_fInitAngles.Y = RandomG() * Float(m_pParent->MaxSpawnAnglePhi - m_pParent->MinSpawnAnglePhi) + m_pParent->MinSpawnAnglePhi;
			m_vVel.SetSpeedDir(speed, theta, phi);

			if (m_pParent->m_Base.m_vPolarVel.z != 0)
			{
				m_vVel = m_vVel.Cross(k_v3YAxis);
				m_vVel.Normalize();
				m_vVel *= speed;
			}

			m_vVel += m_pParent->m_Base.m_vVel;
		}
		break;
	}

	//Give a new speed and direction
	//m_vVel.SetSpeedDir(speed, theta, phi);

	if (m_pParent->m_bRandomizeColor)
		m_cColor.Randomize();

	Float brightness = 1.0f;

	static Int32 nAlt = 0;

	//switch (Random(1, 0))
	//{
	//case 0:
	//	case 1:
	//		case 2:
	//			case 3:
	//	m_fMass = 1.0f;
	//	//m_cColor = Color(1.0f, 0.0f, 0.0f);
	//	break;

	//case 4:
	//	case 5:
	//		case 6:
	//	m_fMass = 2.5f;
	//	//m_cColor = Color(0.0f, 1.0f, 0.0f);
	//	break;

	//case 7:
	//	case 8:
	//	m_fMass = 5.0f;
	//	//m_cColor = Color(0.0f, 0.0f, 1.0f);
	//	break;

	//case 9:		
	//	m_fMass = 10.0f;
	//	//m_cColor = Color(1.0f, 1.0f, 1.0f);
	//	break;

	//default:	m_fMass = 0.25f; break;
	//}

	m_fMass = g_Mass;

	//m_fSize *= m_fMass;

	static const Float k_range = 10.0f;

// 	float a = Random();
// 	float b = 1.0f - a;
// 	float x,y,z;
// 
// 	Vec3	A = m_pParent->pA * scalar,
// 			B = m_pParent->pB * scalar,
// 			C = m_pParent->pC * scalar,
// 			D = m_pParent->pD * scalar;

	switch( m_pParent->EmitShape ) 
	{
	case Emit_Spherical:
		if (m_pParent->EmitRadius > 0.0f)
		{
// 			float theta = m_fInitAngles.X = Random() * 359.0f,
// 					phi = m_fInitAngles.Y = Random() * 179.0f;

			if (m_pParent->EmitDist)
				m_vPos.SetSpeedDir(RandomG(1.0f, 0.0f, 10) * m_pParent->EmitRadius, theta, phi);
			else
				m_vPos.SetSpeedDir(m_pParent->EmitRadius, theta, phi);

			m_vPos += m_pParent->m_Base.m_vPos;
		}
	break;

	case Emit_Rectangular:
		//BirthPos = parent->Base.Pos;

		m_vPos.RandomUnitVolume();
		m_vPos *= m_pParent->EmitRadius;		

		break;

	case Emit_Arc:
		//m_vVel.SetSpeedDir(m_pParent->EmitDist ? Random(m_pParent->m_Base.m_vVel.X, 0.0f) : m_vPolarVel.X, Random(m_pParent->MaxSpawnAngle * 10.0f, m_pParent->MinSpawnAngle * 10.0f) / 10.0f);

		if (m_pParent->EmitDist)
			m_vPos.SetSpeedDir(m_pParent->EmitRadius, 0.0f, 90.0f);
		else
			m_vPos.SetSpeedDir(m_pParent->EmitRadius, theta, 90.0f);

		break;

#if 0
	case Emit_Curve:
		x = A.x * a + B.x * b;
		y = A.y * a + B.y * b;

		m_vPos.x += x;
		m_vPos.y += y;

		//m_vVel.SetSpeedDir(m_pParent->EmitDist ? Random(50, 10) / 10.0f : m_vPolarVel.X, Random(m_pParent->MaxSpawnAngle * 10.0f, m_pParent->MinSpawnAngle * 10.0f) / 10.0f);
		break;

	case Emit_Curve2:
		x = A.x * a * a + B.x * 2 * a * b + C.x * b * b;
		y = A.y * a * a + B.y * 2 * a * b + C.y * b * b;

		m_vPos.x += x;
		m_vPos.y += y;

		//m_vVel.SetSpeedDir(m_pParent->EmitDist ? Random(50, 10) / 10.0f : m_vPolarVel.X, Random(m_pParent->MaxSpawnAngle * 10.0f, m_pParent->MinSpawnAngle * 10.0f) / 10.0f);
		break;

	case Emit_Curve3:
		x = A.x * a * a * a + B.x * 3 * a * a * b + C.x * 3 * a * b * b + D.x * b * b * b;
		y = A.y * a * a * a + B.y * 3 * a * a * b + C.y * 3 * a * b * b + D.y * b * b * b;


		m_vPos.x += x;
		m_vPos.y += y;

		//m_vVel.SetSpeedDir(m_pParent->EmitDist ? Random(m_pParent->m_Base.m_vVel.X, 0.0f) : m_vPolarVel.X, Random(m_pParent->MaxSpawnAngle * 10.0f, m_pParent->MinSpawnAngle * 10.0f) / 10.0f);
		break;
#endif
#if 0
	case Emit_Atom:
		{
			Dim X As Single, Y As Single
				Dim r As Single

				r = 10 + Sin(2 * (I / 10)) * 50
				X = sngX + r * Cos(I / 30)
				Y = sngY + r * Sin(I / 30)
		}
		break;

	case Emit_Galaxy:
		{
			Dim X As Single, Y As Single
				Dim r As Single

				r = Sin(20 / (I + 1)) * 60
				X = sngX + (r * Cos((I)))
				Y = sngY + (r * Sin((I)))
		}
		break;

	case Emit_Heart:
		{
			Dim X As Single, Y As Single
				Dim Q As Single
				Dim a As Single, b As Single
				a = 50: b = 50 'Q=Theta Angle 'HEART

				Q = I / 100
				Y = sngY - a * Cos(Q * 2) * Sqr(Abs(Sin(Q)))
				X = sngX + b * Sin(Q * 2) * Sqr(Abs(Cos(Q)))
		}
		break;
#endif

	case Emit_HighSphere:
		{
#if 0
			nAlt = Random(10);

			switch (nAlt)
			{
			case 0:
			case 4:
			case 7:
			case 9:
				{
					m_vPos.x	= -m_pParent->EmitRadius + Random(k_range, -k_range);
					m_vPos.y	= Random(k_range, -k_range);
					m_vPos.z	= 0.0f;//-m_pParent->EmitRadius + Random(50, -50);
					m_vVel.SetSpeedDir(m_pParent->m_Base.m_vVel.x, 0.0f, 90.0f);
					m_cColor = Color(brightness, 0.0f, 0.0f, m_pParent->m_Base.m_cColor.Alpha());

					m_fMass = 1.0f;

					//nAlt++;
				}
				break;

			case 1:
			case 5:
			case 8:
				{
					m_vPos.x	= m_pParent->EmitRadius + Random(k_range, -k_range);
					m_vPos.y	= Random(k_range, -k_range);
					m_vPos.z	= 0.0f;//-m_pParent->EmitRadius + Random(50, -50);
					m_vVel.SetSpeedDir(-m_pParent->m_Base.m_vVel.x, 0.0f, 90.0f);
					m_cColor = Color(0.0f, 0.0f, brightness, m_pParent->m_Base.m_cColor.Alpha());

					m_fMass = 2.0f;

					//nAlt++;
				}
				break;

			case 2:
			case 6:
				{
					m_vPos.x	= Random(k_range, -k_range);
					m_vPos.y	= -m_pParent->EmitRadius + Random(k_range * 0.5f, -k_range * 0.5f);
					m_vPos.z	= 0.0f;//-m_pParent->EmitRadius + Random(50, -50);
					m_vVel.SetSpeedDir(m_pParent->m_Base.m_vVel.x, 90.0f, 90.0f);
					m_cColor = Color(0.0f, brightness, 0.0f, m_pParent->m_Base.m_cColor.Alpha());

					m_fMass = 3.0f;

					//nAlt++;
				}
				break;

			case 3:
				{
					m_vPos.x	= Random(k_range, -k_range);
					m_vPos.y	= m_pParent->EmitRadius + Random(k_range * 0.5f, -k_range * 0.5f);
					m_vPos.z	= 0.0f;//-m_pParent->EmitRadius + Random(50, -50);
					m_vVel.SetSpeedDir(-m_pParent->m_Base.m_vVel.x, 90.0f, 90.0f);
					m_cColor = Color(brightness * 0.5f, brightness * 0.5f, brightness * 0.5f, m_pParent->m_Base.m_cColor.Alpha());

					m_fMass = 5.0f;

					//nAlt = 0;
				}
				break;
			}
#else
			if (m_pParent->EmitDist)
				m_vPos.SetSpeedDir(RandomG(1.0f, 0.0f, 10) * m_pParent->EmitRadius, theta, phi);
			else
				m_vPos.SetSpeedDir(m_pParent->EmitRadius, theta, phi);

			m_vPos.y += m_pParent->EmitRadius * 2.5f;
			m_vPos += m_pParent->m_Base.m_vPos;
#endif
		}
		break;

	case Emit_FourSpheres:
		{
			if (m_pParent->EmitDist)
				m_vPos.SetSpeedDir(RandomG(1.0f, 0.0f, 7) * m_pParent->EmitRadius, theta, phi);
			else
				m_vPos.SetSpeedDir(m_pParent->EmitRadius, theta, phi);

			m_vPos += m_pParent->m_Base.m_vPos;

			Vec3 pd;
			Float mag = 100.0f;

			Float alpha = m_pParent->m_Base.m_cColor.Alpha();

			switch (m_dwUniqueID % 4)
			{
			case 0:
				pd.SetSpeedDir(mag * 2.5f, 0.0f, 45.0f);
				m_cColor = Color(0.0f, 0.0f, 1.0f, alpha);
				break;

			case 1:
				pd.SetSpeedDir(mag * 3.5f, 270.0f, 135.0f);
				m_cColor = Color(0.0f, 1.0f, 0.0f, alpha);
				break;

			case 2:
				pd.SetSpeedDir(mag * 3.0f, 90.0f, 135.0f);
				m_cColor = Color(1.0f, 0.0f, 0.0f, alpha);
				break;

			case 3:
				pd.SetSpeedDir(mag * 2.0f, 180.0f, 45.0f);
				m_cColor = Color(1.0f, 0.0f, 1.0f, alpha);
				break;
			}
			
			m_vPos += pd;
		}
		break;

	case Emit_GalaxyDisk:
		{
			static UInt32 nArms = 6;
			static Float fArmAngle = ((360 / nArms) % 360);
			static Float fAngSpread = 180.0f / (nArms * 2.0f / 5.0f);
			
			Float i = static_cast<Float>(nAlt % m_pParent->NumParticles);
			
			Float r = Random() * m_pParent->EmitRadius;
			Float q = RandomG(fAngSpread, 0.0f, 4) * (rand()&1?1:-1);
			static const Float k = 1.0f;

			Float a = Random(nArms) * fArmAngle;

			m_vPos.x = r * cos(Deg2Rad * (a + r * k + q));
			m_vPos.y = 0.0f;
			m_vPos.z = r * sin(Deg2Rad * (a + r * k + q));

			m_vPos += m_pParent->m_Base.m_vPos;
		}
		break;

	case Emit_TwoSpheres:
		{
			if (m_pParent->EmitDist)
				m_vPos.SetSpeedDir(RandomG(1.0f, 0.0f, 7) * m_pParent->EmitRadius, theta, phi);
			else
				m_vPos.SetSpeedDir(m_pParent->EmitRadius, theta, phi);

			m_vPos += m_pParent->m_Base.m_vPos;

			Vec3 pd;
			Float mag = 250.0f;

			Float alpha = m_pParent->m_Base.m_cColor.Alpha();

			switch (m_dwUniqueID % 2)
			{
			case 0:
				pd.SetSpeedDir(mag, /*1*/0.0f, 90.0f);
				m_cColor = Color(0.0f, 0.0f, 1.0f, alpha);
				break;

			case 1:
				pd.SetSpeedDir(mag, 180.0f, 90.0f);
				m_vVel *= -1.0f;
				m_cColor = Color(0.0f, 1.0f, 0.0f, alpha);
				break;
			}
			
			m_vPos += pd;
		}
		break;
	}

	if (m_pParent->m_bAnchorRoot && m_dwUniqueID == 0)
	{
		m_vPos = Vec3(0.0f, 0.0f, 0.0f);
		m_fMass = 100.0f;
		m_cColor = Color::km_White;
		m_vVel = k_v3Zero;
		//m_fSize *= 100.0f;
	}

	/*else if (m_dwUniqueID == 1)
	{
		m_vPos = Vec3(-500.0f, 100.0f, 0.0f);
		m_fMass = 100.0f;
		m_cColor = Color::km_White;
		m_fSize = 150.0f;
	}*/
	//else
	//{
	//	m_fMass = 10.0f;
	//	m_cColor = Color::km_Red;
	//	m_fSize = 20.0f;
	//	//m_cColor.SetAlpha(0.25f);
	//}

	//m_fMass = Random() * 100.0f;

	if (m_pParent->m_bRandomizeMass)
	{
		m_vVel = k_v3Zero;

		//m_fMass = (m_dwUniqueID % 10) + 1;
		//m_fCharge = (m_dwUniqueID % 3) - 1 * 1.0f;
		switch (Random(6, 1))
		{
		case 1:
		case 4:
		case 6:
			m_fMass = 1.0f;
			break;

		case 2:
		case 5:
			m_fMass = 5.0f;
			break;

		case 3:
			m_fMass = 10.0f;
			break;
		}
	}

	if (m_pParent->m_bRandomizeCharge)
	{
		switch (Random(6, 1))
		{
		case 1:
		case 4:
		case 6:
		//case 8:
			m_fElecCharge = 1.0f;
			m_fSize *= 5.0f;
			m_fMass = 10.0f;
			break;

		case 2:
		case 5:
		case 7:
		//case 9:
			m_fElecCharge = -1.0f;
			m_fSize *= 1.0f;
			m_fMass = 1.0f;
			break;

		case 3:
			m_fElecCharge = 0.0f;
			m_fSize *= 5.0f;
			m_fMass = 10.0f;
			break;
		}
	}

	//if (m_dwUniqueID == 0)
	//{
	//	m_vPos = k_v3Zero;
	//	//m_fMass = 10.0f;
	//	m_vVel = k_v3Zero;
	//}


	//m_vBirthPos = m_vTargetPos = m_vPos;
	//m_vBirthPos = m_pParent->m_Base.m_vPos;
	m_vBirthPos = m_pParent->m_ForceMgr.m_vCenterOfMass;

	//Set a life amount
	if (m_nMaxLife < m_nMinLife)
		m_nMaxLife = m_nMinLife;

	m_nLife = Random(m_nMaxLife, m_nMinLife);

	nAlt++;

	if (m_pParent->m_bApplyCollisions)
		m_ptArr.SetSize(1024);

	/*if (!m_Mesh)
		VERIFY(InitMesh());*/
}

//////////////////////////////////////////////////////////////////////////

void Particle::Reset()
{
	PROFILE_THIS;

	//Vel.SetSpeedDir( kpLevels->frequency[0][int(a*1024)] / 25.5f, 90);*/
	//m_pParent->activeParticles.Remove((Particle *)this);
	//m_pParent->freeParticles.Enqueue(this);

	Init();
}

//////////////////////////////////////////////////////////////////////////

void Particle::Update()
{
	//PROFILE_THIS;

	ASSERT(m_pParent);

// 	if (!m_bReady)
// 	{
// 		if (m_tDelayTimer.Get() >= m_fDelay)
// 			m_bReady = true;
// 		else
// 			return;
// 	}

	if (m_bActive)
	{
		DoLife();

		DoMovement();

		Float vel = m_vVel.MagnitudeSquared();

		/*if (vel < 1.0f)
			vel = m_vVel.Magnitude() * 20.0f;*/

		m_fKineticEnergy = 0.5f * m_fMass * vel;

		m_fPotentialEnergy = m_fMass * m_vPos.y;

		//DoRotation();

		DoColor();
	}

	if (m_pParent->m_bApplyCollisions /*&& m_nLife % 25 == 0*/)
	{
		if (m_nLife % Max(UInt32(2 / fabsf(m_pParent->m_fTimeMultiplier)), UInt32(1)) == 0)
		{
			ParticleState state;
			
			state.pos = m_vPos;
			state.vel = m_vVel;
			state.kineticEnergy = m_fKineticEnergy;
			state.potentialEnergy = m_fPotentialEnergy;

			Color cColor = m_cColor;
			cColor.SetAlpha(m_pParent->m_Base.m_cColor.Alpha());
			m_ptArr.Push(CVertex(state.pos, k_v3Zero, 0.0f, 0.0f, cColor));
		}

		CVertex temp;
		if (m_ptArr.Size() > 2048)
			m_ptArr.PopFront(temp);
	}

	//If we're not a point sprite draw us
	//if (!(m_pParent->m_bPointSprites))
		Draw();
}

//////////////////////////////////////////////////////////////////////////

void Particle::DoMovement()
{
	//PROFILE_THIS

	DoPolarCoords();

	if (1)
		m_vVel += m_vAccel * m_pParent->m_fTimeMultiplier;
	else
	{
		/*m_vVel.x = Sine(m_pParent->m_Base.m_vPolarAccel.x, 0.0f, m_pParent->m_Base.m_vAccel.x, m_vPos.z) * m_pParent->m_fTimeMultiplier;
		m_vVel.y = Sine(m_pParent->m_Base.m_vPolarAccel.y, 0.0f, m_pParent->m_Base.m_vAccel.y, m_vPos.x) * m_pParent->m_fTimeMultiplier;
		m_vVel.z = Sine(m_pParent->m_Base.m_vPolarAccel.z, 0.0f, m_pParent->m_Base.m_vAccel.z, m_vPos.y) * m_pParent->m_fTimeMultiplier;*/

		Float	k_a =	//m_pParent->m_Base.m_vPolarAccel.x, 
						//0.1f,
						10,
				k_b =	//m_pParent->m_Base.m_vPolarAccel.y,
						//0.1f,
						8 / 3.0f,
				k_c =	//m_pParent->m_Base.m_vPolarAccel.z;	
						//14;
						99.96;

		if (0)
		{
			m_vVel.x = -m_vPos.y - m_vPos.z;
			m_vVel.y = m_vPos.x + k_a * m_vPos.y;
			m_vVel.z = k_b + m_vPos.z * (m_vPos.x - k_c);
		}
		else
		{
			m_vVel.x = k_a * (m_vPos.y - m_vPos.x);
			m_vVel.y = m_vPos.x * (k_b - m_vPos.z) - m_vPos.y;
			m_vVel.z = m_vPos.x * m_vPos.y - k_c * m_vPos.z;
		}
	}

	
	if (m_pParent->m_bApplyGravity || m_pParent->m_bApplyEMForce)
	{
		m_vForceSum = k_v3Zero;
		m_pParent->m_ForceMgr.Add(this);
	}

// 	if (m_bSeeking && Distance(m_vPos, m_vTargetPos) <= m_vVel.Magnitude())
// 	{
// 		m_vPos = m_vTargetPos;
// 		m_vVel = k_v3Zero;
// 		m_bSeeking = false;
// 	}

	if (m_pParent->m_bAnchorRoot && m_dwUniqueID == 0)
	{
		m_vPos = m_vVel = k_v3Zero;
		m_cColor = Color::km_White;
	}

	//if (m_fCharge > 0)
	//	m_vVel = k_v3Zero;

	m_vPos += m_vVel * m_pParent->m_fTimeMultiplier;

	//Make sure acceleration isn't constantly applied
	//m_vAccel			*= 0.95f;//m_vAccelDecay;
	//m_vPolarAccel		*= 0.9f;
}

//////////////////////////////////////////////////////////////////////////

void Particle::DoPolarCoords()
{
	PROFILE_THIS;

	Vec3 temp, temp2;

	//PolarVel		+=	PolarAccel;

	//Check for polar velocity and proper range
	if (m_pParent->PickupRange > 0)	//(m_vPolarVel.x || m_vPolarVel.y || m_vPolarVel.z || m_pParent->m_bRadialVelocityWave || m_pParent->m_bThetaVelocityWave))
	{
		//Get the distance between the base and us
		Float dist = DistanceSquared(m_vPos, m_vBirthPos);

		if (dist < 0.00001f)
			dist = 0.0f;

		if (dist >= m_pParent->PickupRange)
		{
			Vec3 temp = m_vPolarAccel;
			//temp.Normalize();

			D3DXMATRIX matrix, mRx, mRy, mRz;
			D3DXQUATERNION rot;

			//float val = (m_vPolarVel.x - static_cast<int>(m_vPolarVel.x)) / 1000.0f + static_cast<int>(m_vPolarVel.x);

			//if (val == 0)
			//	val = 1;

			//Vec3 scaling = Vec3(val, val, val);

// 			D3DXMatrixRotationAxis(&matrix, &temp, m_vPolarAccel.x * Deg2Rad);
// 			D3DXQuaternionRotationMatrix(&rot, &matrix);
			
			D3DXQuaternionRotationAxis(&rot, &temp, m_vPolarVel.y * Deg2Rad);

			temp =	//k_v3Zero;
					//k_v3One;
					//Vec3(0.0f, m_vPos.y, 0.0f);
					//m_vPos;
					m_pParent->m_ForceMgr.m_vCenterOfMass;

			D3DXMatrixTransformation(&matrix, &temp, NULL, /*&scaling*/NULL, &temp, &rot, NULL/*&m_vVel*/);

			Vec3 force;
			D3DXVec3TransformCoord(&force, &m_vPos, &matrix);

			force -= m_vPos;

			m_vPos += force;
			
			//D3DXVec3TransformCoord(&m_vPos, &m_vPos, &matrix);

			/*Vec3 temp;

			D3DXVec3TransformCoord(&temp, &m_vPos, &matrix);

			Vec3 vel;
			vel.SetSpeedDir(m_vPolarVel.x, m_vPolarVel.y, m_vPolarVel.z);
			m_vVel += vel;*/

			//D3DXMatrixTranslation(&matrix, m_vVel.x, m_vVel.y, m_vVel.z);
			//D3DXVec3TransformCoord(&m_vPos, &m_vPos, &matrix);
		}
 		else
 		{
  			/*D3DXMATRIX matrix;
  			D3DXMatrixTranslation(&matrix, m_vPolarVel.x, m_vVel.y, m_vVel.z);
  			D3DXVec3TransformCoord(&m_vPos, &m_vPos, &matrix);*/
 		}
	}
}

//////////////////////////////////////////////////////////////////////////

void Particle::DoRotation()
{
	// 	if (m_fShrinkRate > 0.0f && m_fSize > 0.1f)
	// 		m_fSize -= m_fShrinkRate / 20.0f;

	//Rot				+= RotSpeed;
}

//////////////////////////////////////////////////////////////////////////

void Particle::DoColor()
{
	//Do color decay
	//You can write your own bounds checking for some COOL effects
//  	if (m_cDecay.Alpha() != 0 && m_cColor.Alpha() >= m_cDecay.Alpha())// && Alpha <= 255 + AlphaDecay)
//  		m_cColor.SetAlpha(m_cColor.Alpha() - m_cDecay.Alpha());
// 
// 	if (m_cDecay.Red() != 0 && m_cColor.Red() >= m_cDecay.Red())// && Alpha <= 255 + AlphaDecay)
// 		m_cColor.SetRed(m_cColor.Red() - m_cDecay.Red());
// 
// 	if (m_cDecay.Green() != 0 && m_cColor.Green() >= m_cDecay.Green())// && Alpha <= 255 + AlphaDecay)
// 		m_cColor.SetGreen(m_cColor.Green() - m_cDecay.Green());
// 
// 	if (m_cDecay.Blue() != 0 && m_cColor.Blue() >= m_cDecay.Blue())// && Alpha <= 255 + AlphaDecay)
// 		m_cColor.Blue(m_cColor.Blue() - m_cDecay.Blue());

	//m_cColor -= m_cDecay;
	//m_cColor.SetAlpha(m_cColor.Alpha() - m_cDecay.Alpha());

	ParticleState state;
		
	state.pos = m_vPos;
	state.vel = m_vVel;
	state.kineticEnergy = m_fKineticEnergy;
	state.potentialEnergy = m_fPotentialEnergy;

	m_cColor = PhysicsColoring(state);
}

//////////////////////////////////////////////////////////////////////////

void Particle::DoLife()
{
	PROFILE_THIS;

	m_nLife -= m_pParent->m_nLifeDecay;

	//Check to see if our life is over or we're totally transparent
	if (m_nLife <= 0 /*|| m_cColor.Alpha() <= 0*/)
	{
		if (m_nMinLife != 10000)
		{
			m_pParent->NumActiveParticles--;

			if (m_bAutoReset && !m_pParent->m_bGoOneTime)
				Reset();
		}
		else
			m_nLife = 10000;
	}
}

//////////////////////////////////////////////////////////////////////////

Bool Particle::Collision(Particle* pOther)
{
	Float fDist = m_fSize + pOther->m_fSize;
	fDist *= fDist;
	return (DistanceSquared(m_vPos, pOther->m_vPos) <= fDist);
}

//////////////////////////////////////////////////////////////////////////

Int32 Particle::OnCollision(Particle* pOther)
{
#if 1
	Vec3	p1	= m_vPos,
			p2	= pOther->m_vPos,
			dp	= p2 - p1,

			v1	= m_vVel,
			v2	= pOther->m_vVel,
			dv	= v2 - v1;

	float	m1	= m_fMass,
			m2	= pOther->m_fMass,
			dm	= m2 / m1;

	float	a	= dp.y / dp.x;

	float	dvx	= -2 * (dv.x + a * dv.y) / ((1 + a * a) * (1 + dm));

	pOther->m_vVel.x += dvx;
	pOther->m_vVel.y += a * dvx;

	m_vVel.x = m_vVel.x - dm * dvx;
	m_vVel.y = m_vVel.y - a * dm * dvx;

#endif

	return 1;
}

//////////////////////////////////////////////////////////////////////////

#if 0
int Particle::OnCollision(Particle* pOther)
{
	dm = m2 / m1;
	x21=x2-x1;
	y21=y2-y1;
	z21=z2-z1;
	vx21=vx2-vx1;
	vy21=vy2-vy1;
	vz21=vz2-vz1;

	//     **** calculate relative distance and relative speed ***
	d=sqrt(x21*x21 +y21*y21 +z21*z21);
	v=sqrt(vx21*vx21 +vy21*vy21 +vz21*vz21);

	//     **** return if distance between balls smaller than sum of radii ****
	if (d<r12) {error=2; return;}

	//     **** return if relative speed = 0 ****
	if (v==0) {error=1; return;}


	//     **** shift coordinate system so that ball 1 is at the origin ***
	x2=x21;
	y2=y21;
	z2=z21;

	//     **** boost coordinate system so that ball 2 is resting ***
	vx1=-vx21;
	vy1=-vy21;
	vz1=-vz21;

	//     **** find the polar coordinates of the location of ball 2 ***
	theta2=acos(z2/d);
	if (x2==0 && y2==0) {phi2=0;} else {phi2=atan2(y2,x2);}
	st=sin(theta2);
	ct=cos(theta2);
	sp=sin(phi2);
	cp=cos(phi2);


	//     **** express the velocity vector of ball 1 in a rotated coordinate
	//          system where ball 2 lies on the z-axis ******
	vx1r=ct*cp*vx1+ct*sp*vy1-st*vz1;
	vy1r=cp*vy1-sp*vx1;
	vz1r=st*cp*vx1+st*sp*vy1+ct*vz1;
	thetav=acos(vz1r/v);
	if (vx1r==0 && vy1r==0) {phiv=0;} else {phiv=atan2(vy1r,vx1r);}


	//     **** calculate the normalized impact parameter ***
	dr=d*sin(thetav)/r12;


	//     **** return old positions and velocities if balls do not collide ***
	if (thetav>pi/2 || fabs(dr)>1) {
		x2=x2+x1;
		y2=y2+y1;
		z2=z2+z1;
		vx1=vx1+vx2;
		vy1=vy1+vy2;
		vz1=vz1+vz2;
		error=1;
		return;
	}

	//     **** calculate impact angles if balls do collide ***
	alpha=asin(-dr);
	beta=phiv;
	sbeta=sin(beta);
	cbeta=cos(beta);


	//     **** calculate time to collision ***
	t=(d*cos(thetav) -r12*sqrt(1-dr*dr))/v;


	//     **** update positions and reverse the coordinate shift ***
	x2=x2+vx2*t +x1;
	y2=y2+vy2*t +y1;
	z2=z2+vz2*t +z1;
	x1=(vx1+vx2)*t +x1;
	y1=(vy1+vy2)*t +y1;
	z1=(vz1+vz2)*t +z1;



	//  ***  update velocities ***

	a=tan(thetav+alpha);

	dvz2=2*(vz1r+a*(cbeta*vx1r+sbeta*vy1r))/((1+a*a)*(1+m21));

	vz2r=dvz2;
	vx2r=a*cbeta*dvz2;
	vy2r=a*sbeta*dvz2;
	vz1r=vz1r-m21*vz2r;
	vx1r=vx1r-m21*vx2r;
	vy1r=vy1r-m21*vy2r;


	//     **** rotate the velocity vectors back and add the initial velocity
	//           vector of ball 2 to retrieve the original coordinate system ****

	vx1=ct*cp*vx1r-sp*vy1r+st*cp*vz1r +vx2;
	vy1=ct*sp*vx1r+cp*vy1r+st*sp*vz1r +vy2;
	vz1=ct*vz1r-st*vx1r               +vz2;
	vx2=ct*cp*vx2r-sp*vy2r+st*cp*vz2r +vx2;
	vy2=ct*sp*vx2r+cp*vy2r+st*sp*vz2r +vy2;
	vz2=ct*vz2r-st*vx2r               +vz2;



	return;
}
#endif

//////////////////////////////////////////////////////////////////////////

void Particle::Randomize()
{
#if 0
	Wind				= Vec3(Random(5.0f, -5.0f), 0.0f, 0.0f);
	Gravity				= Vec3(0.0f, Random(5.0f, -5.0f), 0.0f);

	//PolarVel			= Vec2(Random(5.0f, -5.0f), Random(5.0f, -5.0f));

	m_vAccel			= Vec2(Random(5.0f, -5.0f), Random(5.0f, -5.0f));
	//PolarAccel		= Vec2(Random(5.0f, -5.0f), Random(5.0f, -5.0f));

	m_pParent->PickupRange	= Random(400.0f, 1.0f);

	m_fSize				= Random((m_pParent->m_bPointSprites ? 50.0f : 4.0f), 1.0f);
	m_fShrinkRate		= Random(3.0f, 1.0f);

	m_cColor.Randomize();

	m_cDecay = Color(Random(20.0f, -20.0f), Random(20.0f, -20.0f), Random(20.0f, -20.0f));

	//True randomness :D
	m_nLife				= Random(Random(Random(50) + 5) + 5) + 5; 

	m_fRotation			= 0;//Random(360);
	m_fRotSpeed			= 0;//Random(20, -20);

	m_pParent->EmitDist	= RandomBool();
#endif
}

//////////////////////////////////////////////////////////////////////////

Float Particle::DistanceToCamera()
{
	return Distance(m_vPos, g_D3D->GetCamera().GetPosition());
}

//////////////////////////////////////////////////////////////////////////

void Particle::Draw()
{
	if (m_bRender)
	{ 	
		if (m_pParent->m_bApplyGravity)
		{
			CDynamicArray<Particle*>& list = m_pParent->sortedParticles;

			if (list.IsEmpty())
				list.Push(this);
			else
			{
				Float dist = DistanceToCamera();
				UInt32 nSize = list.Size();

				for (UInt32 i = 0; i < nSize; i++)
				{
					ASSERT(!BAD_PTR(list[i]));
					if (dist < list[i]->DistanceToCamera())
					{
						if (i == 0)
							list.PushFront(this);
						else
							list.Insert(this, i - 1);
					}
				}
			}

			//if (m_fSize == 1.0f)
				//m_pParent->m_RenderLists[0].Push(m_dwUniqueID);
 			//else if (m_fSize == 2.5f)
 			//	m_pParent->m_RenderLists[1].Push(m_dwUniqueID);
 			//else if (m_fSize == 5.0f)
 			//	m_pParent->m_RenderLists[2].Push(m_dwUniqueID);
 			//else /*if (m_fMass == 10.0f)*/
 			//	m_pParent->m_RenderLists[3].Push(m_dwUniqueID);
		}
 		else
 		{
			//UInt32 buffSize = m_pParent->m_RenderLists[0].BufferSize();
			//PROFILE("RenderList");
			//ASSERT(m_dwUniqueID < buffSize);
			m_pParent->m_RenderLists[0].Push(m_dwUniqueID);
		}
	}

	//g_SpriteMgr->DrawSprite(m_pParent->m_TexID[0], m_vPos.x, m_vPos.y, m_vPos.z, 0.0f, 1.0f, m_cColor);

	//if (parent->EnableTexture2)
	//g_SpriteMgr->DrawSprite(TexID[1], Pos.X, Pos.Y, Rot, NULL, RGB32(Alpha, Red, Green, Blue), Size, Size);

	//if (parent->EnableTexture3)
	//g_SpriteMgr->DrawSprite(TexID[2], Pos.X, Pos.Y, Rot, NULL, RGB32(Alpha, Red, Green, Blue), Size, Size);
}

//////////////////////////////////////////////////////////////////////////

Color Particle::PhysicsColoring(const Particle::ParticleState& state)
{
	Color cColor = m_cColor;

	Float fMult = m_pParent->m_fColorMultiplier;

	switch (mk_eColorMode)
	{
	case eCM_Custom:
		break;

	case eCM_Mass:
		cColor.ConvertFromHSV(m_fMass * fMult - fMult, 1.0f, 1.0f);
		break;

	case eCM_Charge:
		if (m_fElecCharge > 0)
			cColor = Color::km_Red /** m_fCharge*/;
		else if (m_fElecCharge < 0)
			cColor = Color::km_Blue /** fabsf(m_fCharge)*/;
		else
			cColor = Color::km_Gray;

		break;
		
	case eCM_Velocity:
		{
			Float vel = state.vel.Magnitude();
			cColor = m_pParent->m_Base.m_cColor.GetGradient(vel * fMult, m_cDecay);
			
			/*if (vel > 5.0f)
				break;*/
		}
		break;

	case eCM_Acceleration:
		{
			Float vel = state.vel.Magnitude();
			cColor = m_pParent->m_Base.m_cColor.GetGradient(vel * fMult, m_cDecay);
			
			if (vel > 100.0f)
				break;
		}
		break;
		
	case eCM_Momentum:
		{
			Float vel = state.vel.Magnitude();

			cColor.ConvertFromHSV(m_fMass * fMult - fMult, 1.0f, 1.0f);
			cColor = cColor.GetGradient(vel / 5.0f, Color::km_White);
		}
		break;

	case eCM_KineticEnergy:
		{
			Float e = Clamp(state.kineticEnergy * fMult, 0.0f, 360.0f);

			cColor.ConvertFromHSV(e, 1.0f, 1.0f);
			//cColor = m_pParent->m_Base.m_cColor.GetGradient(e / 5.0f, m_cDecay);
			
			if (e > 5.0f)
				break;
		}
		break;

	case eCM_PotentialEnergy:
		{
			Float e = Clamp(state.potentialEnergy * fMult, 0.0f, 360.0f);

			cColor.ConvertFromHSV(e, 1.0f, 1.0f);
		}
		break;

	default:
		break;
	}

	return cColor;
}

void Particle::Render(Effect* pFX /* = NULL */)
{
	D3DMatrix mat;
	D3DMaterial m;

	ZeroMemory(&m, sizeof(m));
	
	D3DXMatrixTranslation(&mat, m_vPos.x, m_vPos.y, m_vPos.z);

	m.Diffuse = m_cColor;
	m.Ambient = Color(m.Diffuse) * 0.25f;

	g_D3D->SetWorldMatrix(mat);
	g_D3D->GetDevice()->SetMaterial(&m);
	
	if (pFX)
	{
		pFX->Shader->SetMatrix("matWorld", &mat);
		pFX->Shader->SetVector("cParticleColor", &Vec4(m_cColor));

		pFX->Shader->BeginPass(0);
	}

	RenderMesh();

	if (pFX)
		pFX->Shader->EndPass();
}

void Particle::RenderMesh()
{
//#ifdef _DEBUG
	static UInt32 meshC = 0;
	if (!m_Mesh)
	{
		VERIFY(InitMesh());
		meshC++;
	}
//#endif
	m_Mesh->DrawSubset(0);
}