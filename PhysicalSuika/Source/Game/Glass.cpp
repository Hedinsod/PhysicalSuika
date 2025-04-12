#include "pch.h"
#include "Glass.h"
#include "Systems/Engine.h"
#include "Renderer/GeometryComp.h"


AGlass::AGlass(glm::vec2 InPos)
	: AActor(InPos)
{
	std::vector<glm::vec2> Points = { 
			glm::vec2(-5.0f, 16.0f),
			glm::vec2(-5.0f, 0.0f),
			glm::vec2(5.0f, 0.0f),
			glm::vec2(5.0f, 16.0f)
		};

	GeoHandle = Engine::GetGraphics().CreateGeometry(this);
	(*GeoHandle).Import(Points);

	FColliderShape* LeftBox = FColliderShape::Create<FBoxCollider>({ -5.5f, 8.0f }, -0.5f, 8.0f, 0.5f, -8.0f);
	FColliderShape* RightBox = FColliderShape::Create<FBoxCollider>({ 5.5f, 8.0f }, -0.5f, 8.0f, 0.5f, -8.0f);
	FColliderShape* BottomBox = FColliderShape::Create<FBoxCollider>({ 0.f, -0.5f }, -5.0f, 0.5f, 5.0f, -0.5f);

	Left = Engine::GetPhyScene().CreateRigidBody(this, "Glass", LeftBox);
	Right = Engine::GetPhyScene().CreateRigidBody(this, "Glass", RightBox);
	Bottom = Engine::GetPhyScene().CreateRigidBody(this, "Glass", BottomBox);
}

AGlass::~AGlass()
{
	Engine::GetGraphics().RemoveGeometry(GeoHandle);

	Engine::GetPhyScene().RemoveRigidBody(Left);
	Engine::GetPhyScene().RemoveRigidBody(Right);
	Engine::GetPhyScene().RemoveRigidBody(Bottom);
}
