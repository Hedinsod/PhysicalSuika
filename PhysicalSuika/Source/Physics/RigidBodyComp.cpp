#include "pch.h"

#include "RigidBodyComp.h"
#include "Systems/Engine.h"


CRigidBodyComp::CRigidBodyComp(AActor* InOwner, const std::string& InMaterialTag, FColliderShape* InShape, uint32_t InLayers, bool bInTrigger/*= false*/)
	: CComponent(InOwner)
	, Shape(InShape)
	, MaterialTag(InMaterialTag)
	, Layers(InLayers)
	, bTrigger(bInTrigger)
{
	const FMaterial& Material = Engine::GetMaterialLibrary().Get(InMaterialTag);
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
	, Contacts(Other.Contacts)
{
	BasicCopy(Other);
}

CRigidBodyComp::CRigidBodyComp(CRigidBodyComp&& Other) noexcept
	: CComponent(Other.Owner)
	, Shape(Other.Shape)
	, Contacts(std::move(Other.Contacts))
{
	BasicCopy(Other);
	Other.Shape = nullptr;

	Other.OnDestruction.InvocationList.clear();
	Other.OnCollision.InvocationList.clear();
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
	Other.OnDestruction.InvocationList.clear();
	Other.OnCollision.InvocationList.clear();

	return *this;
}

void CRigidBodyComp::BasicCopy(const CRigidBodyComp& Other)
{
	Owner = Other.Owner;
	Id = Other.Id;
	MaterialTag = Other.MaterialTag;
	Layers = Other.Layers;

	InvMass = Other.InvMass;
	InvInertia = Other.InvInertia;

	Position = Other.Position;
	Velocity = Other.Velocity;
	Forces = Other.Forces;

	DeltaRotation = Other.DeltaRotation;
	AngularVelocity = Other.AngularVelocity;
	Torque = Other.Torque;

	bDisabled = Other.bDisabled;
	bTrigger = Other.bTrigger;
	OnDestruction = Other.OnDestruction;
	OnCollision = Other.OnCollision;
}

void CRigidBodyComp::IntegrateVelocity(float TimeStep)
{
	static const glm::vec2 Gravity(0.f, -98.0f);

	const FMaterial& Material = Engine::GetMaterialLibrary().Get(MaterialTag);

	// Symplectic Euler 
	glm::vec2 Acceleration = Forces * InvMass + Gravity * Material.GravityScale;
	Velocity += Acceleration * TimeStep;

	GAssert(!std::isnan(Velocity.x));
	GAssert(!std::isnan(Velocity.y));

	AngularVelocity += Torque * InvInertia * TimeStep;
}

void CRigidBodyComp::IntegratePosition(float TimeStep)
{
	// A completly arbitrary value preventing disaster
	static glm::vec2 MaxSpeed(45.0f, 45.0);
	Velocity = glm::clamp(Velocity, -MaxSpeed, MaxSpeed);

	DeltaPosition += Velocity * TimeStep;
	Owner->GetTransform().Translate(DeltaPosition);
	Owner->GetTransform().Rotate(DeltaRotation + AngularVelocity * TimeStep);

	DeltaPosition = glm::vec2(0.0f);
	DeltaRotation = 0.0f;
}
