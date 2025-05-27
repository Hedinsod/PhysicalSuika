#include "pch.h"

#include "Core/Application.h"
#include "PhyScene.h"
#include "Game/Game.h"
#include "Game/Actor.h"
#include "Systems/Transform.h"



SPhyScene::SPhyScene(float InTargetFrametime, uint32_t InSubStepsCount)
	: TargetFrametime(InTargetFrametime)
	, SubStepsCount(InSubStepsCount)
	, InvStepsCount(1.0f / InSubStepsCount)
{
	Solver = MakeScoped<SPhySolver>();
}

CBodyHandle SPhyScene::CreateRigidBody(AActor* Owner, const std::string& MaterialTag, FColliderShape* InShape, bool bInTrigger /*= false*/, uint32_t InLayers /*= 1*/)
{
	CBodyHandle BodyId = BodyPool.Emplace(Owner, MaterialTag, InShape, InLayers, bInTrigger);
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

void SPhyScene::Tick(float DeltaTime)
{
	if (BodyPool.Size() == 0)
		return;

	do // Allow physics to catch up
	{
		InternalTick(glm::min(TargetFrametime, DeltaTime));
		DeltaTime -= TargetFrametime;
	} while (DeltaTime >= TargetFrametime);
}

void SPhyScene::InternalTick(float StepTime)
{
	float Substep = StepTime * InvStepsCount;

	for (uint32_t i = 0; i < SubStepsCount; i++)
	{
		// 1. Integrate Forces & Gravity
		for (CRigidBodyComp& Body : BodyPool)
		{
			if (!Body.IsStatic() && !Body.IsDisabled())
			{
				Body.IntegrateVelocity(Substep);
			}
		}

		// 2. Solve collisions
		BroadPass();
		Solver->WarmUp(Substep);
		Solver->SolveContacts();

		// 3. Integrate Position
		for (CRigidBodyComp& Body : BodyPool)
		{
			if (!Body.IsStatic() && !Body.IsDisabled())
			{
				Body.IntegratePosition(Substep);
			}
		}
	}
}

void SPhyScene::BroadPass()
{
	for (auto It = BodyPool.begin(); It != BodyPool.end(); It++)
	{
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

			// Disabled bodies don't form pairs
			if (First.IsDisabled() || Second.IsDisabled())
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
