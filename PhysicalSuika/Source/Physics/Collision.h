#pragma once

#include "PhysicsComp.h"
#include <unordered_set>


struct FManifold
{
	CPhysicsComp* BodyA;
	CPhysicsComp* BodyB;
	float         Penetration;
	glm::vec2     Normal;
};

class SCollisionCheck
{
public:
	CPhysicsComp* AddPhysics(AActor* Owner, float InMass, float InRestitution);
	void RemovePhysics(CPhysicsComp* Comp);

	void Tick(float DeltaTime);

	void BroadPass();
	void NarrowPass();

private:
	bool SimpleCollision(const FBoxCollider& FirstAABB, const FBoxCollider& SecondAABB);
	bool GenManifold(FManifold& Manifold);
		
	void ResolveCollision(FManifold& Manifold); //CPhysicsComp& A, CPhysicsComp& B, const glm::vec2& CollisionVector);
	void PositionalCorrection(FManifold& Manifold);


	// All bodies of the scene
	std::unordered_set<CPhysicsComp*> PhComps;

	// Cache
	std::vector<std::pair<CPhysicsComp*, CPhysicsComp*>> Pairs;
	std::vector<FManifold> Manifolds;
};
