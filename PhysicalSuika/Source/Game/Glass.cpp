#include "pch.h"
#include "Glass.h"
#include "Systems/Engine.h"
#include "Physics/PhyMaterialLibrary.h"
#include "Renderer/GeometryComp.h"


bool AGlass::bMatCreated = false;
uint32_t AGlass::MatId = 0;

AGlass::AGlass(glm::vec2 InPos)
	: AActor(InPos, "Glass\n")
{
	Geo = Engine::GetGraphics().CreateGeometry(this);

	FColorRGB GlassColor({ 125, 125, 125 });

	Geo->SetVertices({
		 -5.0f, 16.0f,
		 -5.0f,  0.0f,
		  5.0f,  0.0f,
		  5.0f, 16.0f
		});
	Geo->SetIndecies({
		0, 1, 2, 3
		});
	Geo->BuildGeometry();

	if (!bMatCreated)
	{
		FPhysicalMaterial Glass{ .Density = 0.0f, .Friction = 0.4f, .GravityScale = 2.0f };
		MatId = SPhyMatirialLibrary::AddMaterial(Glass);
		bMatCreated = true;
	}

	FColliderShape* LeftBox = FColliderShape::Create<FBoxCollider>({ -5.5f, 8.0f }, -0.5f, 8.0f, 0.5f, -8.0f);
	FColliderShape* RightBox = FColliderShape::Create<FBoxCollider>({ 5.5f, 8.0f }, -0.5f, 8.0f, 0.5f, -8.0f);
	FColliderShape* BottomBox = FColliderShape::Create<FBoxCollider>({ 0.f, -0.5f }, -5.0f, 0.5f, 5.0f, -0.5f);

	Left = Engine::GetPhyScene().CreateRigidBody(this, MatId, LeftBox);
	Right = Engine::GetPhyScene().CreateRigidBody(this, MatId, RightBox);
	Bottom = Engine::GetPhyScene().CreateRigidBody(this, MatId, BottomBox);
}

AGlass::~AGlass()
{
	Engine::GetGraphics().RemoveGeometry(Geo);

	Engine::GetPhyScene().RemoveRigidBody(Left);
	Engine::GetPhyScene().RemoveRigidBody(Right);
	Engine::GetPhyScene().RemoveRigidBody(Bottom);
}

void AGlass::Tick()
{
}
