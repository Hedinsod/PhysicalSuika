#include "pch.h"
#include "Colliders.h"


FColliderShape* FBoxCollider::Clone() const
{
	return new FBoxCollider(*this);
}

std::pair<float, float> FBoxCollider::CalculateMass(float Density) const
{
	glm::vec2 Dim = Max - Min;

	float Mass = (Max.y - Min.y) * (Max.x - Min.x) * Density;
	float InvMass = 1.0f / Mass;
	float InvInertia = 12.0f / (glm::dot(Dim, Dim) * Mass); 

	return std::pair<float, float>();
}

FBoxCollider FBoxCollider::GenerateAABB(const glm::vec2& Pos) const
{
	FBoxCollider Result = *this;
	Result.Max += Pos + Pivot;
	Result.Min += Pos + Pivot;

	return Result;
}

FColliderShape* FCircleCollider::Clone() const
{
	return new FCircleCollider(*this);
}

FBoxCollider FCircleCollider::GenerateAABB(const glm::vec2& Pos) const
{
	FBoxCollider Result;

	const glm::vec2 ActualPos = Pos + Pivot;

	Result.Max = { ActualPos.x + Radius, ActualPos.y + Radius };
	Result.Min = { ActualPos.x - Radius, ActualPos.y - Radius };

	return Result;
}

std::pair<float, float> FCircleCollider::CalculateMass(float Density) const
{
	float Mass = Density * 3.14f * Radius;
	float InvMass = 1.0f / Mass;
	float InvInertia = 1.0f / (0.5f * Mass * Radius);

	return { InvMass, InvInertia };
}
