#pragma once

#include "Vec.h"
#include "Tree.h"
#include "DynamicArray.h"

#include "ProfileSample.h"

#include "VectorGraphics.h"

#include "MathFuncs.h"

enum EForce
{
	eForce_Gravity,
	eForce_Electric,
	eForce_Strong,
	eForce_Weak
};

template <typename Type>
class CForceMgr
{
	static const Bool mk_bGlobalGravity =	//true;
											false;

	CVectorGraphics		m_GravGrid;

public:
	CForceMgr(void) : f_Cg(/*9.8f*/-9.8f), m_bColorCollisions(false)
	{}
	~CForceMgr(void) {m_Objects.Clear();}

	void InitGrid()
	{
		m_GravGrid.Init();

		D3DXVertex

	}

	void RenderGrid()
	{
	}

	void Add(Type* pObj)
	{
		m_Objects.Push(pObj);
	}

	void Empty()
	{
		m_Objects.Clear();
	}

	Bool SphereIntersectsBox(Vec3 boxMin, Vec3 boxMax, Vec3 center, Float rad) const
	{
		D3DXPLANE plane;

		Vec3 p1	= boxMin,
			 p2	= boxMin + Vec3(0.0f, 0.0f, 50.0f),
			 p3	= boxMax;

		//for (UInt32 face = 0; face < 6; face++)
		{
			D3DXPlaneFromPoints(&plane, &p1, &p2, &p3);
			return (fabsf(D3DXPlaneDotNormal(&plane, &center)) <= rad);
		}
	}

	void Process(EForce eForceType)
	{
		//PROFILE_THIS;

		Type* ObjI = NULL, *ObjJ = NULL;

		m_vCenterOfMass = k_v3Zero;
		m_fTotalMass = 0.0f;

		Vec3 force, fwd;
		Float mag = 0.0f;
		Float d = 0.0f, d2 = 0.0f, fC_Gmi = 0.0f;

		Float	f1 = 0.0f,
				f2 = 0.0f;

		switch (eForceType)
		{
		case eForce_Gravity:
			f_Cg = 1.0f;//9.8f;
			break;

		case eForce_Electric:
			f_Cg = -3.0f * 9.8f;
			break;

		case eForce_Strong:
			f_Cg = -1.0f;
			break;
		}

		//Timer tprocess(true);

		for (UInt32 i = 0; i < m_Objects.Size(); i++)
		{
			force = k_v3Zero;

			ObjI = m_Objects[i];

			switch (eForceType)
			{
			case eForce_Gravity:
				if (!mk_bGlobalGravity)
				{
					f1 = ObjI->m_fMass;

					m_fTotalMass += f1;
					m_vCenterOfMass += f1 * ObjI->m_vPos;
				}
				break;

			case eForce_Electric:
				f1 = ObjI->m_fElecCharge;
				break;

			case eForce_Strong:
				if(ObjI->m_fElecCharge >= 0)
					f1 = 2.0f;//ObjI->m_fElecCharge;
				else
					f1 = 0.0f;
				break;
			}

			fC_Gmi = f_Cg * f1;

			if (mk_bGlobalGravity && 
					SphereIntersectsBox(Vec3(-50.0f, 1.0f, -50.0f), Vec3(50.0f, 1.0f, 50.0f), ObjI->m_vPos, ObjI->m_fSize * 0.15f)
					&& ObjI->m_vVel.y < 0.0f)
			{
				D3DXQUATERNION q;
				D3DMatrix mat;
				Vec3 norm =		k_v3YAxis;
								//Vec3(0.707f, 0.707f);
								//Vec3(0.866f, 0.5f);

				//								  energy lost to friction
				Vec3 vel = ObjI->m_vVel * -1.0f * 0.7f;

				D3DXQuaternionRotationAxis(&q, &norm, PI);
				D3DXMatrixRotationQuaternion(&mat, &q);

				D3DXVec3TransformCoord(&vel, &vel, &mat);
				
				//ObjI->m_vForceSum += vel;
				ObjI->m_vVel = vel;
			}

			for (UInt32 j = i + 1; j < m_Objects.Size(); j++)
			{
				ObjJ = m_Objects[j];

				d2 = DistanceSquared(ObjI->m_vPos, ObjJ->m_vPos);

				// 0.2 = 10% of each size (constant because of graphics)
				d = /*0.01f **/ (ObjI->m_fSize + ObjJ->m_fSize);

				d *= d;

				fwd = Vec3(ObjI->m_vPos + ObjI->m_vVel);

				//If collide with another particle
				if (d2 < d /*&& fwd.Dot(ObjJ->m_vPos) >= 0*/)
				{
					if (!mk_bGlobalGravity)
					{
						Vec3 temp = ObjI->m_vVel + ObjJ->m_vVel;

						//move particle back
						Vec3 axis = ObjJ->m_vPos - ObjI->m_vPos;
						axis.Normalize();

						Float amt = sqrt(d) - sqrt(d2);

						ObjI->m_vPos -= axis * amt;
						ObjJ->m_vPos += axis * amt;

						ObjI->m_vVel = temp * (1.0f / (ObjI->m_fMass + ObjJ->m_fMass));
						ObjJ->m_vVel = temp * (1.0f / (ObjI->m_fMass + ObjJ->m_fMass));

						if (m_bColorCollisions)
						{
							ObjI->m_cColor += Color(0.5f, 0.5f, 0.5f);
							ObjJ->m_cColor += Color(0.5f, 0.5f, 0.5f);
						}
					}
				}
				else //if (d2 < 400.0f)
				{
					switch (eForceType)
					{
					case eForce_Gravity:
						if (!mk_bGlobalGravity)
						{
							f2 = ObjJ->m_fMass;

							m_fTotalMass += f2;
							m_vCenterOfMass += f2 * ObjJ->m_vPos;
						}
						break;

					case eForce_Electric:
						f2 = ObjJ->m_fElecCharge;
						break;

					case eForce_Strong:
						if (ObjJ->m_fElecCharge >= 0 && d2 < 2500)
							f2 = -2.0f;//ObjJ->m_fElecCharge;
						else
							f2 = 0.0f;
						break;
					}

					mag = fC_Gmi * f2 / (d2 * 1.0f);

					force = ObjJ->m_vPos - ObjI->m_vPos;
					force.Normalize();
					//force /= d2;
					force *= mag;
					
					ObjI->m_vForceSum += force;
					ObjJ->m_vForceSum -= force;
				}
			}

			if (mk_bGlobalGravity)
				ObjI->m_vForceSum += Vec3(0.0f, -0.01f, 0.0f);

			//ObjI->m_vPos += sum;
			ObjI->m_vVel += (ObjI->m_vForceSum * (1.0f / ObjI->m_fMass)) * ObjI->m_pParent->m_fTimeMultiplier;
		}

		//Float ftime = tprocess.Get()

		if (eForceType == eForce_Gravity)
			m_vCenterOfMass *= 1.0f / m_fTotalMass;

		//Empty();
	}

	Float							f_Cg;
	
	Bool							m_bColorCollisions;

	Vec3							m_vCenterOfMass;
	Float							m_fTotalMass;

protected:
	CTree<Type*, 8>					m_Tree;
	CDynamicArray<Type*>			m_Objects;
};
