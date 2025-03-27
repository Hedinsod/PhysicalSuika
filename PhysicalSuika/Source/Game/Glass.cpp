#include "pch.h"
#include "Glass.h"
#include "Systems/Engine.h"
#include "Renderer/GeometryComp.h"

AGlass::AGlass(glm::vec2 InPos)
	: AActor(InPos, "Glass\n")
{
	Geo = Engine::GetGraphics().CreateGeometry(this);

	FColor GlassColor({ 125, 125, 125 });

	Geo->SetVertices({
		 -50, 160,
		 -50,  0,
		  50,  0,
		  50, 160
		});
	Geo->SetIndecies({
		0, 1, 2, 3
		});
	Geo->SetColors({
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		});
	Geo->BuildGeometry();

	Left = Engine::GetCollision().AddPhysics(this);
	Left->SetupBoundingBox(-51, 160, -50, 0);

	Right = Engine::GetCollision().AddPhysics(this);
	Right->SetupBoundingBox(50, 160, 51, 0);

	Bottom = Engine::GetCollision().AddPhysics(this);
	Bottom->SetupBoundingBox(-50, 0, 50, -1);
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
