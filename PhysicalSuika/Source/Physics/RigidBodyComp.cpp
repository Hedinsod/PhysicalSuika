#include "pch.h"

#include "RigidBodyComp.h"
#include "PhyMaterialLibrary.h"
#include "Systems/Engine.h"


CRigidBodyComp::CRigidBodyComp(AActor* InOwner, uint32_t InMaterialId, FColliderShape* InShape, uint32_t InLayers)
	: CComponent(InOwner)
	, Shape(InShape)
	, MaterialId(InMaterialId)
	, Layers(InLayers)
{
	const FPhysicalMaterial& Material = SPhyMatirialLibrary::GetMaterial(MaterialId);
	if (Material.Density != 0)
	{
		auto MassPair = Shape->CalculateMass(Material.Density);
		InvMass = MassPair.first;
		InvInertia = MassPair.second;
	}
}

CRigidBodyComp::CRigidBodyComp(const CRigidBodyComp& Other)
	: CComponent(Other.Owner)
	, Shape(Other.Shape->Clone())
{
	BasicCopy(Other);
}

CRigidBodyComp::CRigidBodyComp(CRigidBodyComp&& Other) noexcept
	: CComponent(Other.Owner)
	, Shape(Other.Shape)
{
	BasicCopy(Other);
	Other.Shape = nullptr;
}

CRigidBodyComp::~CRigidBodyComp()
{
	OnDestruction.Broadcast(Contacts);

	if (Shape)
		delete Shape;
}

CRigidBodyComp& CRigidBodyComp::operator=(const CRigidBodyComp& Other)
{
	if (this == &Other)
		return *this;

	BasicCopy(Other);

	if (Shape) delete Shape;
	Shape = Other.Shape->Clone();

	return *this;
}
CRigidBodyComp& CRigidBodyComp::operator=(CRigidBodyComp&& Other) noexcept
{
	if (this == &Other)
		return *this;

	BasicCopy(Other);
	
	if (Shape) delete Shape;
	Shape = Other.Shape;
	Other.Shape = nullptr;

	return *this;
}

void CRigidBodyComp::BasicCopy(const CRigidBodyComp& Other)
{
	Owner = Other.Owner;
	Id = Other.Id;
	MaterialId = Other.MaterialId;
	Layers = Other.Layers;

	InvMass = Other.InvMass;
	InvInertia = Other.InvInertia;

	Position = Other.Position;
	Velocity = Other.Velocity;
	Forces = Other.Forces;

	Orientation = Other.Orientation;
	AngularVelocity = Other.AngularVelocity;
	Torque = Other.Torque;
}

void CRigidBodyComp::IntegrateVelocity(float TimeStep)
{
	static const glm::vec2 Gravity(0.f, -9.8);

	const FPhysicalMaterial& Mat = SPhyMatirialLibrary::GetMaterial(MaterialId);

	// Symplectic Euler 
	glm::vec2 Acceleration = Forces * InvMass + Gravity * Mat.GravityScale;
	Velocity += Acceleration * TimeStep;

	AngularVelocity += Torque * InvInertia * TimeStep;
}

void CRigidBodyComp::IntegratePosition(float TimeStep)
{
	Owner->GetTransform().Translate(Velocity * TimeStep);
	Owner->GetTransform().RotRad(AngularVelocity * TimeStep);
}
