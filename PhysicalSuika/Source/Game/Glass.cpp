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

	Left = Engine::GetCollision().AddPhysics(this, 0.f, 0.9f);
	Left->SetupBoxCollider({ -5.5f, 8.0f }, -0.5f, 8.0f, 0.5f, -8.0f);

	Right = Engine::GetCollision().AddPhysics(this, 0.f, 0.9f);
	Right->SetupBoxCollider({ 5.5f, 8.0f }, -0.5f, 8.0f, 0.5f, -8.0f);

	Bottom = Engine::GetCollision().AddPhysics(this, 0.f, 0.9f);
	Bottom->SetupBoxCollider({ 0.f, -0.5f }, -5.0f, 0.5f, 5.0f, -0.5f);
}

AGlass::~AGlass()
{
	Engine::GetGraphics().RemoveGeometry(Geo);

	Engine::GetCollision().RemovePhysics(Left);
	Engine::GetCollision().RemovePhysics(Right);
	Engine::GetCollision().RemovePhysics(Bottom);
}

void AGlass::Tick()
{
}
