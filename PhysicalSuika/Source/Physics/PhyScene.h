#pragma once

#include "Core/Containers/SparseArray.h"
#include "Core/Timestep.h"
#include "RigidBodyComp.h"
#include "PhySolver.h"


//
// Manages all physical objects of the scene
// Has it's own solver to.. solve physics
class SPhyScene
{
public:
	SPhyScene(int32_t InStepsNumber);

	CBodyHandle CreateRigidBody(AActor* Owner, const std::string& MaterialTag, FColliderShape* InShape, uint32_t InLayers = 1);
	void RemoveRigidBody(CBodyHandle Handle);
	CRigidBodyComp& GetRigidBody(CBodyHandle Handle);

	void Tick(STimestep Step);
	void InternalTick(float DeltaTime);

	// Searching for potential collisions to give to solver
	void BroadPass();

private:
	// Detecting AABBs' intersections to form a pair
	bool SimpleCollision(const FBoxCollider& FirstAABB, const FBoxCollider& SecondAABB);

	// Number of sub-iterations during same render frame
	const int32_t StepsNumber = 0;

	// Physics solver
	std::unique_ptr<SPhySolver> Solver;

	// All bodies of the scene
	TSparseArray<CRigidBodyComp> BodyPool;

};
