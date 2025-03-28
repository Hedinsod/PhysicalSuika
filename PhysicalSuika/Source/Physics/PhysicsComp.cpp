#include "pch.h"
#include "PhysicsComp.h"


FBoxCollider FBoxCollider::GenerateAABB(const glm::vec2& Pos)
{
	FBoxCollider Result = *this;
	Result.Max += Pos;
	Result.Min += Pos;

	return Result;
}

FBoxCollider FCircleCollider::GenerateAABB(const glm::vec2& Pos)
{
	FBoxCollider Result;
	Result.Max = { Pos.x + Radius, Pos.y + Radius };
	Result.Min = { Pos.x - Radius, Pos.y - Radius };

	return Result;
}




