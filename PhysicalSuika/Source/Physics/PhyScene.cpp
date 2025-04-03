#include "pch.h"

#include "Core/Application.h"
#include "PhyScene.h"
#include "Game/Game.h"
#include "Game/Actor.h"
#include "Renderer/Transform.h"



SPhyScene::SPhyScene(int32_t InStepsNumber)
	: StepsNumber(InStepsNumber)
{
	Solver = std::make_unique<SPhySolver>();
}

CBodyHandle SPhyScene::CreateRigidBody(AActor* Owner, uint32_t InMaterialId, FColliderShape* InShape, uint32_t InLayers /*= 1*/)
{
	CBodyHandle BodyId = BodyPool.Emplace(Owner, InMaterialId, InShape, InLayers);
	BodyPool[BodyId].SetId(BodyId);

	return BodyId;
}

CRigidBodyComp& SPhyScene::GetRigidBody(CBodyHandle Handle)  // Why would anybody need that ?
{
	return BodyPool[Handle];
}

void SPhyScene::RemoveRigidBody(CBodyHandle Handle)
{
	// Just remove it from sparse array as it is perfectly stable index-wise
	BodyPool.Remove(Handle);
}

bool SPhyScene::SimpleCollision(const FBoxCollider& FirstAABB, const FBoxCollider& SecondAABB)
{
	if (FirstAABB.Max.x < SecondAABB.Min.x or FirstAABB.Min.x > SecondAABB.Max.x) return false;
	if (FirstAABB.Max.y < SecondAABB.Min.y or FirstAABB.Min.y > SecondAABB.Max.y) return false;

	return true;
}

void SPhyScene::Tick(float DeltaTimeMs)
{
	if (BodyPool.Size() == 0)
		return;

	// TODO: should physics run sub-steps & catch up on its own?
	// Convert Ms to S
	float TimeStep = DeltaTimeMs / (1000 * StepsNumber);

	const float Gravity = 9.8f;
	for (int32_t i = 0; i < StepsNumber; i++)
	{
		// 1. Integrate Forces & Gravity
		for (CRigidBodyComp& Body : BodyPool)
		{
			if (!Body.IsStatic())
			{
				Body.IntegrateVelocity(TimeStep);
			}
		}

		// 2. Solve collisions
		BroadPass();
		Solver->WarmUp();
		Solver->SolveContacts();

		// 3. Integrate Position
		for (CRigidBodyComp& Body : BodyPool)
		{
			if (!Body.IsStatic())
			{
				Body.IntegratePosition(TimeStep);
			}
		}
	}
}

void SPhyScene::BroadPass()
{
	//for (int32_t i = 0; i < BodyPool.Size(); i++)
	for (auto It = BodyPool.begin(); It != BodyPool.end(); It++)
	{
		//for (int32_t j = i+1; j < BodyPool.Size(); j++)
		for (auto Jt = BodyPool.begin(); Jt != BodyPool.end(); Jt++)
		{
			if (It == Jt)
				continue;

			CRigidBodyComp& First = *It;
			CRigidBodyComp& Second = *Jt;

			// TODO: Do I even need Layers?
			if (!(First.GetLayers() & Second.GetLayers()))
				continue;

			// Skip static pairs
			if (First.IsStatic() && Second.IsStatic())
				continue;

			const FBoxCollider FirstBox = First.GenerateAABB();
			const FBoxCollider SecondBox = Second.GenerateAABB();
			if (SimpleCollision(FirstBox, SecondBox))
			{
				// Suspect contact
				Solver->AddPair(First, Second);
			}
		}
	}
}
