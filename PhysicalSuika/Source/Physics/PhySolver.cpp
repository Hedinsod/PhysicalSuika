#include "pch.h"

#include "PhySolver.h"
#include "Systems/Engine.h"

#include <sstream>


SPhySolver::SPhySolver()
{
	GenManifoldTable[0][0] = std::bind(&SPhySolver::GenManifold_BoxBox, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	GenManifoldTable[0][1] = std::bind(&SPhySolver::GenManifold_BoxCircle, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	GenManifoldTable[1][0] = std::bind(&SPhySolver::GenManifold_CircleBox, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	GenManifoldTable[1][1] = std::bind(&SPhySolver::GenManifold_CircleCircle, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

void SPhySolver::WarmUp(float DeltaTime)
{
	for (FContact& Contact : ContactGraph)
	{
		CRigidBodyComp& FirstBody = Engine::GetPhyScene().GetRigidBody(Contact.FirstId);
		CRigidBodyComp& SecondBody = Engine::GetPhyScene().GetRigidBody(Contact.SecondId);

		// Put any pre-calculations here:
		// Positions
		FirstBody.Position = FirstBody.Owner->GetTransform().GetPos() + FirstBody.Shape->Pivot;
		SecondBody.Position = SecondBody.Owner->GetTransform().GetPos() + SecondBody.Shape->Pivot;

		if (!FirstBody.IsDisabled() && !SecondBody.IsDisabled() && 
			GenManifoldTable[FirstBody.Shape->GetShapeIndex()][SecondBody.Shape->GetShapeIndex()](FirstBody, SecondBody, Contact.Manifold))
		{
			// Apply impulses form previous calculation
			FirstBody.Velocity -= Contact.Manifold.NormalImpulse * FirstBody.InvMass * Contact.Manifold.Normal;
			SecondBody.Velocity += Contact.Manifold.NormalImpulse * SecondBody.InvMass * Contact.Manifold.Normal;

			FirstBody.AngularVelocity -= Contact.Manifold.TangentImpulse * FirstBody.InvInertia;
			SecondBody.AngularVelocity += Contact.Manifold.TangentImpulse * SecondBody.InvInertia;
		}
		else
		{
			ContactGraph.RemoveContact(Contact.Id);
		}
	}
}

void SPhySolver::SolveContacts()
{
	for (FContact& Contact : ContactGraph)
	{
		CRigidBodyComp& First = Engine::GetPhyScene().GetRigidBody(Contact.FirstId);
		CRigidBodyComp& Second = Engine::GetPhyScene().GetRigidBody(Contact.SecondId);

		if (First.IsDisabled() || Second.IsDisabled())
			continue;

		// Add Slot here
		First.OnCollision.Broadcast(Second.Owner);
		Second.OnCollision.Broadcast(First.Owner);

		ResolveCollision(First, Second, Contact.Manifold);
	}
}

void SPhySolver::ResolveCollision(CRigidBodyComp& First, CRigidBodyComp& Second, FManifold& Manifold)
{
	glm::vec2 FirstRadius = Manifold.ContactPoints[0] - First.Position;
	glm::vec2 SecondRadius = Manifold.ContactPoints[0] - Second.Position;

	float FirstNormalRadius = glm::dot(FirstRadius, Manifold.Normal);
	float SecondNormalRadius = glm::dot(SecondRadius, Manifold.Normal);

	// Calculate relative velocity in terms of the normal direction
	glm::vec2 RelativeVelocity = Second.Velocity + Math::cross2(Second.AngularVelocity, SecondRadius)
		- First.Velocity + Math::cross2(First.AngularVelocity, FirstRadius);
	float VelAlongNormal = glm::dot(RelativeVelocity, Manifold.Normal);

	// Calculate impulse scalar 
	float CommonMass = (First.InvMass + Second.InvMass);
	CommonMass += First.InvInertia * (glm::dot(FirstRadius, FirstRadius) - FirstNormalRadius * FirstNormalRadius)
		+ Second.InvInertia * (glm::dot(SecondRadius, SecondRadius) - SecondNormalRadius * SecondNormalRadius);
	CommonMass = 1.0f / CommonMass;

	float NormalImpulse = -1.0f * VelAlongNormal * CommonMass;

	// Accumulate and clump
	float OldImpulse = Manifold.NormalImpulse;
	Manifold.NormalImpulse = glm::max(OldImpulse + NormalImpulse, 0.f);
	NormalImpulse = Manifold.NormalImpulse - OldImpulse;

	glm::vec2 ContactImpulse = NormalImpulse * Manifold.Normal;

	// Apply
	First.Velocity -= First.InvMass * ContactImpulse;
	First.AngularVelocity -= First.InvInertia * Math::cross2(FirstRadius, ContactImpulse);

	Second.Velocity += Second.InvMass * ContactImpulse;
	Second.AngularVelocity += Second.InvInertia * Math::cross2(SecondRadius, ContactImpulse);



	// Friction
	// Recalculate velocity
	glm::vec2 Tangent = Math::cross2(Manifold.Normal, 1.0f);
	RelativeVelocity = Second.Velocity + Math::cross2(Second.AngularVelocity, SecondRadius)
		- First.Velocity + Math::cross2(First.AngularVelocity, FirstRadius);
	float TangentVelocity = glm::dot(RelativeVelocity, Tangent);

	float FirstTangentRadius = glm::dot(FirstRadius, Tangent);
	float SecondTangentRadius = glm::dot(SecondRadius, Tangent);

	float CommonMassT = (First.InvMass + Second.InvMass);
	CommonMassT += First.InvInertia * (glm::dot(FirstRadius, FirstRadius) - FirstTangentRadius * FirstTangentRadius)
		+ Second.InvInertia * (glm::dot(SecondRadius, SecondRadius) - SecondTangentRadius * SecondTangentRadius);

	float TangentImpulseM = CommonMassT * (-TangentVelocity);

	const FMaterial& Mat1 = Engine::GetMaterialLibrary().Get(First.MaterialTag);
	const FMaterial& Mat2 = Engine::GetMaterialLibrary().Get(Second.MaterialTag);
	float Friction = (Mat1.Friction + Mat2.Friction) / 2;

	// Compute friction impulse
	float maxPt = Friction * Manifold.NormalImpulse;

	// Clamp friction
	float OldTangentImpulse = Manifold.TangentImpulse;
	Manifold.TangentImpulse = glm::clamp(OldTangentImpulse + TangentImpulseM, -maxPt, maxPt);
	TangentImpulseM = Manifold.TangentImpulse - OldTangentImpulse;

	glm::vec2 TangentImpulse = TangentImpulseM * Tangent;

	First.Velocity -= First.InvMass * TangentImpulse;
	First.AngularVelocity -= First.InvInertia * Math::cross2(FirstRadius, TangentImpulse);

	Second.Velocity += Second.InvMass * TangentImpulse;
	Second.AngularVelocity += Second.InvInertia * Math::cross2(SecondRadius, TangentImpulse);



	// Position solving
	float positionImpulse = 0.2f * glm::max(0.0f, Manifold.Penetration);
	First.DeltaPosition -= positionImpulse * Manifold.Normal;
	Second.DeltaPosition += positionImpulse * Manifold.Normal;

	float Rotation = Math::cross2(FirstRadius, positionImpulse * Manifold.Normal);
	First.DeltaRotation -= Rotation;
	Rotation = Math::cross2(SecondRadius, positionImpulse * Manifold.Normal);
	Second.DeltaRotation += Rotation;
}

// ****************************************************************************
// ********** Manifold Generators *********************************************
// ****************************************************************************

bool SPhySolver::GenManifold_BoxBox(CRigidBodyComp& First, CRigidBodyComp& Second, FManifold& Manifold)
{
	const FBoxCollider& FirstBox = First.GetCollider<FBoxCollider>();
	const FBoxCollider& SecondBox = Second.GetCollider<FBoxCollider>();

	glm::vec2 Direction = Second.Position - First.Position;

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

bool SPhySolver::GenManifold_CircleBox(CRigidBodyComp& First, CRigidBodyComp& Second, FManifold& Manifold)
{
	return GenManifold_BoxCircle(Second, First, Manifold);
}

bool SPhySolver::GenManifold_BoxCircle(CRigidBodyComp& First, CRigidBodyComp& Second, FManifold& Manifold)
{
	// Alarm: actually calculating shape for the second time!
	FBoxCollider Box = First.GetCollider<FBoxCollider>();
	Box.Min += First.Position;
	Box.Max += First.Position;
	const FCircleCollider& Circle = Second.GetCollider<FCircleCollider>();

	// 
	glm::vec2 Direction = Second.Position - First.Position;
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
		return false;
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

	// Arbitrary one point of contact
	Manifold.ContactPoints[0] = Closest + First.Position;

	return true;
}

bool SPhySolver::GenManifold_CircleCircle(CRigidBodyComp& First, CRigidBodyComp& Second, FManifold& Manifold)
{
	// Calculate Shapes
	const FCircleCollider& FirstCircle = First.GetCollider<FCircleCollider>();
	const FCircleCollider& SecondCircle = Second.GetCollider<FCircleCollider>();

	//
	glm::vec2 Direction = Second.Position - First.Position;
	float Radius = FirstCircle.Radius + SecondCircle.Radius;
	float Radius2 = Radius * Radius;

	float Dist2 = glm::dot(Direction, Direction);
	if (Dist2 > Radius2)
	{
		return false;
	}

	// Circles have collided, now compute manifold 
	float Dist = glm::length(Direction);
	if (Dist != 0)
	{
		Manifold.Penetration = Radius - Dist;
		Manifold.Normal = glm::normalize(Direction);

		Manifold.ContactPoints[0] = Direction - SecondCircle.Radius * Manifold.Normal;
		Manifold.ContactPoints[0] += First.Position;

		return true;
	}
	else    // Circles are on same position 
	{
		// Choose random (but consistent) values
		Manifold.Penetration = FirstCircle.Radius;
		Manifold.Normal = glm::vec2(1, 0);

		Manifold.ContactPoints[0] = Direction - SecondCircle.Radius * Manifold.Normal;
		Manifold.ContactPoints[0] += First.Position;

		return true;
	}
}
