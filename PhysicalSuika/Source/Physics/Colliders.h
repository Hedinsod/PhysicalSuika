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
	FColliderShape(glm::vec2 InPivot) : Pivot(InPivot) {}
	FColliderShape(const FColliderShape&) = default;
	FColliderShape(FColliderShape&&) = default;
	
	template <class T, typename... Args>
	static inline FColliderShape* Create(glm::vec2 InPivot, Args... args)
	{
		return new T(InPivot, args...);
	}

	virtual FColliderShape* Clone() const = 0;
	virtual FBoxCollider GenerateAABB(const glm::vec2& Pos) const = 0;
	virtual std::pair<float, float> CalculateMass(float Density) const = 0;
	virtual uint16_t GetShapeIndex() const = 0;

	glm::vec2 Pivot{ 0 };
};

struct FBoxCollider : public FColliderShape
{
	FBoxCollider() = default;
	FBoxCollider(glm::vec2 InPivot, float Left, float Top, float Right, float Bottom)
		: FColliderShape(InPivot)
		, Max(Right, Top)
		, Min(Left, Bottom)
	{
	}
	FBoxCollider(const FBoxCollider&) = default;
	FBoxCollider(FBoxCollider&&) = default;


	virtual FColliderShape* Clone() const override;
	virtual std::pair<float, float> CalculateMass(float Density) const override;
	virtual uint16_t GetShapeIndex() const { return (uint16_t)EColliderShape::Box; }

	virtual FBoxCollider GenerateAABB(const glm::vec2& Pos) const override;

	glm::vec2 Max{ 0 };
	glm::vec2 Min{ 0 };
};

struct FCircleCollider : public FColliderShape
{
	FCircleCollider() = default;
	FCircleCollider(glm::vec2 InPivot, float InRadius)
		: FColliderShape(InPivot)
		, Radius(InRadius)
	{
	}
	FCircleCollider(const FCircleCollider&) = default;
	FCircleCollider(FCircleCollider&&) = default;

	virtual FColliderShape* Clone() const override;
	virtual std::pair<float, float> CalculateMass(float Density) const override;
	virtual uint16_t GetShapeIndex() const { return (uint16_t)EColliderShape::Circle; }
	virtual FBoxCollider GenerateAABB(const glm::vec2& Pos) const override;

	float Radius = 0.0f;
};
