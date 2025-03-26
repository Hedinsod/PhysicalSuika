#include "pch.h"
#include "Glass.h"
#include "Systems/Engine.h"
#include "Renderer/GeometryComp.h"

AGlass::AGlass(float InX, float InY)
	: AActor(InX, InY)
{
	Geo = Engine::GetGraphics().CreateGeometry(this);

	FColor GlassColor({ 125, 125, 125 });

	Geo->SetVertices({
		-150, -500,
		-150,    0,
		 150,    0,
		 150, -500
		});
	Geo->SetIndecies({
		0, 1, 2, 3
		});
	Geo->BuildGeometry();

	Left = new CollisionBox(this, 0, -151, -150, -500);
	Right = new CollisionBox(this, 0, 150, 151, -500);
	Bottom = new CollisionBox(this, 1, -150, 150, 0);
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
