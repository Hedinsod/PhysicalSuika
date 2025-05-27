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
	/*
	* TargetFrametime - 1.0f / TargetFPS in seconds
	* SubStepsCount - number of iteration per (target) frame.
	*/
	SPhyScene(float InTargetFrametime, uint32_t InSubStepsCount);

	CBodyHandle CreateRigidBody(AActor* Owner, const std::string& MaterialTag, FColliderShape* InShape, bool bInTrigger = false, uint32_t InLayers = 1);
	void RemoveRigidBody(CBodyHandle Handle);
	CRigidBodyComp& GetRigidBody(CBodyHandle Handle);

	void Tick(float DeltaTime);

	inline void SetOnCollisionEventHandler(const FPhyEvent_OnCollision::EventCallbackFn& InCallback)
	{
		Solver->SetOnCollisionEventHandler(InCallback);
	}

private:
	// Detecting AABBs' intersections to form a pair
	bool SimpleCollision(const FBoxCollider& FirstAABB, const FBoxCollider& SecondAABB);

private:
	void InternalTick(float StepTime);
	// Searching for potential collisions to give to solver
	void BroadPass();

	// Number of sub-iterations during same render frame
	const uint32_t SubStepsCount;
	const float InvStepsCount;
	// Target frametime in seconds
	const float TargetFrametime;

	// Physics solver
	StdScoped<SPhySolver> Solver;

	// All bodies of the scene
	TSparseArray<CRigidBodyComp> BodyPool;

};
