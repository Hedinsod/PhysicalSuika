#include "pch.h"
#include "Colliders.h"


FColliderShape* FBoxCollider::Clone()
{
	return new FBoxCollider(*this);
}

FBoxCollider FBoxCollider::GenerateAABB(const glm::vec2& Pos) const
{
	FBoxCollider Result = *this;
	Result.Max += Pos + Pivot;
	Result.Min += Pos + Pivot;

	return Result;
}

FColliderShape* FCircleCollider::Clone()
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
