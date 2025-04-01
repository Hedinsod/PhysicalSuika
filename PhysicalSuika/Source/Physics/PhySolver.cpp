#include "pch.h"

#include "PhySolver.h"
#include "Systems/Engine.h"
#include "PhyMaterialLibrary.h"


SPhySolver::SPhySolver()
{
	GenManifoldTable[0][0] = std::bind(&SPhySolver::GenManifold_BoxBox, this, std::placeholders::_1);
	GenManifoldTable[0][1] = std::bind(&SPhySolver::GenManifold_BoxCircle, this, std::placeholders::_1);
	GenManifoldTable[1][0] = std::bind(&SPhySolver::GenManifold_CircleBox, this, std::placeholders::_1);
	GenManifoldTable[1][1] = std::bind(&SPhySolver::GenManifold_CircleCircle, this, std::placeholders::_1);
}

void SPhySolver::AddPair(CRigidBodyComp& InFirst, CRigidBodyComp& InSecond)
{
	FManifold NewManifold(InFirst.Id, InSecond.Id);

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

void SPhySolver::WarmUp()
{
	for (const FManifold& Manifold : Contacts)
	{
		CRigidBodyComp& FirstBody = Engine::GetPhyScene().GetRigidBody(Manifold.FirstId);
		CRigidBodyComp& SecondBody = Engine::GetPhyScene().GetRigidBody(Manifold.SecondId);
		if (GenManifoldTable[FirstBody.Shape->GetShapeIndex()][SecondBody.Shape->GetShapeIndex()](Manifold))
		{
			FirstBody.Velocity -= Manifold.NormalImpulse * FirstBody.InvMass * Manifold.Normal;
			SecondBody.Velocity += Manifold.NormalImpulse * SecondBody.InvMass * Manifold.Normal;

			FirstBody.AngularVelocity -= Manifold.TangentImpulse * FirstBody.InvInertia;
			SecondBody.AngularVelocity += Manifold.TangentImpulse * SecondBody.InvInertia;
		}
	}
}

void SPhySolver::SolveContacts()
{
	for (auto it = Contacts.begin(); it != Contacts.end();)
	{
		if (it->bCollision)
		{
			ResolveCollision(*it++);
		}
		else
		{
			it = Contacts.erase(it);
		}
	}
}

void SPhySolver::ResolveCollision(const FManifold& Manifold)
{
	// TODO: Calculate per contact

	CRigidBodyComp& First = Engine::GetPhyScene().GetRigidBody(Manifold.FirstId);
	CRigidBodyComp& Second = Engine::GetPhyScene().GetRigidBody(Manifold.SecondId);

	glm::vec2 FirstPos = First.GetTransform().GetPos() + First.Shape->Pivot;
	glm::vec2 SecondPos = Second.GetTransform().GetPos() + Second.Shape->Pivot;

	glm::vec2 FirstRadius = Manifold.ContactPoints[0] - FirstPos;
	glm::vec2 SecondRadius = Manifold.ContactPoints[0] - SecondPos;

	float FirstNormalRadius = glm::dot(FirstRadius, Manifold.Normal);
	float SecondNormalRadius = glm::dot(SecondRadius, Manifold.Normal);

	// Erin Catto calls this crude, but whatever. TODO: get timestep properly!
	float Bias = -0.2f * 1.f / (0.0041f) * glm::min(0.0f, 0.01f - Manifold.Penetration);

	// Calculate relative velocity in terms of the normal direction
	glm::vec2 RelativeVelocity = Second.Velocity + Utility::cross2(Second.AngularVelocity, SecondRadius)
		- First.Velocity + Utility::cross2(First.AngularVelocity, FirstRadius);
	float VelAlongNormal = glm::dot(RelativeVelocity, Manifold.Normal);

	// Calculate impulse scalar 
	float CommonMass = (First.InvMass + Second.InvMass);
	CommonMass += First.InvInertia * (glm::dot(FirstRadius, FirstRadius) - FirstNormalRadius * FirstNormalRadius)
		+ Second.InvInertia * (glm::dot(SecondRadius, SecondRadius) - SecondNormalRadius * SecondNormalRadius);
	CommonMass = 1.0f / CommonMass;

	float NormalImpulse = (Bias - VelAlongNormal) * CommonMass;

	// Accumulate and clump
	float OldImpulse = Manifold.NormalImpulse;
	Manifold.NormalImpulse = glm::max(OldImpulse + NormalImpulse, 0.f);
	NormalImpulse = Manifold.NormalImpulse - OldImpulse;

	glm::vec2 ContactImpulse = NormalImpulse * Manifold.Normal;

	if (First.InvMass != 0 && Second.InvMass != 0)
	{
		std::cerr << "AGA!" << std::endl;
	}

	// Apply
	First.Velocity -= First.InvMass * ContactImpulse;
	First.AngularVelocity -= First.InvInertia * Utility::cross2(FirstRadius, ContactImpulse);

	Second.Velocity += Second.InvMass * ContactImpulse;
	Second.AngularVelocity += Second.InvInertia * Utility::cross2(SecondRadius, ContactImpulse);



	// Friction
	// Recalculate velocity
	glm::vec2 Tangent = Utility::cross2(Manifold.Normal, 1.0f);
	RelativeVelocity = Second.Velocity + Utility::cross2(Second.AngularVelocity, SecondRadius)
		- First.Velocity + Utility::cross2(First.AngularVelocity, FirstRadius);
	float TangentVelocity = glm::dot(RelativeVelocity, Tangent);

	float FirstTangentRadius = glm::dot(FirstRadius, Tangent);
	float SecondTangentRadius = glm::dot(SecondRadius, Tangent);

	float CommonMassT = (First.InvMass + Second.InvMass);
	CommonMassT += First.InvInertia * (glm::dot(FirstRadius, FirstRadius) - FirstTangentRadius * FirstTangentRadius)
		+ Second.InvInertia * (glm::dot(SecondRadius, SecondRadius) - SecondTangentRadius * SecondTangentRadius);

	float TangentImpulseM = CommonMassT * (-TangentVelocity);

	const FPhysicalMaterial& Mat1 = SPhyMatirialLibrary::GetMaterial(First.MaterialId);
	const FPhysicalMaterial& Mat2 = SPhyMatirialLibrary::GetMaterial(Second.MaterialId);
	float Friction = (Mat1.Friction + Mat2.Friction) / 2;

	// Compute friction impulse
	float maxPt = Friction * Manifold.NormalImpulse;

	// Clamp friction
	float OldTangentImpulse = Manifold.TangentImpulse;
	Manifold.TangentImpulse = glm::clamp(OldTangentImpulse + TangentImpulseM, -maxPt, maxPt);
	TangentImpulseM = Manifold.TangentImpulse - OldTangentImpulse;

	glm::vec2 TangentImpulse = TangentImpulseM * Tangent;

	First.Velocity -= First.InvMass * TangentImpulse;
	First.AngularVelocity -= First.InvInertia * Utility::cross2(FirstRadius, TangentImpulse);

	Second.Velocity += Second.InvMass * TangentImpulse;
	Second.AngularVelocity += Second.InvInertia * Utility::cross2(SecondRadius, TangentImpulse);

}

// ****************************************************************************
// ********** Manifold Generators *********************************************
// ****************************************************************************

struct FPolygonShape
{
	std::vector<glm::vec2> Vertices;
	std::vector<glm::vec2> Normals;

	glm::vec2 Pivot;
};

static glm::vec2 GetSupport(const FPolygonShape& Polygon, const glm::vec2& Direction)
{
	float MaxProjection = std::numeric_limits<float>::min();
	glm::vec2 MaxVertex{0};
	for (uint32_t i = 0; i < Polygon.Vertices.size(); ++i)
	{
		glm::vec2 Vert = Polygon.Vertices[i];
		float Projection = glm::dot(Vert, Direction);
		if (Projection > MaxProjection)
		{
			MaxVertex = Vert;
			MaxProjection = Projection;
		}
	}
	return MaxVertex;
}

static std::pair<uint32_t, float> FindAxisLeastPenetration(const FPolygonShape& First, const FPolygonShape& Second)
{
	float LeastPenetration = std::numeric_limits<float>::min();
	uint32_t FaceIndex;
	for (uint32_t i = 0; i < First.Normals.size(); ++i)
	{
		glm::vec2 Normal = First.Normals[i];
		glm::vec2 Support = GetSupport(Second, -Normal);
		glm::vec2 Vertex = First.Vertices[i]; // first point on the face i
		
		float Penetration = glm::dot(Normal, Support - Vertex);

		// Store greatest distance 
		if (Penetration > LeastPenetration)
		{
			LeastPenetration = Penetration;
			FaceIndex = i;
		}
	}
	return { FaceIndex, LeastPenetration };
}

void FindClosestPoint(const FPolygonShape& First, int32_t FaceIndex, const FPolygonShape& Second)
{

}


bool SPhySolver::GenManifold_BoxBox(const FManifold& Manifold)
{
	CRigidBodyComp& First = Engine::GetPhyScene().GetRigidBody(Manifold.FirstId);
	CRigidBodyComp& Second = Engine::GetPhyScene().GetRigidBody(Manifold.SecondId);

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

bool SPhySolver::GenManifold_CircleBox(const FManifold& Manifold)
{
	// Interesting trick
	CBodyHandle Temp = Manifold.FirstId;
	Manifold.FirstId = Manifold.SecondId;
	Manifold.SecondId = Temp;

	return GenManifold_BoxCircle(Manifold);
}

bool SPhySolver::GenManifold_BoxCircle(const FManifold& Manifold)
{
	CRigidBodyComp& First = Engine::GetPhyScene().GetRigidBody(Manifold.FirstId);
	CRigidBodyComp& Second = Engine::GetPhyScene().GetRigidBody(Manifold.SecondId);

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

	// Arbitrary one point of contact
	Manifold.ContactPoints[0] = Closest + FirstPos;

	Manifold.bCollision = true;
	return Manifold.bCollision;
}

bool SPhySolver::GenManifold_CircleCircle(const FManifold& Manifold)
{
	// Calculate Shapes
	CRigidBodyComp& First = Engine::GetPhyScene().GetRigidBody(Manifold.FirstId);
	CRigidBodyComp& Second = Engine::GetPhyScene().GetRigidBody(Manifold.SecondId);

	const FCircleCollider& FirstCircle = First.GetCollider<FCircleCollider>();
	const FCircleCollider& SecondCircle = Second.GetCollider<FCircleCollider>();

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

		Manifold.ContactPoints[0] = Direction - SecondCircle.Radius * Manifold.Normal;
		Manifold.ContactPoints[0] += PosA;

		Manifold.bCollision = true;
		return Manifold.bCollision;
	}
	else    // Circles are on same position 
	{
		// Choose random (but consistent) values
		Manifold.Penetration = FirstCircle.Radius;
		Manifold.Normal = glm::vec2(1, 0);

		Manifold.ContactPoints[0] = Direction - SecondCircle.Radius * Manifold.Normal;
		Manifold.ContactPoints[0] += PosA;

		Manifold.bCollision = true;
		return Manifold.bCollision;
	}
}
