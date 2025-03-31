#pragma once

#include <glm/glm.hpp>


enum class EColliderShape : uint16_t
{
	Box = 0,
	Circle = 1,
};

struct FBoxCollider;

struct FColliderShape
{
	FColliderShape() = default;
	FColliderShape(const FColliderShape&) = default;
	FColliderShape(FColliderShape&&) = default;
	FColliderShape(glm::vec2 InPivot)
		: Pivot(InPivot)
	{
	}

	virtual FColliderShape* Clone() = 0;

	virtual FBoxCollider GenerateAABB(const glm::vec2& Pos) const = 0;
	virtual int32_t GetShapeIndex() const = 0;

	glm::vec2 Pivot{ 0 };
};

struct FBoxCollider : public FColliderShape
{
	FBoxCollider() = default;
	FBoxCollider(const FBoxCollider&) = default;
	FBoxCollider(FBoxCollider&&) = default;
	FBoxCollider(glm::vec2 InPivot, float Left, float Top, float Right, float Bottom)
		: FColliderShape(InPivot)
		, Max(Right, Top)
		, Min(Left, Bottom)
	{
	}

	virtual FColliderShape* Clone() override;

	virtual FBoxCollider GenerateAABB(const glm::vec2& Pos) const override;
	virtual int32_t GetShapeIndex() const override { return 0; }

	glm::vec2 Max{ 0 };
	glm::vec2 Min{ 0 };
};

struct FCircleCollider : public FColliderShape
{
	FCircleCollider() = default;
	FCircleCollider(const FCircleCollider&) = default;
	FCircleCollider(FCircleCollider&&) = default;
	FCircleCollider(glm::vec2 InPivot, float InRadius)
		: FColliderShape(InPivot)
		, Radius(InRadius)
	{
	}

	virtual FColliderShape* Clone() override;

	virtual FBoxCollider GenerateAABB(const glm::vec2& Pos) const override;
	virtual int32_t GetShapeIndex() const override { return 1; }

	float Radius = 0.0f;
};
