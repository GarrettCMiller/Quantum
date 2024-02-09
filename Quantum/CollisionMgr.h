#pragma once

#include "Queue.h"
//#include "Functor.h"

template <typename Type>
class CCollisionMgr
{
public:
	CCollisionMgr(void) {}
	~CCollisionMgr(void) {m_Objects.Clear();}

	void Add(Type* pObj)
	{
		m_Objects.Enqueue(pObj);
	}

	void Empty()
	{
		m_Objects.Clear();
	}

	void Process()
	{
		Type* pCurrent = NULL;

		while (!m_Objects.IsEmpty())
		{
			m_Objects.Dequeue(pCurrent);

			Queue<Type*>::Iterator it(m_Objects);

			for (it.Begin(); !it.End(); ++it)
			{
				//if (pCurrent->m_pfnCollision()
				if (pCurrent->Collision(it.Current()))
				{
					pCurrent->OnCollision(it.Current());
					//it.Current()->OnCollision(pCurrent);
				}
			}
		}
	}
protected:
	Queue<Type*>					m_Objects;
	//CFunctor1<Type, int, Type*>		m_pfnOnCollision;
	//CFunctor1<Type, bool, Type*>	m_pfnCollision;
};
