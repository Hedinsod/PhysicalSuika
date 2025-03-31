#pragma once

#include "PhScene.h"
#include <unordered_set>
#include <list>

#if 0
struct FManifoldKey
{
	FManifoldKey()
		: BodyA(nullptr)
		, BodyB(nullptr)
	{
	}

	FManifoldKey(CRigidBodyComp* A, CRigidBodyComp* B)
	{
		if (A < B)
		{
			BodyA = A;
			BodyB = B;
		}
		else
		{
			BodyA = B;
			BodyB = A;
		}
	}

	CRigidBodyComp* BodyA;
	CRigidBodyComp* BodyB;

	size_t operator()(const FManifoldKey& m) const
	{
		return std::hash<void*>()(m.BodyA) ^ (std::hash<void*>()(m.BodyB) << 1);
	}

	bool operator ==(const FManifoldKey& m) const
	{
		return m.BodyA == BodyA && m.BodyB == BodyB;
	}

};

struct FManifold_2
{
	CBodyHandle    BodyA = -1;
	CBodyHandle    BodyB = -1;
	float          Penetration = 0;
	glm::vec2      Normal{ 0.0f };
	float          Impulse = 0.0f;

	bool           bCollision = true;
};




class SContactGraph
{
public:


private:
	// sparse array ?
	std::unordered_set<FManifold_2> Contacts;
	std::list<FManifold_2> Contacts_2;

};
#endif

