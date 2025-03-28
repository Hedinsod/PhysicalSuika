#pragma once

#include "Systems/Components.h"
#include "Game/Actor.h"
#include <glm/glm.hpp>


// ****************************************************************************
// ******** FBoundingShapes ***************************************************
// ****************************************************************************

struct FBoxCollider;

struct FColliderShape
{
	FColliderShape() = default;
	FColliderShape(glm::vec2 InPivot)
		: Pivot(InPivot)
	{ }

	virtual FBoxCollider GenerateAABB(const glm::vec2& Pos) = 0;

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

	virtual FBoxCollider GenerateAABB(const glm::vec2& Pos) override;

	glm::vec2 Max{ 0 };
	glm::vec2 Min{ 0 };
};

struct FCircleCollider : public FColliderShape
{
	FCircleCollider(glm::vec2 InPivot, float InRadius)
		: FColliderShape(InPivot)
		, Radius(InRadius)
	{
	}

	virtual FBoxCollider GenerateAABB(const glm::vec2& Pos) override;

	float Radius = 0.0f;
};


// ****************************************************************************
// ******** Physics Component *************************************************
// ****************************************************************************


class CPhysicsComp : public CComponent
{
	friend class SCollisionCheck; // Make Physics Engine friend of Body

public:
	CPhysicsComp(AActor* InOwner, float InMass, float InRestitution)
		: CComponent(InOwner)
		, InvMass(InMass == 0 ? 0 : 1.f / InMass)
		, Restitution(InRestitution)
		, Gravity(0, -0.98f * InMass)
		, Velocity(0)
		, Forces(0)
		, Layers(0)
	{
	}
	~CPhysicsComp()
	{
		if (Shape)
			delete Shape;
	}

	inline void SetupBoxCollider(glm::vec2 InPivot, float Left, float Top, float Right, float Bottom)
	{
		Shape = new FBoxCollider(InPivot, Left, Top, Right, Bottom);
	}
	inline void SetupCircleCollider(glm::vec2 InPivot, float InRadius)
	{
		Shape = new FCircleCollider(InPivot, InRadius);
	}
	void SetLayer(uint32_t InLayer)
	{
		Layers |= InLayer;
	}

	inline const FColliderShape* GetCollider() { return Shape; }
	inline CTransform& GetTransform() { return GetOwner().GetTransform(); }

	inline FBoxCollider GenerateAABB()
	{
		return Shape->GenerateAABB(Shape->Pivot + GetTransform().GetPos());
	}

	inline void ApplyForce(glm::vec2 NewForce)
	{
		Forces += NewForce;
	}

private:
	// Collider
	FColliderShape* Shape = nullptr;

	float InvMass;
	float Restitution;
	glm::vec2 Gravity;

	glm::vec2 Velocity;
	glm::vec2 Forces;

	uint32_t Layers;

};

/* Materials
Rock       Density : 0.6  Restitution : 0.1
Wood       Density : 0.3  Restitution : 0.2
Metal      Density : 1.2  Restitution : 0.05
BouncyBall Density : 0.3  Restitution : 0.8
SuperBall  Density : 0.3  Restitution : 0.95
Pillow     Density : 0.1  Restitution : 0.2
Static     Density : 0.0  Restitution : 0.4
*/
