#pragma once

#include "RigidBodyComp.h"
#include "ContactGraph.h"
#include <array>


class SPhySolver
{
public:
	SPhySolver();

	inline void AddPair(CRigidBodyComp& InFirst, CRigidBodyComp& InSecond)
	{
		ContactGraph.AddContact(InFirst, InSecond);
	}

	void WarmUp();
	void SolveContacts();

private:
	bool GenManifold_BoxBox(CRigidBodyComp& First, CRigidBodyComp& Second, FManifold& Manifold);
	bool GenManifold_BoxCircle(CRigidBodyComp& First, CRigidBodyComp& Second, FManifold& Manifold);
	bool GenManifold_CircleBox(CRigidBodyComp& First, CRigidBodyComp& Second, FManifold& Manifold);
	bool GenManifold_CircleCircle(CRigidBodyComp& First, CRigidBodyComp& Second, FManifold& Manifold);

	void ResolveCollision(CRigidBodyComp& First, CRigidBodyComp& Second, FManifold& Manifold);

private:
	// 
	FContactGraph ContactGraph;

	// Double virtual table of manifold generators
	std::function<bool(CRigidBodyComp& First, CRigidBodyComp& Second, FManifold&)> GenManifoldTable[2][2];

};