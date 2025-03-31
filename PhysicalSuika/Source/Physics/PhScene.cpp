#include "pch.h"

#include "Core/Application.h"
#include "PhScene.h"
#include "Game/Game.h"
#include "Game/Actor.h"
#include "Renderer/Transform.h"

#include <concepts>


SPhScene::SPhScene(int32_t InStepsNumber)
	: StepsNumber(InStepsNumber)
{
	GenManifoldTable[0][0] = std::bind(&SPhScene::GenManifold_BoxBox, this, std::placeholders::_1);
	GenManifoldTable[0][1] = std::bind(&SPhScene::GenManifold_BoxCircle, this, std::placeholders::_1);
	GenManifoldTable[1][0] = std::bind(&SPhScene::GenManifold_CircleBox, this, std::placeholders::_1);
	GenManifoldTable[1][1] = std::bind(&SPhScene::GenManifold_CircleCircle, this, std::placeholders::_1);
}

CBodyHandle SPhScene::CreateRigidBody(AActor* Owner, const FRigidBodyDesc& Desc)
{
	// Always add new Id
	CBodyHandle BodyId = (CBodyHandle)IdToIndex.size();
	int32_t NextFreeIndex = (int32_t)BodyPool.size();    // TODO: BodyPool.GetFreeIndex()

	IdToIndex.emplace(IdToIndex.begin() + BodyId, NextFreeIndex);
	BodyPool.emplace(BodyPool.begin() + NextFreeIndex, Owner, BodyId, Desc);

	return BodyId;
}

CRigidBodyComp& SPhScene::GetRigidBody(CBodyHandle Handle)  // Why would anybody need that ?
{
	GAssert(Handle >= 0 && Handle < (CBodyHandle)IdToIndex.size());
	const int32_t Index = IdToIndex[Handle];

	GAssert(Index >= 0 && Index < BodyPool.size());
	return BodyPool[Index];
}

void SPhScene::RemoveRigidBody(CBodyHandle Handle)
{
	// Deal with handle first
	uint32_t Index = IdToIndex[Handle];
	IdToIndex[Handle] = -1;

	// Swap and remove the body
	size_t LastIndex = BodyPool.size() - 1;
	if (Index != LastIndex)
	{
		std::swap(BodyPool[Index], BodyPool[LastIndex]);
		IdToIndex[BodyPool[Index].Id] = Index;
	}
	BodyPool.erase(BodyPool.end() - 1);
}

bool SPhScene::SimpleCollision(const FBoxCollider& FirstAABB, const FBoxCollider& SecondAABB)
{
	if (FirstAABB.Max.x < SecondAABB.Min.x or FirstAABB.Min.x > SecondAABB.Max.x) return false;
	if (FirstAABB.Max.y < SecondAABB.Min.y or FirstAABB.Min.y > SecondAABB.Max.y) return false;

	return true;
}

bool SPhScene::GenManifold_BoxBox(const FManifold& Manifold)
{
	CRigidBodyComp& First = GetRigidBody(Manifold.FirstId);
	CRigidBodyComp& Second = GetRigidBody(Manifold.SecondId);

	const FBoxCollider& FirstBox = First.GetCollider<FBoxCollider>();
	const FBoxCollider& SecondBox = Second.GetCollider<FBoxCollider>();

	glm::vec2 FirstPos = First.GetTransform().GetPos() + First.Shape->Pivot;
	glm::vec2 SecondPos = Second.GetTransform().GetPos() + Second.Shape->Pivot;
	glm::vec2 Direction = SecondPos - FirstPos;

	glm::vec2 Overlap = (FirstBox.Max - FirstBox.Min) * 0.5f + (SecondBox.Max - SecondBox.Min) * 0.5f;
	Overlap -= glm::abs(Direction);

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

			Manifold.bCollision = true;
			return Manifold.bCollision;
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

			Manifold.bCollision = true;
			return Manifold.bCollision;

		}
	}

	Manifold.bCollision = false;
	return Manifold.bCollision;
}
bool SPhScene::GenManifold_CircleBox(const FManifold& Manifold)
{
	// Interesting trick
	CBodyHandle Temp = Manifold.FirstId;
	Manifold.FirstId = Manifold.SecondId;
	Manifold.SecondId = Temp;

	return GenManifold_BoxCircle(Manifold);
}
bool SPhScene::GenManifold_BoxCircle(const FManifold& Manifold)
{
	CRigidBodyComp& First = GetRigidBody(Manifold.FirstId);
	CRigidBodyComp& Second = GetRigidBody(Manifold.SecondId);

	// Calculate Shapes
	glm::vec2 FirstPos = First.GetTransform().GetPos() + First.Shape->Pivot;
	glm::vec2 SecondPos = Second.GetTransform().GetPos() + Second.Shape->Pivot;

	// Alarm: actually calculating shape for the second time!
	FBoxCollider Box = First.GetCollider<FBoxCollider>();
	Box.Min += FirstPos;
	Box.Max += FirstPos;
	const FCircleCollider& Circle = Second.GetCollider<FCircleCollider>();

	// 
	glm::vec2 Direction = SecondPos - FirstPos;
	glm::vec2 Closest = Direction;

	// Calculate half extents along each axis 
	float ExtentX = (Box.Max.x - Box.Min.x) / 2;
	float ExtentY = (Box.Max.y - Box.Min.y) / 2;

	// Clamp point to edges of the AABB 
	Closest.x = glm::clamp(Closest.x, -ExtentX, ExtentX);
	Closest.y = glm::clamp(Closest.y, -ExtentY, ExtentY);

	bool bInside = false;

	// Circle center is inside the AABB, so we need to clamp the circle's center 
	// to the closest edge 
	if (Direction == Closest)
	{
		bInside = true;

		// Find closest axis 
		if (abs(Direction.x) > abs(Direction.y))
		{
			// Clamp to closest extent 
			if (Closest.x > 0)
				Closest.x = ExtentX;
			else
				Closest.x = -ExtentX;
		}
		else    // y axis is shorter 
		{
			// Clamp to closest extent 
			if (Closest.y > 0)
				Closest.y = ExtentY;
			else
				Closest.y = -ExtentY;
		}
	}

	glm::vec2 Normal = Direction - Closest;
	float d = glm::dot(Normal, Normal);
	float r = Circle.Radius;

	// Early out of the radius is shorter than distance to closest point and 
	// Circle not inside the AABB 
	if (d > r * r && !bInside)
	{
		Manifold.bCollision = false;
		return Manifold.bCollision;
	}

	// Avoided sqrt until we needed 
	d = glm::sqrt(d);

	// Collision normal needs to be flipped to point outside if circle was 
	// inside of the AABB 
	if (bInside)
	{
		Manifold.Normal = glm::normalize(-Normal);
		Manifold.Penetration = r - d;
	}
	else
	{
		Manifold.Normal = glm::normalize(Normal);
		Manifold.Penetration = r - d;
	}

	Manifold.bCollision = true;
	return Manifold.bCollision;
}
bool SPhScene::GenManifold_CircleCircle(const FManifold& Manifold)
{
	// Calculate Shapes
	CRigidBodyComp& First = GetRigidBody(Manifold.FirstId);
	CRigidBodyComp& Second = GetRigidBody(Manifold.SecondId);

	const FCircleCollider& FirstCircle = First.GetCollider<FCircleCollider>();
	const FCircleCollider & SecondCircle = Second.GetCollider<FCircleCollider>();

	glm::vec2 PosA = First.GetTransform().GetPos() + FirstCircle.Pivot;
	glm::vec2 PosB = Second.GetTransform().GetPos() + SecondCircle.Pivot;

	//
	glm::vec2 Direction = PosB - PosA;
	float Radius = FirstCircle.Radius + SecondCircle.Radius;
	float Radius2 = Radius * Radius;

	float Dist2 = glm::dot(Direction, Direction);
	if (Dist2 > Radius2)
	{
		Manifold.bCollision = false;
		return Manifold.bCollision;
	}

	// Circles have collided, now compute manifold 
	float Dist = glm::length(Direction);
	if (Dist != 0)
	{
		Manifold.Penetration = Radius - Dist;
		Manifold.Normal = glm::normalize(Direction);

		Manifold.bCollision = true;
		return Manifold.bCollision;
	}
	else    // Circles are on same position 
	{
		// Choose random (but consistent) values
		Manifold.Penetration = FirstCircle.Radius;
		Manifold.Normal = glm::vec2(1, 0);
		Manifold.bCollision = true;
		return Manifold.bCollision;
	}
}

void SPhScene::ResolveCollision(const FManifold& Manifold)
{
	CRigidBodyComp& First = GetRigidBody(Manifold.FirstId);
	CRigidBodyComp& Second = GetRigidBody(Manifold.SecondId);

	// 
	// float Bias = -0.2f * 1/dt * glm::min(0.0f, 0.01f - Manifold.Penetration);

	// Calculate relative velocity in terms of the normal direction
	glm::vec2 RelativeVelocity = Second.Velocity - First.Velocity;
	float VelAlongNormal = glm::dot(RelativeVelocity, Manifold.Normal);

	// Calculate impulse scalar 
	float Restitution = glm::min(First.PhyMat.Restitution, Second.PhyMat.Restitution);
	float CommonMass = 1 / (First.PhyMat.InvMass + Second.PhyMat.InvMass);
	float Impulse = -(1 + Restitution) * VelAlongNormal * CommonMass /* + Bias */;

	// Accumulate and clump
	float OldImpulse = Manifold.Impulse;
	Manifold.Impulse = glm::max(OldImpulse + Impulse, 0.f);
	Impulse = Manifold.Impulse - OldImpulse;

	// Apply
	First.Velocity -= Impulse * First.PhyMat.InvMass * Manifold.Normal;
	Second.Velocity += Impulse * Second.PhyMat.InvMass * Manifold.Normal;
	
	// Friction
	// Recalculate velocity
	RelativeVelocity = Second.Velocity - First.Velocity;
	glm::vec2 Tangent = RelativeVelocity - glm::dot(RelativeVelocity, Manifold.Normal) * Manifold.Normal;
	if (glm::dot(Tangent, Tangent) > 0)
	{
		glm::vec2 TangentNormal = glm::normalize(Tangent);
		float Friction = -glm::dot(TangentNormal, RelativeVelocity);
		Friction /= (Second.PhyMat.InvMass + First.PhyMat.InvMass);

		float StaticFriction = (First.PhyMat.StaticFriction + Second.PhyMat.StaticFriction) / 2;
		if (abs(Friction) > Impulse * StaticFriction)
		{
			float DynamicFriction = (First.PhyMat.DynamicFriction + Second.PhyMat.DynamicFriction) / 2;
			Friction = -Impulse * DynamicFriction;
		}

		First.ApplyForce(-Friction * TangentNormal);
		Second.ApplyForce(Friction * TangentNormal);
	}
}

void SPhScene::Tick(float DeltaTimeMs)
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
			if (Body.PhyMat.InvMass == 0)
				continue;

			// Symplectic Euler 
			glm::vec2 Acceleration = Body.Forces * Body.PhyMat.InvMass + glm::vec2(0.f, -Gravity) * Body.PhyMat.GravityScale;
			Body.Velocity += Acceleration * TimeStep;
		}

		// 2. Solve collisions
		BroadPass();
		// Warm up ?
		NarrowPass();

		// 3. Integrate Position
		for (CRigidBodyComp& Body : BodyPool)
		{
			if (Body.PhyMat.InvMass == 0)
				continue;
			
			Body.GetOwner().GetTransform().Translate(Body.Velocity * TimeStep);
		}
	}
}

void SPhScene::BroadPass()
{
	for (int32_t i = 0; i < BodyPool.size(); i++)
	{
		CRigidBodyComp& First = BodyPool[i];
		for (int32_t j = i+1; j < BodyPool.size(); j++)
		{
			CRigidBodyComp& Second = BodyPool[j];

			// TODO: Do I even need Layers?
			if (!(First.Layers & Second.Layers))
				continue;

			// Skip static bodies
			if (First.PhyMat.InvMass == 0 && Second.PhyMat.InvMass == 0)
				continue;

			const FBoxCollider FirstBox = First.GenerateAABB();
			const FBoxCollider SecondBox = Second.GenerateAABB();
			if (SimpleCollision(FirstBox, SecondBox))
			{
				// Suspect contact
				FManifold NewManifold(First.Id, Second.Id);

				auto It = Contacts.find(NewManifold);
				if (It == Contacts.end())
				{
					Contacts.insert(NewManifold);
				}
				else
				{
					; // Technically std::set is readonly structure
					// Fortunately I don't need to do anything here
				}
				
			}
		}
	}
}

void SPhScene::NarrowPass()  
{  
	// Warm
	for (const FManifold& Manifold : Contacts)
	{  
		CRigidBodyComp& FirstBody = GetRigidBody(Manifold.FirstId);
		CRigidBodyComp& SecondBody = GetRigidBody(Manifold.SecondId);
		if (GenManifoldTable[FirstBody.Shape->GetShapeIndex()][SecondBody.Shape->GetShapeIndex()](Manifold))
		{
			FirstBody.Velocity -= Manifold.Impulse * FirstBody.PhyMat.InvMass * Manifold.Normal;
			SecondBody.Velocity += Manifold.Impulse * SecondBody.PhyMat.InvMass * Manifold.Normal;
		}
	}  

	// Solve
	for (const FManifold& Manifold : Contacts)
	{
		if (Manifold.bCollision)
		{
			ResolveCollision(Manifold);
		}
		else
		{
			// delete Manifold
		}
	}
}
