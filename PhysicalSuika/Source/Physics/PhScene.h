#pragma once

#include "RigidBodyComp.h"
#include <unordered_set>


struct FManifold
{
	mutable CBodyHandle  FirstId = -1;
	mutable CBodyHandle  SecondId = -1;

	mutable float        Penetration = 0;
	mutable glm::vec2    Normal{ 0.0f };
	mutable float        Impulse = 0.0f;
	mutable bool         bCollision = true;

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


class SPhScene
{
public:
	SPhScene(int32_t InStepsNumber);

	CBodyHandle CreateRigidBody(AActor* Owner, const FRigidBodyDesc& Desc);
	void RemoveRigidBody(CBodyHandle Handle);
	CRigidBodyComp& GetRigidBody(CBodyHandle Handle);

	void Tick(float DeltaTime);

	// Internal stuff
	void BroadPass();
	void NarrowPass();

private:
	bool SimpleCollision(const FBoxCollider& FirstAABB, const FBoxCollider& SecondAABB);
	bool GenManifold_BoxBox(const FManifold& Manifold);
	bool GenManifold_BoxCircle(const FManifold& Manifold);
	bool GenManifold_CircleBox(const FManifold& Manifold);
	bool GenManifold_CircleCircle(const FManifold& Manifold);
		
	void ResolveCollision(const FManifold& Manifold);

	const int32_t StepsNumber = 0;

	// All bodies of the scene
	std::vector<int32_t> IdToIndex;
	std::vector<CRigidBodyComp> BodyPool;

	// TODO: replace with contact graph
	std::unordered_set<FManifold, FManifold> Contacts;

	// Double virtual table of manifold generators
	std::function<bool(const FManifold&)> GenManifoldTable[2][2];

};
