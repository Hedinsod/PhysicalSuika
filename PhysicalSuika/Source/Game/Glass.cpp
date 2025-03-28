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
		 -50, 160,
		 -50,   0,
		  50,   0,
		  50, 160
		});
	Geo->SetIndecies({
		0, 1, 2, 3
		});
	Geo->BuildGeometry();

	/*
	GeoBox = Engine::GetGraphics().CreateGeometry(this);
	GeoBox->SetVertices({
		-75,  0,
		 75,  0,
		 75, -1,
		-75, -1 });
	GeoBox->SetIndecies({
		0, 1, 2, 3, 0
		});
	GeoBox->BuildGeometry();
	*/

	Left = Engine::GetCollision().AddPhysics(this, 0.f, 0.9f);
	Left->SetupBoxCollider({ -50.5, 80 }, -51, 160, -50, 0);
	Left->SetLayer(1 | 2);

	Right = Engine::GetCollision().AddPhysics(this, 0.f, 0.9f);
	Right->SetupBoxCollider({ 50.5, 80 }, 50, 160, 51, 0);
	Right->SetLayer(1 | 4);

	Bottom = Engine::GetCollision().AddPhysics(this, 0.f, 0.9f);
	Bottom->SetupBoxCollider({ 0, -0.5 }, -50, 0, 50, -1);
	Bottom->SetLayer(1 | 8);
}

AGlass::~AGlass()
{
	Engine::GetGraphics().RemoveGeometry(Geo);
	//Engine::GetGraphics().RemoveGeometry(GeoBox);

	Engine::GetCollision().RemovePhysics(Left);
	Engine::GetCollision().RemovePhysics(Right);
	Engine::GetCollision().RemovePhysics(Bottom);
}

void AGlass::Tick()
{
}
