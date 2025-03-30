#include "pch.h"

#include "Core/Application.h"
#include "Collision.h"
#include "Game/Game.h"
#include "Game/Actor.h"
#include "Renderer/Transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>


const float GGravity = 9.8f;
const float GSlop = 0.015f;


SCollisionCheck::SCollisionCheck()
{
	DoubleVTable[0][0] = std::bind(&SCollisionCheck::GenManifold_BoxBox, this, std::placeholders::_1);
	DoubleVTable[0][1] = std::bind(&SCollisionCheck::GenManifold_BoxCircle, this, std::placeholders::_1);
	DoubleVTable[1][0] = std::bind(&SCollisionCheck::GenManifold_CircleBox, this, std::placeholders::_1);
	DoubleVTable[1][1] = std::bind(&SCollisionCheck::GenManifold_CircleCircle, this, std::placeholders::_1);
}

CPhysicsComp* SCollisionCheck::AddPhysics(AActor* Owner, float InMass, float InRestitution)
{
	auto [It, bSuccess] = Bodies.emplace(new CPhysicsComp(Owner, InMass, InRestitution));
	GAssert(bSuccess);
	return *It;
}

void SCollisionCheck::RemovePhysics(CPhysicsComp* Comp)
{
	Bodies.erase(Comp);
	delete Comp;
}

bool SCollisionCheck::SimpleCollision(const FBoxCollider& FirstAABB, const FBoxCollider& SecondAABB)
{
	if (FirstAABB.Max.x < SecondAABB.Min.x or FirstAABB.Min.x > SecondAABB.Max.x) return false;
	if (FirstAABB.Max.y < SecondAABB.Min.y or FirstAABB.Min.y > SecondAABB.Max.y) return false;

	return true;
}

bool SCollisionCheck::GenManifold_BoxBox(FManifold& Manifold)
{
	CPhysicsComp* First = Manifold.BodyA;
	CPhysicsComp* Second = Manifold.BodyB;

	// TODO: GetShape() or something
	const FBoxCollider BoxA = First->GenerateAABB();
	const FBoxCollider BoxB = Second->GenerateAABB();

	glm::vec2 PosA = First->GetTransform().GetPos() + First->Shape->Pivot;
	glm::vec2 PosB = Second->GetTransform().GetPos() + Second->Shape->Pivot;
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

bool SCollisionCheck::GenManifold_CircleBox(FManifold& Manifold)
{
	CPhysicsComp* Temp = Manifold.BodyA;
	Manifold.BodyA = Manifold.BodyB;
	Manifold.BodyB = Temp;

	return GenManifold_BoxCircle(Manifold);
}

bool SCollisionCheck::GenManifold_BoxCircle(FManifold& Manifold)
{
	// Calculate Shapes
	glm::vec2 PosA = Manifold.BodyA->GetTransform().GetPos() + Manifold.BodyA->Shape->Pivot;
	glm::vec2 PosB = Manifold.BodyB->GetTransform().GetPos() + Manifold.BodyB->Shape->Pivot;

	FBoxCollider BoxA = *static_cast<FBoxCollider*>(Manifold.BodyA->Shape);
	BoxA.Min += PosA;
	BoxA.Max += PosA;
	const FCircleCollider& CircleB = *static_cast<FCircleCollider*>(Manifold.BodyB->Shape);

	// 
	glm::vec2 Direction = PosB - PosA;
	glm::vec2 Closest = Direction;

	// Calculate half extents along each axis 
	float ExtentX = (BoxA.Max.x - BoxA.Min.x) / 2;
	float ExtentY = (BoxA.Max.y - BoxA.Min.y) / 2;

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
	float d = glm::length2(Normal); // TODO: dot ?
	GAssert(d == glm::dot(Normal, Normal));
	float r = CircleB.Radius;

	// Early out of the radius is shorter than distance to closest point and 
	// Circle not inside the AABB 
	if (d > r * r && !bInside)
		return false;

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

	return true;
}

bool SCollisionCheck::GenManifold_CircleCircle(FManifold& Manifold)
{
	// Calculate Shapes
	CPhysicsComp* First = Manifold.BodyA;
	CPhysicsComp* Second = Manifold.BodyB;

	FCircleCollider& CircleA = *static_cast<FCircleCollider*>(First->Shape);
	FCircleCollider& CircleB = *static_cast<FCircleCollider*>(Second->Shape);

	glm::vec2 PosA = First->GetTransform().GetPos() + CircleA.Pivot;
	glm::vec2 PosB = Second->GetTransform().GetPos() + CircleB.Pivot;

	//

	glm::vec2 Direction = PosB - PosA;
	float Radius = CircleA.Radius + CircleB.Radius;
	float Radius2 = Radius * Radius;

	float Dist2 = glm::length2(Direction);
	if (Dist2 > Radius2)
		return false;

	// Circles have collided, now compute manifold 
	float Dist = glm::length(Direction);
	if (Dist != 0)
	{
		Manifold.Penetration = Radius - Dist;
		Manifold.Normal = glm::normalize(Direction);
		return true;
	}
	else    // Circles are on same position 
	{
		// Choose random (but consistent) values
		Manifold.Penetration = CircleA.Radius;
		Manifold.Normal = glm::vec2(1, 0);
		return true;
	}
}

void SCollisionCheck::ResolveCollision(FManifold& Manifold)
{
	CPhysicsComp& BodyA = *Manifold.BodyA;
	CPhysicsComp& BodyB = *Manifold.BodyB;

//	float bias = -0.2f * 1/dt * glm::min(0.0f, 0.01f - Manifold.Penetration);

	// Warm
	BodyA.Velocity -= Manifold.Impulse * BodyA.InvMass * Manifold.Normal;
	BodyB.Velocity += Manifold.Impulse * BodyB.InvMass * Manifold.Normal;

	glm::vec2 RelativeVelocity = BodyB.Velocity - BodyA.Velocity;

	// Calculate relative velocity in terms of the normal direction
	float VelAlongNormal = glm::dot(RelativeVelocity, Manifold.Normal);

	// Calculate restitution 
	float e = glm::min(BodyA.Restitution, BodyB.Restitution);

	// Calculate impulse scalar 
	float Impulse = -(1 + e) * VelAlongNormal / (BodyA.InvMass + BodyB.InvMass);

	float OldImpulse = Manifold.Impulse;
	Manifold.Impulse = glm::max(Manifold.Impulse + Impulse, 0.f);
	Impulse = Manifold.Impulse - OldImpulse;

	BodyA.Velocity -= Impulse * BodyA.InvMass * Manifold.Normal;
	BodyB.Velocity += Impulse * BodyB.InvMass * Manifold.Normal;
	
	
	
	// Friction
	RelativeVelocity = BodyB.Velocity - BodyA.Velocity;
	glm::vec2 Tangent = RelativeVelocity - glm::dot(RelativeVelocity, Manifold.Normal) * Manifold.Normal;
	if (glm::length2(Tangent) > 0)
	{
		glm::vec2 TangentNormal = glm::normalize(Tangent);
		float Friction = -glm::dot(TangentNormal, RelativeVelocity);
		Friction /= (BodyB.InvMass + BodyA.InvMass);

		constexpr float mu = 0.3f;  // static friction - from material
		if (abs(Friction) > Impulse * mu)
		{
			float dynamicFriction = 0.22f;
			Friction = -Impulse * dynamicFriction;
		}

		BodyA.ApplyForce(-Friction * TangentNormal);
		BodyB.ApplyForce(Friction * TangentNormal);
	}
}

void SCollisionCheck::PositionalCorrection(FManifold& Manifold)
{
	glm::vec2 Ft = 12.0f * Manifold.Penetration * Manifold.Normal;

	Manifold.BodyA->Velocity -= Manifold.BodyA->InvMass * Ft * Manifold.BodyA->InvMass;
	Manifold.BodyB->Velocity += Manifold.BodyB->InvMass * Ft * Manifold.BodyB->InvMass;

	/*
	const float Multip = 0.5f;    // usually 20% to 80%

	glm::vec2 Correction = glm::max(Manifold.Penetration - GSlop, 0.0f)
		/ (Manifold.BodyA->InvMass + Manifold.BodyB->InvMass) * Multip * Manifold.Normal;

	Manifold.BodyA->GetTransform().Translate(-Correction * Manifold.BodyA->InvMass); 
	Manifold.BodyB->GetTransform().Translate(Correction * Manifold.BodyB->InvMass);
	*/
}

void SCollisionCheck::Tick(float DeltaTimeMs)
{
	if (Bodies.empty())
		return;

	int32_t Steps = 4;
	dt = DeltaTimeMs /= 1000 * Steps;

	for (int32_t i = 0; i < Steps; i++)
	{
		// Integrate Forces & Gravity
		for (CPhysicsComp* Body : Bodies)
		{
			if (Body->InvMass == 0)
				continue;

			// Symplectic Euler 
			glm::vec2 Acc = Body->Forces * Body->InvMass + glm::vec2(0.f, -GGravity) * Body->GravityScale;
			Body->Velocity += Acc * DeltaTimeMs;
		}

		BroadPass();
		NarrowPass();

		// Integrate Position
		for (CPhysicsComp* Body : Bodies)
		{
			if (Body->InvMass == 0)
				continue;

			Body->GetOwner().GetTransform().Translate(Body->Velocity * DeltaTimeMs);
		}
	}
}

void SCollisionCheck::BroadPass()
{
	//Manifolds.clear();

	for (auto First = Bodies.begin(); First != Bodies.end(); First++)
	{
		// Reset Forces
		(*First)->Forces = { 0.f, 0.f };

		for (auto Second = First; Second != Bodies.end(); Second++)
		{
			if (First == Second)
				continue;

			if (!((*First)->Layers & (*Second)->Layers))
				continue;

			if ((*First)->InvMass == 0 && (*Second)->InvMass == 0)
				continue;

			FBoxCollider FirstBox = (*First)->GenerateAABB();
			FBoxCollider SecondBox = (*Second)->GenerateAABB();
			if (SimpleCollision(FirstBox, SecondBox))
			{
				/*
				FManifold NewManifold;
				NewManifold.BodyA = *First;
				NewManifold.BodyB = *Second;
				*/

				FManifoldKey Key(*First, *Second);
				auto It = Manifolds.find(Key);
				if (It == Manifolds.end())
				{
					FManifold NewManifold;
					NewManifold.BodyA = *First;
					NewManifold.BodyB = *Second;

					Manifolds.insert({ Key, NewManifold });
				}
				else
				{
					It->second.Penetration = 0.0f;
					It->second.Normal = { 0.0f, 0.0f };
				}
				
			}
		}
	}
}

void SCollisionCheck::NarrowPass()  
{  
	//for (FManifold& Manifold : Manifolds)  
	for (auto& [Key, Manifold] : Manifolds)
	{  
		if (DoubleVTable[Manifold.BodyA->Shape->GetShapeIndex()][Manifold.BodyB->Shape->GetShapeIndex()](Manifold))
		{  
			ResolveCollision(Manifold);
			PositionalCorrection(Manifold);
		}
	}  
}

