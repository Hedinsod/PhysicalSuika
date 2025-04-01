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
	// Always add new Id
	CBodyHandle BodyId = (CBodyHandle)IdToIndex.size();
	int32_t NextFreeIndex = (int32_t)BodyPool.size();    // TODO: BodyPool.GetFreeIndex()

	IdToIndex.emplace(IdToIndex.begin() + BodyId, NextFreeIndex);
	BodyPool.emplace(BodyPool.begin() + NextFreeIndex, Owner, BodyId, InMaterialId, InShape, InLayers);

	return BodyId;
}

CRigidBodyComp& SPhyScene::GetRigidBody(CBodyHandle Handle)  // Why would anybody need that ?
{
	GAssert(Handle >= 0 && Handle < (CBodyHandle)IdToIndex.size());
	const int32_t Index = IdToIndex[Handle];

	GAssert(Index >= 0 && Index < BodyPool.size());
	return BodyPool[Index];
}

void SPhyScene::RemoveRigidBody(CBodyHandle Handle)
{
	// Deal with handle first
	uint32_t Index = IdToIndex[Handle];
	IdToIndex[Handle] = -1;

	// Swap and remove the body
	size_t LastIndex = BodyPool.size() - 1;
	if (Index != LastIndex)
	{
		std::swap(BodyPool[Index], BodyPool[LastIndex]);
		IdToIndex[BodyPool[Index].GetId()] = Index;
	}
	BodyPool.erase(BodyPool.end() - 1);
}

bool SPhyScene::SimpleCollision(const FBoxCollider& FirstAABB, const FBoxCollider& SecondAABB)
{
	if (FirstAABB.Max.x < SecondAABB.Min.x or FirstAABB.Min.x > SecondAABB.Max.x) return false;
	if (FirstAABB.Max.y < SecondAABB.Min.y or FirstAABB.Min.y > SecondAABB.Max.y) return false;

	return true;
}

void SPhyScene::Tick(float DeltaTimeMs)
{
	if (BodyPool.empty())
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
	for (int32_t i = 0; i < BodyPool.size(); i++)
	{
		CRigidBodyComp& First = BodyPool[i];
		for (int32_t j = i+1; j < BodyPool.size(); j++)
		{
			CRigidBodyComp& Second = BodyPool[j];

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
