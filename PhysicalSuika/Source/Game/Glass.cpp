#include "pch.h"
#include "Glass.h"
#include "Systems/Engine.h"
#include "Renderer/GeometryComp.h"

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

	FRigidBodyDesc Desc;
	Desc.ColliderShape = EColliderShape::Box;
	Desc.Mass = 0.f;
	Desc.Restitution = 0.9f;
	Desc.StaticFriction = 0.3f;
	Desc.DynamicFriction = 0.1f;
	Desc.Layers = 1;

	Left = Engine::GetCollision().CreateRigidBody(this, Desc);
	CRigidBodyComp& LeftBody = Engine::GetCollision().GetRigidBody(Left);
	//LeftBody.SetupBoxCollider({ -5.5f, 8.0f }, -0.5f, 8.0f, 0.5f, -8.0f);
	LeftBody.SetupCollider<FBoxCollider>({ -5.5f, 8.0f }, -0.5f, 8.0f, 0.5f, -8.0f);

	Right = Engine::GetCollision().CreateRigidBody(this, Desc);
	CRigidBodyComp& RightBody = Engine::GetCollision().GetRigidBody(Right);
	RightBody.SetupCollider<FBoxCollider>({ 5.5f, 8.0f }, -0.5f, 8.0f, 0.5f, -8.0f);

	Bottom = Engine::GetCollision().CreateRigidBody(this, Desc);
	CRigidBodyComp& BottomBody = Engine::GetCollision().GetRigidBody(Bottom);
	BottomBody.SetupCollider<FBoxCollider>({ 0.f, -0.5f }, -5.0f, 0.5f, 5.0f, -0.5f);
}

AGlass::~AGlass()
{
	Engine::GetGraphics().RemoveGeometry(Geo);

	Engine::GetCollision().RemoveRigidBody(Left);
	Engine::GetCollision().RemoveRigidBody(Right);
	Engine::GetCollision().RemoveRigidBody(Bottom);
}

void AGlass::Tick()
{
}
