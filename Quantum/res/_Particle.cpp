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
#include "d3d.h"

//////////////////////////////////////////////////////////////////////////

//No gravity to begin with
Vec2 Gravity		= Vec2(0.0f, 0.01f);
//No wind either
Vec2 Wind		= Vec2(0.0f, 0.0f);

int Particle::ParticlesSoFar = 0;

//////////////////////////////////////////////////////////////////////////

//Set that particle up
Particle::Particle(float x, float y, float xv, float yv, float size, int red, int green, int blue, int alpha)
			:	Rot(0),//Random(360)),
				RotSpeed(0),
				Size(1),
				ShrinkRate(0.0f),
				Alpha(127),		Red(255),			Green(255),			Blue(255),
				AlphaDecay(0),	RedDecay(0),		GreenDecay(0),		BlueDecay(0)
{
	Life		= MAX_LIFE  + Random(MAX_LIFE / 2, -(MAX_LIFE / 4));
	Pos			= Vec2(x,y);
	Vel			= Vec2(xv, yv);
	PolarVel	= Vec2();	//No polar velocity
}

//////////////////////////////////////////////////////////////////////////

void Particle::Randomize()
{
	Wind				= Vec2(Random(5.0f, -5.0f), 0.0f);
	Gravity				= Vec2(0.0f, Random(5.0f, -5.0f));

	//PolarVel			= Vec2(Random(5.0f, -5.0f), Random(5.0f, -5.0f));

	Accel				= Vec2(Random(5.0f, -5.0f), Random(5.0f, -5.0f));
	//PolarAccel		= Vec2(Random(5.0f, -5.0f), Random(5.0f, -5.0f));

	parent->PickupRange	= Random(400.0f, 1.0f);

	Size				= Random((parent->PointSprites ? 50.0f : 4.0f), 1.0f);
	ShrinkRate			= Random(3.0f, 1.0f);

	Alpha				= Random(255);
	Red					= Random(255);
	Green				= Random(255);
	Blue				= Random(255);

	AlphaDecay			= Random(3, -3);
	RedDecay			= Random(20, -20);
	GreenDecay			= Random(20, -20);
	BlueDecay			= Random(20, -20);

	//True randomness :D
	Life				= Random(Random(Random(50) + 5) + 5) + 5; 

	Rot					= 0;//Random(360);
	RotSpeed			= 0;//Random(20, -20);

	parent->Explosion	= bool(Random(1));
}

//////////////////////////////////////////////////////////////////////////

void Particle::Draw()
{
	g_SpriteMgr->DrawSprite(TexID[0], Pos.X, Pos.Y, 0.09f, RGB32(Alpha, Red, Green, Blue));
	
	//if (parent->EnableTexture2)
		//g_SpriteMgr->DrawSprite(TexID[1], Pos.X, Pos.Y, Rot, NULL, RGB32(Alpha, Red, Green, Blue), Size, Size);

	//if (parent->EnableTexture3)
		//g_SpriteMgr->DrawSprite(TexID[2], Pos.X, Pos.Y, Rot, NULL, RGB32(Alpha, Red, Green, Blue), Size, Size);
}

//////////////////////////////////////////////////////////////////////////

void Particle::Reset()
{
	if (BAD_PTR(parent))
		return;			//parent = &pe;

	//Set the particle back to the "base" state
	*this				= parent->Base;

	Pos.X				+= Random(5.0f, -5.0f);
	Pos.Y				+= Random(5.0f, -5.0f);

	//Give a new speed and direction
	//Vel.SetSpeedDir(parent->Explosion ? Random(0.25f, 0.1666f) : PolarVel.X, Random(360.0f));
	float speed = Random(0.25f, 0.1666f),
			dir = Random(360.0f);
	Vel.SetSpeedDir(speed, dir);

	Vel += parent->Base.Vel;
	
	//Set a life amount
	Life				+= Random(Life / 2, -(Life/ 4));

	fDelay			= Random(0.5f, 0.0f);

	if (Life <= 0)
		Life = 1;
}

//////////////////////////////////////////////////////////////////////////

void Particle::Handle()
{
	if (!bReady)
	{
		if (DelayTimer.Get() >= fDelay)
			bReady = true;
		else
			return;
	}

	Vel				+= Accel + (parent->bApplyGravity ? Gravity : Vec2()) + (parent->bApplyWind ? Wind : Vec2());

	Vec2 temp;

	//PolarVel		+=	PolarAccel;

    //Get the distance between the base and us
//	float dist = Distance<float>(Pos, parent->Base.Pos);
//
//	//Check for polar velocity and proper range
//	if ((!PolarVel.X && !PolarVel.Y) || Round(dist) < parent->PickupRange)
//	{
//		Pos += Vel;
//		goto skipPolar;
//	}
//
//	if (parent->bUseSinRV)
//		PolarVel.X		*= 2 * sin(dist * /*0.25f **/ Deg2Rad);
//
//	if (parent->bUseSinAV)
//		PolarVel.Y		= 2 * sin(dist * 5 * Deg2Rad);
//
//	//Get the angle between us and the base
//	float angle =	atan(fabs(Pos.GetY() - parent->Base.Pos.Y) / fabs(Pos.GetX() - parent->Base.Pos.X)) * Rad2Deg;
//
//	//X and Y distances between the base and us
//	int	dx = parent->Base.Pos.X - Pos.GetX(),
//			dy = parent->Base.Pos.Y - Pos.GetY();
//
//	//Adjust the angle for the other three quadrants
//	if (dx <= 0 && dy >= 0)
//		angle = 180.0f - angle;
//	else if (dx <= 0 && dy <= 0)
//		angle = 180.0f + angle;
//	else if (dx >= 0 && dy <= 0)
//		angle = 360.0f - angle;
//
//	temp				= Vec2(dist, angle);
//	temp				+=		PolarVel;
//	
//	//Make sure that the values aren't infinitesimally small, so round up or down
//	Round(temp.X);
//	Round(temp.Y);
//
//	temp.SetSpeedDir(temp.X, temp.Y);
//
//	Pos				= parent->Base.Pos + temp;
//
	Pos				+=	Vel;
//
//skipPolar:

	/*if (Size > 0.1)
		Size			-= ShrinkRate / 20;*/

	//Make sure acceleration isn't constantly applied
	Accel				*= 0.9f;
	//PolarAccel		*= 0.9f;

	//Rot				+= RotSpeed;

	//Do color decay
	//You can write your own bounds checking for some COOL effects
	if (Alpha >= AlphaDecay)// && Alpha <= 255 + AlphaDecay)
		Alpha	-= AlphaDecay;
	if (Red >= RedDecay)// && Red <= 255 + RedDecay)
		Red		-= RedDecay;
	if (Green >= GreenDecay)// && Green <= 255 + GreenDecay)
		Green	-= GreenDecay;
	if (Blue >= BlueDecay)// && Blue <= 255 + BlueDecay)
		Blue		-= BlueDecay;

	//Check to see if our life is over or we're totally transparent
	if (Life-- <= 0 || Alpha <= 0)// || Pos.X + (Size * 32) < 0 || Pos.X > SCW || Pos.Y + (Size * 32) < 0 || Pos.Y > SCH)
	{
		parent->NumActiveParticles--;

		if (!parent->GoOneTime)
			Reset();
	}

	//If we're not a point sprite draw us
	if (!(parent->PointSprites))
		Draw();
}

//////////////////////////////////////////////////////////////////////////

//Renders the point g_SpriteMgr every frame
void ParticleEmitter::RenderPS()
{
	LPDIRECT3DDEVICE9	dev = g_D3D->GetDevice();
	
	//Set the texture
	dev->SetTexture(0, g_SpriteMgr->GetTexture(Base.TexID[0]));
	
	//Set all point sprite flags
	dev->SetRenderState(D3DRS_ZWRITEENABLE, false);
	dev->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	dev->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	dev->SetRenderState(D3DRS_POINTSIZE,	FToDW(Base.Size));
	dev->SetRenderState(D3DRS_POINTSIZE_MIN, FToDW(1.0f));
	dev->SetRenderState(D3DRS_POINTSCALE_A, FToDW(0.0f));
	dev->SetRenderState(D3DRS_POINTSCALE_B, FToDW(0.0f));
	dev->SetRenderState(D3DRS_POINTSCALE_C, FToDW(1.0f));

	dev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	Vertex	*pv;
	Particle particle;
	DblLinkedList<Particle>::Iterator		pt(particles);

	//Lock the vertex buffer for vertex adjustment
	if (FAILED(m_pVertexBuffer->Lock(0, NumParticles * sizeof(Vertex), (void**)&pv, D3DLOCK_DISCARD)))
	{
		//MessageBox(NULL, "Can't lock buffer!", "", MB_OK);
		return;
	}

	pt.Begin();

	//Run through the particle list
	for (int p = 0; p < NumParticles && !pt.End(); p++, pv++, ++pt)
	{
		particle = pt.Current();

		//Handle the current particle
		particle.Handle();

		//Set up the current "vertex" for the point sprite
		//D3DCOLOR structs use colors not from 0-255, but rather 0.0f to 1.0f, so be sure to use the COLOR_ADJ
		pv->color		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR(particle.Red / COLOR_ADJ, particle.Green / COLOR_ADJ, particle.Blue / COLOR_ADJ, particle.Alpha / COLOR_ADJ);
		pv->position	= D3DXVECTOR3(particle.Pos.X, particle.Pos.Y, 0.0f);
	}

	m_pVertexBuffer->Unlock();

	//Set which vertex buffer to run through for rendering
	dev->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(Vertex));
	
	//Set the format flags
	dev->SetFVF(Vertex::FVF);
	
	//Draw the point g_SpriteMgr
	dev->DrawPrimitive(D3DPT_POINTLIST, 0, NumParticles);
	
	//Turn off the point sprite only flags
	dev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	dev->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	dev->SetRenderState(D3DRS_POINTSCALEENABLE, false);

	dev->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

//////////////////////////////////////////////////////////////////////////

void ParticleEmitter::InitParticles()
{
	DblLinkedList<Particle>::Iterator	i(particles);

	for (i.Begin(); !i.End(); ++i)
		i.Current().Reset();
}

//////////////////////////////////////////////////////////////////////////

void ParticleEmitter::InitEmitter()
{
	//Create the vertex buffer
	g_D3D->GetDevice()->CreateVertexBuffer(MAX_PARTICLES * sizeof(Vertex), 
										D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS, 
										Vertex::FVF, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);

	fMaxPointSize = 0.0f;
	bDeviceSupportsPSIZE = false;
	
    D3DCAPS9 d3dCaps;
	g_D3D->GetDevice()->GetDeviceCaps(&d3dCaps);

	//Check if hardware supports point g_SpriteMgr
    if (d3dCaps.FVFCaps & D3DFVFCAPS_PSIZE)
        bDeviceSupportsPSIZE = true;
    else
        bDeviceSupportsPSIZE = false;

	fMaxPointSize = d3dCaps.MaxPointSize;

	//Set the texture
	Base.TexID[0]	= g_SpriteMgr->LoadSprite(PointSprites ? "Particle.bmp" : "Dot.bmp");

	Particle pt;
	float speed;

	//Run through and initialize all possible particles, we'll trim the count down later
	for (int i = 0; i < NumParticles; i++)
	{
		//Create a new particle
		pt = Particle(Base);

	
		if (Explosion)
			speed = Random(Base.Vel.X, 0.25f);
		else
			speed = Base.PolarVel.X;
		
		pt.Vel.SetSpeedDir(speed, Random(360.0f));
		
		//Add the particle to the list
		particles.Add(pt);
	}

	NumActiveParticles = NumParticles;
}

//////////////////////////////////////////////////////////////////////////

void ParticleEmitter::Handle()
{
	//if (!Active)
	//	return;

	//Protect against bad pointers to the parent (I've had my share of these)
	ASSERT(!BAD_PTR(Base.parent));

// 	if (Active && NumActiveParticles <= 0)
// 	{
// 		Active = false;
// 		return;
// 	}

	DblLinkedList<Particle>::Iterator		p(particles);
	
	if (PointSprites)
		RenderPS();		//Point sprites are handled in here
	else
	{
		//D3DXMATRIX		tmat;
		
		//D3DXMatrixTranslation(&tmat, g_Game->player->m_vPosition.X, g_Game->player->m_vPosition.Y, 0.0f);
		//D3DXMatrixScaling(&tmat, 5.0f, 5.0f, 1.0f);
		//tmat[0][0] = tmat[1][1] = tmat[2][2] = 5.0f;
		//tmat._11 = 1.0f;
		//tmat._22 = 1.0f;
		//tmat._33 = 1.0f;

		//g_D3D->GetSpriteObject()->SetTransform(&tmat);

		//Otherwise, do it here
		int particle = 0;

		for (p.Begin(); 
				particle < NumParticles; 
				particle++, ++p)
		{
			Particle::ParticlesSoFar++;

			if (Particle::TooManyParticles())
				return;

			p.Current().Handle();
		}

		//D3DXMatrixIdentity(&tmat);

		//g_D3D->GetSpriteObject()->SetTransform(&tmat);
	}
}

//////////////////////////////////////////////////////////////////////////