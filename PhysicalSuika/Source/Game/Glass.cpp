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
	Geo->BuildGeometry();

	Left = new CollisionBox(this, 160, -53, -50, 0);
	Right = new CollisionBox(this, 160, 50, 53, 0);
	Bottom = new CollisionBox(this, 3, 0, 0, 0);
	Engine::GetCollision().AddCollisionBox(Left);
	Engine::GetCollision().AddCollisionBox(Right);
	Engine::GetCollision().AddCollisionBox(Bottom);
}

AGlass::~AGlass()
{
	Engine::GetGraphics().RemoveGeometry(Geo);

	Engine::GetCollision().RemoveCollisionBox(Left);
	Engine::GetCollision().RemoveCollisionBox(Right);
	Engine::GetCollision().RemoveCollisionBox(Bottom);
}

void AGlass::Tick()
{
}
