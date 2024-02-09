// stdafx.cpp : source file that includes just the standard includes
// Quantum.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

CParticleEmitter* g_pRootEmitter	= NULL;
CParticleEmitter* g_pCurrentEmitter = NULL;
CParticleEmitter* g_pDefaultEmitter	= NULL;

//const float g_fFloatPrecision = 1000.0f;
//const float g_fInvFloatPrecision = 1 / g_fFloatPrecision;

Float	fCamDist	= 500.0f,
		fCamDir		= 270.0f,
		fCamAlt		= 90.0f;

Terrain grid;