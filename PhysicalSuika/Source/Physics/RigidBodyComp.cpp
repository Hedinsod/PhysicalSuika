#include "pch.h"
#include "RigidBodyComp.h"


CRigidBodyComp::CRigidBodyComp(AActor* InOwner, CBodyHandle NewId, const FRigidBodyDesc& Desc)
	: CComponent(InOwner)
	, Id(NewId)
	, Layers(1)
	, Velocity(0)
	, Forces(0)
{
	Shape = nullptr;  // TODO: Initialize. Somehow.

	/*
	switch (Desc.ColliderShape)
	{
	case EColliderShape::Box: SetupCollider<FColliderShape>(glm::vec2 InPivot, Args... args); break;
	case EColliderShape::Circle: SetupCollider<FColliderShape>(glm::vec2 InPivot, Args... args); break;
	default: GAssert(false, "Unknown collider shape.");
	}
	*/

	PhyMat.InvMass = Desc.Mass == 0 ? 0 : 1.f / Desc.Mass;
	PhyMat.Restitution = Desc.Restitution;
	PhyMat.StaticFriction = Desc.StaticFriction;
	PhyMat.DynamicFriction = Desc.DynamicFriction;
	PhyMat.GravityScale = 3.0f;
}

CRigidBodyComp::CRigidBodyComp(const CRigidBodyComp& Other)
	: CComponent(Other.Owner)
	, Id(Other.Id)
	, Shape(Other.Shape->Clone())
	, PhyMat(Other.PhyMat)
	, Layers(Other.Layers)
	, Velocity(Other.Velocity)
	, Forces(Other.Forces)
{
}

CRigidBodyComp::CRigidBodyComp(CRigidBodyComp&& Other) noexcept
	: CComponent(Other.Owner)
	, Id(Other.Id)
	, Shape(Other.Shape)
	, PhyMat(Other.PhyMat)
	, Layers(Other.Layers)
	, Velocity(Other.Velocity)
	, Forces(Other.Forces)
{
	Other.Shape = nullptr;
}

CRigidBodyComp::~CRigidBodyComp()
{
	if (Shape)
		delete Shape;
}

CRigidBodyComp& CRigidBodyComp::operator=(const CRigidBodyComp& Other)
{
	if (this == &Other)
		return *this;

	Owner = Other.Owner;
	Id = Other.Id;
	PhyMat = Other.PhyMat;
	Layers = Other.Layers;

	Velocity = Other.Velocity;
	Forces = Other.Forces;

	delete Shape;
	Shape = Other.Shape->Clone();

	return *this;
}
CRigidBodyComp& CRigidBodyComp::operator=(CRigidBodyComp&& Other) noexcept
{
	if (this == &Other)
		return *this;

	Owner = Other.Owner;
	Id = Other.Id;
	PhyMat = Other.PhyMat;
	Layers = Other.Layers;

	Velocity = Other.Velocity;
	Forces = Other.Forces;
	
	delete Shape;
	Shape = Other.Shape;
	Other.Shape = nullptr;

	return *this;
}