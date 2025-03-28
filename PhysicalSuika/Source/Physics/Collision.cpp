#include "pch.h"

#include "Core/Application.h"
#include "Collision.h"
#include "Game/Game.h"
#include "Game/Actor.h"
#include "Renderer/Transform.h"

#include <queue>

CPhysicsComp* SCollisionCheck::AddPhysics(AActor* Owner, float InMass, float InRestitution)
{
	auto [It, bSuccess] = PhComps.emplace(new CPhysicsComp(Owner, InMass, InRestitution));
	GAssert(bSuccess);
	return *It;
}

void SCollisionCheck::RemovePhysics(CPhysicsComp* Comp)
{
	PhComps.erase(Comp);
	delete Comp;
}

bool SCollisionCheck::SimpleCollision(const FBoxCollider& FirstAABB, const FBoxCollider& SecondAABB)
{
	if (FirstAABB.Max.x < SecondAABB.Min.x or FirstAABB.Min.x > SecondAABB.Max.x) return false;
	if (FirstAABB.Max.y < SecondAABB.Min.y or FirstAABB.Min.y > SecondAABB.Max.y) return false;

	return true;
}

bool SCollisionCheck::GenManifold(FManifold& Manifold)
{
	CPhysicsComp* First = Manifold.BodyA;
	CPhysicsComp* Second = Manifold.BodyB;

	// TODO: GetShape() or something
	FBoxCollider BoxA = First->GenerateAABB();
	FBoxCollider BoxB = Second->GenerateAABB();

	glm::vec2 PosA = First->GetTransform().GetPos();
	glm::vec2 PosB = Second->GetTransform().GetPos();
	glm::vec2 Direction = PosB - PosA;

	glm::vec2 Overlap = (BoxA.Max - BoxA.Min) * 0.5f + (BoxB.Max - BoxB.Min) * 0.5f;
	Overlap.x -= glm::abs(Direction.x);
	Overlap.y -= glm::abs(Direction.y);

	if (Overlap.x > 0.f && Overlap.y > 0.f)
	{
		// Find out which axis is axis of least penetration 
		if (Overlap.x < Overlap.y)
		{
			// Point towards B knowing that n points from A to B 
			if (Direction.x < 0)
			{
				Manifold.Normal = glm::vec2(-1.f, 0.f);
			}
			else
			{
				Manifold.Normal = glm::vec2(1.f, 0.f);
			}
			Manifold.Penetration = Overlap.x;
			return true;
		}
		else
		{
			// Point toward B knowing that n points from A to B 
			if (Direction.y < 0)
			{
				Manifold.Normal = glm::vec2(0.f, -1.f);
			}
			else
			{
				Manifold.Normal = glm::vec2(0.f, 1.f);
			}
			Manifold.Penetration = Overlap.y;
			return true;
		}
	}

	return false;
}

void SCollisionCheck::ResolveCollision(FManifold& Manifold)
{
	glm::vec2 RelativeVelocity = Manifold.BodyB->Velocity - Manifold.BodyA->Velocity;

	// Calculate relative velocity in terms of the normal direction
	float VelAlongNormal = glm::dot(RelativeVelocity, Manifold.Normal);

	// Do not resolve if velocities are separating 
	if (VelAlongNormal > 0)
		return;

	// Calculate restitution 
	float e = glm::min(Manifold.BodyA->Restitution, Manifold.BodyB->Restitution);

	// Calculate impulse scalar 
	float ImpMagnitude = -(1 + e) * VelAlongNormal / (Manifold.BodyA->InvMass + Manifold.BodyB->InvMass);

	if (ImpMagnitude < 0.015)
		return;

	// Apply impulse 
	glm::vec2 Impulse = ImpMagnitude * Manifold.Normal;
	Manifold.BodyA->ApplyForce(-Impulse);
	Manifold.BodyB->ApplyForce(Impulse);
}


void SCollisionCheck::PositionalCorrection(FManifold& Manifold)
{
	constexpr float Density = 0.6f;
	float PenetrationMultiplier = 1 + Manifold.Penetration * Density;
	glm::vec2 ConterForce = Manifold.Normal * PenetrationMultiplier;

	Manifold.BodyA->ApplyForce(-ConterForce);
	Manifold.BodyB->ApplyForce(ConterForce);
}

void SCollisionCheck::Tick(float DeltaTimeMs)
{
	if (PhComps.empty())
		return;

	DeltaTimeMs /= 1000;

	BroadPass();
	NarrowPass();

	// Integration
	for (CPhysicsComp* Body : PhComps)
	{
		// Symplectic Euler 
		glm::vec2 Accel = Body->Forces * Body->InvMass;

		Body->Velocity += Accel;

		// 2. Update Position
		Body->GetOwner().GetTransform().Translate(Body->Velocity * DeltaTimeMs);
	}
}

void SCollisionCheck::BroadPass()
{
	Pairs.clear();
	Manifolds.clear();

	for (auto First = PhComps.begin(); First != PhComps.end(); First++)
	{
		// Reset Forces
		(*First)->Forces = (*First)->Gravity;

		for (auto Second = First; Second != PhComps.end(); Second++)
		{
			if (First == Second)
				continue;

			// TODO: Rigid  bodies don't interact with each other
			if (!((*First)->Layers & (*Second)->Layers))
				continue;

			FBoxCollider FirstBox = (*First)->GenerateAABB();
			FBoxCollider SecondBox = (*Second)->GenerateAABB();
			if (SimpleCollision(FirstBox, SecondBox))
			{
				Pairs.emplace_back(*First, *Second);

				auto& Manifold = Manifolds.emplace_back();
				Manifold.BodyA = *First;
				Manifold.BodyB = *Second;
			}
		}
	}

	std::sort(Pairs.begin(), Pairs.end());

	std::queue<std::pair<CPhysicsComp*, CPhysicsComp*>> UniquePairs;
	// Queue manifolds for solving 
	for (int i = 0; i < Pairs.size(); )
	{
		auto Pair = Pairs[i];

		UniquePairs.push(Pair);
		i++;

		// Skip duplicate pairs by iterating i until we find a unique pair 
		while (i < Pairs.size())
		{
			auto NextPair = Pairs[i];
			if (Pair.first != NextPair.second || Pair.second != NextPair.first)
				break;

			i++;
		}
	}
}

void SCollisionCheck::NarrowPass()
{
	for (FManifold& Manifold : Manifolds)
	{
		if (GenManifold(Manifold))
		{
			ResolveCollision(Manifold);
			PositionalCorrection(Manifold);
		}
	}
}

