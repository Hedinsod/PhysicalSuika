#pragma once

#include "RigidBodyComp.h"
#include <unordered_set>
#include <array>


// WIP: Will be replaced with contact graph nodes
struct FManifold
{
	mutable CBodyHandle  FirstId = -1;
	mutable CBodyHandle  SecondId = -1;

	mutable float        Penetration = 0;
	mutable glm::vec2    Normal{ 0.0f };
	mutable float        NormalImpulse = 0.0f;
	mutable float        TangentImpulse = 0.0f;
	mutable bool         bCollision = true;

	mutable std::array<glm::vec2, 2> ContactPoints;

	// hash table shenanigans
	size_t operator()(const FManifold& M) const
	{
		uint32_t One = M.FirstId < M.SecondId ? M.FirstId : M.SecondId;
		uint32_t Two = M.FirstId < M.SecondId ? M.SecondId : M.FirstId;

		return std::hash<uint32_t>()(One) ^ (std::hash<uint32_t>()(Two) << 1);
	}
	bool operator ==(const FManifold& Other) const
	{
		return (Other.FirstId == FirstId && Other.SecondId == SecondId)
			|| (Other.SecondId == FirstId && Other.FirstId == SecondId);
	}
};

class SPhySolver
{
public:
	SPhySolver();

	void AddPair(CRigidBodyComp& InFirst, CRigidBodyComp& InSecond);
	void WarmUp();
	void SolveContacts();

private:
	bool GenManifold_BoxBox(const FManifold& Manifold);
	bool GenManifold_BoxCircle(const FManifold& Manifold);
	bool GenManifold_CircleBox(const FManifold& Manifold);
	bool GenManifold_CircleCircle(const FManifold& Manifold);

	void ResolveCollision(const FManifold& Manifold);

private:
	// TODO: replace with contact graph
	std::unordered_set<FManifold, FManifold> Contacts;

	// Double virtual table of manifold generators
	std::function<bool(const FManifold&)> GenManifoldTable[2][2];

};