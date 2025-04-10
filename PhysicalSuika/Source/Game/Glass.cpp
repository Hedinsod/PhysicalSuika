#include "pch.h"
#include "Glass.h"
#include "Systems/Engine.h"
#include "Renderer/GeometryComp.h"


AGlass::AGlass(glm::vec2 InPos)
	: AActor(InPos)
{
	Geo = Engine::GetGraphics().CreateGeometry(this);

	FColorRGB GlassColor({ 125, 125, 125 });

	Geo->SetVertices({
		 -5.0f, 16.0f,
		 -5.0f,  0.0f,
		  5.0f,  0.0f,
		  5.0f, 16.0f
		});
	Geo->SetIndices({
		0, 1, 2, 3
		});
	Geo->BuildGeometry();
	Geo->SetColor(FColorRGB(0,0,0));

	FColliderShape* LeftBox = FColliderShape::Create<FBoxCollider>({ -5.5f, 8.0f }, -0.5f, 8.0f, 0.5f, -8.0f);
	FColliderShape* RightBox = FColliderShape::Create<FBoxCollider>({ 5.5f, 8.0f }, -0.5f, 8.0f, 0.5f, -8.0f);
	FColliderShape* BottomBox = FColliderShape::Create<FBoxCollider>({ 0.f, -0.5f }, -5.0f, 0.5f, 5.0f, -0.5f);

	Left = Engine::GetPhyScene().CreateRigidBody(this, "Glass", LeftBox);
	Right = Engine::GetPhyScene().CreateRigidBody(this, "Glass", RightBox);
	Bottom = Engine::GetPhyScene().CreateRigidBody(this, "Glass", BottomBox);
}

AGlass::~AGlass()
{
	Engine::GetGraphics().RemoveGeometry(Geo);

	Engine::GetPhyScene().RemoveRigidBody(Left);
	Engine::GetPhyScene().RemoveRigidBody(Right);
	Engine::GetPhyScene().RemoveRigidBody(Bottom);
}
