#pragma once

#include "PhysicsComp.h"
#include <unordered_set>


extern const float GGravity;
extern const float GSlop;

struct FManifoldKey
{
	FManifoldKey()
		: BodyA(nullptr)
		, BodyB(nullptr)
	{ }

	FManifoldKey(CPhysicsComp* A, CPhysicsComp* B)
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

	CPhysicsComp* BodyA;
	CPhysicsComp* BodyB;

	size_t operator()(const FManifoldKey& m) const
	{
		return std::hash<void*>()(m.BodyA) ^ (std::hash<void*>()(m.BodyB) << 1);
	}

	bool operator ==(const FManifoldKey& m) const
	{
		return m.BodyA == BodyA && m.BodyB == BodyB;
	}

};

struct FManifold
{
	CPhysicsComp* BodyA = nullptr;
	CPhysicsComp* BodyB = nullptr;
	float         Penetration = 0;
	glm::vec2     Normal{ 0.0f };

	float Impulse = 0.0f;
};

class SCollisionCheck
{
public:
	SCollisionCheck();

	CPhysicsComp* AddPhysics(AActor* Owner, float InMass, float InRestitution);
	void RemovePhysics(CPhysicsComp* Comp);

	void Tick(float DeltaTime);

	void BroadPass();
	void NarrowPass();

private:
	bool SimpleCollision(const FBoxCollider& FirstAABB, const FBoxCollider& SecondAABB);
	bool GenManifold_BoxBox(FManifold& Manifold);
	bool GenManifold_BoxCircle(FManifold& Manifold);
	bool GenManifold_CircleBox(FManifold& Manifold);
	bool GenManifold_CircleCircle(FManifold& Manifold);
		
	void ResolveCollision(FManifold& Manifold);
	void PositionalCorrection(FManifold& Manifold);


	// All bodies of the scene
	std::unordered_set<CPhysicsComp*> Bodies;

	// Cache
	//std::vector<FManifold> Manifolds;
	std::unordered_map<FManifoldKey, FManifold, FManifoldKey> Manifolds;

	// no comments...
	std::function<bool(FManifold&)> DoubleVTable[2][2];

	float dt;
	
};
